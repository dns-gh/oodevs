// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package server

import (
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"net/http"
	"net/url"
	"strconv"
	"strings"
	"swapi"
	"sword"
	"sync/atomic"
)

const (
	SkipEncoding = 0
)

type TcpProxy struct {
	http    int
	tcp     int
	scheme  string
	verbose int
	sword   *int
}

func NewTcpProxy(http, tcp, verbose int, ssl bool, sword *int) *TcpProxy {
	scheme := "http"
	if ssl {
		scheme = "https"
	}
	return &TcpProxy{
		http:    http,
		tcp:     tcp,
		scheme:  scheme,
		verbose: verbose,
		sword:   sword,
	}
}

func (t *TcpProxy) Run() error {
	listener, err := net.Listen("tcp", fmt.Sprintf(":%d", t.tcp))
	if err != nil {
		return err
	}
	id := int32(0)
	for {
		link, err := listener.Accept()
		if err != nil {
			return err
		}
		go t.handleClient(link, id)
		id++
	}
}

type Packet struct {
	tag  uint32
	data []byte
	done chan struct{}
}

func postPacket(ctx *TcpContext, dst chan<- *Packet, tag uint32, data []byte) {
	pkt := &Packet{
		tag:  tag,
		data: data,
		done: make(chan struct{}),
	}
	select {
	case <-ctx.quit:
		return
	case dst <- pkt:
	}
	<-pkt.done
}

func postRawPacket(ctx *TcpContext, dst chan<- *Packet, data []byte) {
	postPacket(ctx, dst, SkipEncoding, data)
}

type TcpContext struct {
	errors   chan error
	client   chan *Packet
	server   chan *Packet
	done     uint32
	profiles map[string]struct{}
	quit     chan struct{}
}

func NewTcpContext() *TcpContext {
	return &TcpContext{
		errors:   make(chan error),
		client:   make(chan *Packet),
		profiles: map[string]struct{}{},
		quit:     make(chan struct{}),
	}
}

func (t *TcpProxy) handleClient(link net.Conn, id int32) {
	log.Println("+ client", link.RemoteAddr())
	defer log.Println("- client", link.RemoteAddr())
	defer link.Close()
	ctx := NewTcpContext()
	if t.sword != nil {
		err := t.openServer(ctx, *t.sword)
		if err != nil {
			log.Printf("Unable to open server %d: %s\n", *t.sword, err)
			return
		}
	}
	go t.readClient(ctx, link)
	if t.sword == nil {
		go t.publishAuthService(ctx)
	}
	t.writePackets(ctx, link, ctx.client)
}

func (t *TcpContext) stop() bool {
	first := atomic.AddUint32(&t.done, 1) == 1
	if first {
		close(t.quit)
	}
	return first
}

type PayloadReader func(tag uint32, data []byte) bool
type MessageReader func(msg *swapi.SwordMessage) error

func (t *TcpProxy) readPackets(ctx *TcpContext, link net.Conn, readPayload PayloadReader, readMessage MessageReader) {
	defer ctx.stop()
	header := swapi.Header{}
	buffer := []byte{}
	reader := swapi.NewReader(link)
	for {
		var err error
		buffer, err = reader.Parse(&header, buffer)
		if err != nil {
			if !ctx.isClosed(err) {
				log.Printf("unable to read packet header from %s: %s\n", link.RemoteAddr(), err)
			}
			return
		}
		if t.verbose > 0 {
			log.Println(8+len(buffer), "bytes read from", link.RemoteAddr(), header, dumpMessage(header.Tag, buffer))
		}
		if readPayload(header.Tag, buffer) {
			continue
		}
		msg := swapi.SwordMessage{}
		err = swapi.DecodeMessage(&msg, header.Tag, buffer)
		if err != nil {
			log.Printf("unable to decode message from %s: %s\n", link.RemoteAddr(), err)
			return
		}
		err = readMessage(&msg)
		if err != nil {
			log.Printf("unable to process message from %s: %s\n", link.RemoteAddr(), err)
			return
		}
	}
}

func (t *TcpProxy) readClient(ctx *TcpContext, link net.Conn) {
	filterPayload := func(tag uint32, data []byte) bool {
		if ctx.server == nil {
			return false
		}
		postPacket(ctx, ctx.server, tag, data)
		return true
	}
	readMessage := func(msg *swapi.SwordMessage) error {
		err := fmt.Errorf("access denied")
		if msg.ClientToAuthentication != nil {
			err = t.clientToAuthentication(ctx, msg.ClientToAuthentication)
		}
		if err == nil {
			return nil
		}
		log.Printf("error on %s: %s\n", link.RemoteAddr(), err)
		return t.deny(ctx, msg)
	}
	t.readPackets(ctx, link, filterPayload, readMessage)
}

func (t *TcpProxy) clientToAuthentication(ctx *TcpContext, msg *sword.ClientToAuthentication) error {
	context := msg.GetContext()
	sub := msg.GetMessage()
	if sub.GetDisconnectionRequest() != nil {
		ctx.stop()
	} else if auth := sub.GetAuthenticationRequest(); auth != nil {
		return t.fixAuthentication(ctx, context, auth)
	}
	return nil
}

type JsonRestrictedUser struct {
	Name     string
	Profiles []string
}

type JsonRestricted struct {
	Enabled string
	List    map[string]JsonRestrictedUser
}

type JsonSession struct {
	Port        int `json:",string"`
	CurrentUser int `json:"current_user,string"`
	Restricted  JsonRestricted
}

func (c *TcpContext) readFilters(s *JsonSession) error {
	ok, err := strconv.ParseBool(s.Restricted.Enabled)
	if err != nil {
		return err
	}
	if !ok {
		return nil
	}
	user := strconv.FormatInt(int64(s.CurrentUser), 10)
	filter, ok := s.Restricted.List[user]
	if !ok {
		return nil
	}
	for _, v := range filter.Profiles {
		c.profiles[v] = struct{}{}
	}
	return nil
}

func (t *TcpProxy) fixAuthentication(ctx *TcpContext, context int32, auth *sword.AuthenticationRequest) error {
	parts := strings.SplitN(auth.GetPassword(), "$", 3)
	if len(parts) != 3 {
		return fmt.Errorf("invalid password format %s", auth.GetPassword())
	}
	values := url.Values{}
	values.Add("sid", parts[0])
	values.Add("id", parts[1])
	values.Add("current_user", "true")
	url := url.URL{}
	url.Scheme = t.scheme
	url.Host = fmt.Sprintf("localhost:%d", t.http)
	url.Path = "/api/get_session"
	url.RawQuery = values.Encode()
	rpy, err := http.Get(url.String())
	if err != nil {
		return err
	}
	defer rpy.Body.Close()
	body, err := ioutil.ReadAll(rpy.Body)
	if err != nil {
		return err
	}
	session := JsonSession{}
	err = json.Unmarshal(body, &session)
	if err != nil {
		return err
	}
	err = ctx.readFilters(&session)
	if err != nil {
		return err
	}
	err = t.openServer(ctx, session.Port)
	if err != nil {
		return err
	}
	return t.login(ctx, context, auth, parts[2])
}

func dumpMessage(tag uint32, src []byte) string {
	msg := swapi.SwordMessage{}
	err := swapi.DecodeMessage(&msg, tag, src)
	if err != nil {
		return fmt.Sprintf("unable to decode message: %s", err)
	}
	data, err := json.MarshalIndent(msg.GetMessage(), "", "\t")
	if err != nil {
		return fmt.Sprintf("unable to encode to json: %s", err)
	}
	return string(data)
}

func dump(data []byte) string {
	reader := swapi.NewReader(bytes.NewReader(data))
	header := swapi.Header{}
	payload, err := reader.Parse(&header, []byte{})
	if err != nil {
		return fmt.Sprintf("unable to parse header: %s", err)
	}
	return dumpMessage(header.Tag, payload)
}

func createMessage(tag uint32, msg proto.Message) ([]byte, error) {
	buffer := bytes.Buffer{}
	writer := swapi.NewWriter(&buffer)
	_, err := writer.Encode(tag, msg)
	if err != nil {
		return nil, err
	}
	return buffer.Bytes(), nil

}

func (t *TcpProxy) login(ctx *TcpContext, context int32, auth *sword.AuthenticationRequest, password string) error {
	auth.Password = proto.String(password)
	data, err := createMessage(swapi.ClientToAuthenticationTag,
		&sword.ClientToAuthentication{
			Context: proto.Int32(context),
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: auth,
			},
		})
	if err != nil {
		return err
	}
	postRawPacket(ctx, ctx.server, data)
	return nil
}

func (t *TcpProxy) deny(ctx *TcpContext, msg *swapi.SwordMessage) error {
	data, err := createMessage(swapi.AuthenticationToClientTag,
		&sword.AuthenticationToClient{
			Context: proto.Int32(msg.Context),
			Message: &sword.AuthenticationToClient_Content{
				AuthenticationResponse: &sword.AuthenticationResponse{
					ErrorCode: sword.AuthenticationResponse_invalid_login.Enum(),
					ServerVersion: &sword.ProtocolVersion{
						Value: proto.String(sword.Default_ProtocolVersion_Value),
					},
				},
			},
		})
	if err != nil {
		return err
	}
	postRawPacket(ctx, ctx.client, data)
	return nil
}

// Tell connected clients we support authentication service. We'll use the
// password triplet to determine where to dispatch the current connection. We
// cannot just wait for clients to login as some of them wait for those
// services to be broadcast before doing anything
func (t *TcpProxy) publishAuthService(ctx *TcpContext) {
	data, err := createMessage(swapi.DispatcherToClientTag,
		&sword.DispatcherToClient{
			Message: &sword.DispatcherToClient_Content{
				ServicesDescription: &sword.ServicesDescription{
					Services: []string{"struct authentication::Service"},
				},
			},
		})
	if err != nil {
		ctx.stop()
	}
	postRawPacket(ctx, ctx.client, data)
}

func (t *TcpProxy) writePackets(ctx *TcpContext, link net.Conn, packets <-chan *Packet) {
	defer ctx.stop()
	for {
		select {
		case <-ctx.quit:
			return
		case packet := <-packets:
			var written int
			var err error
			if packet.tag == SkipEncoding {
				written, err = link.Write(packet.data)
			} else {
				written, err = swapi.EncodePayload(link, packet.tag, packet.data)
			}
			if t.verbose > 0 {
				var dumped string
				if packet.tag == SkipEncoding {
					dumped = dump(packet.data)
				} else {
					dumped = dumpMessage(packet.tag, packet.data)
				}
				log.Println(written, "bytes written to", link.RemoteAddr(), dumped)
			}
			close(packet.done)
			if err != nil {
				if !ctx.isClosed(err) {
					log.Printf("Unable to write data to %s: %s\n", link.RemoteAddr(), err)
				}
				return
			}
		}
	}
}

func (t *TcpProxy) openServer(ctx *TcpContext, port int) error {
	host := fmt.Sprintf("localhost:%d", port)
	link, err := net.Dial("tcp", host)
	if err != nil {
		return fmt.Errorf("unable to open %s: %s", host, err)
	}
	ctx.server = make(chan *Packet)
	go t.handleServer(ctx, link)
	return nil
}

func (t *TcpProxy) filterAuthentication(ctx *TcpContext, msg *swapi.SwordMessage) {
	m := msg.AuthenticationToClient
	if m == nil {
		return
	}
	profiles := m.GetMessage().GetAuthenticationResponse().GetProfiles().GetElem()
	if profiles == nil {
		return
	}
	filtered := []*sword.ProfileDescription{}
	for _, p := range profiles {
		if _, ok := ctx.profiles[p.GetLogin()]; ok {
			filtered = append(filtered, p)
		}
	}
	m.Message.AuthenticationResponse.Profiles.Elem = filtered
}

func (t *TcpProxy) handleServer(ctx *TcpContext, link net.Conn) {
	log.Println("+ server", link.RemoteAddr())
	defer log.Println("- server", link.RemoteAddr())
	defer link.Close()
	filter := len(ctx.profiles) > 0
	filterPayload := func(tag uint32, data []byte) bool {
		if filter && tag == swapi.AuthenticationToClientTag {
			return false
		}
		postPacket(ctx, ctx.client, tag, data)
		return true
	}
	readMessage := func(msg *swapi.SwordMessage) error {
		t.filterAuthentication(ctx, msg)
		buffer := bytes.Buffer{}
		writer := swapi.NewWriter(&buffer)
		_, err := writer.EncodeMessage(msg)
		if err != nil {
			return err
		}
		postRawPacket(ctx, ctx.client, buffer.Bytes())
		return nil
	}
	go t.readPackets(ctx, link, filterPayload, readMessage)
	t.writePackets(ctx, link, ctx.server)
}

func (ctx *TcpContext) isClosed(err error) bool {
	if err == io.EOF {
		return true
	}
	return atomic.LoadUint32(&ctx.done) > 0
}
