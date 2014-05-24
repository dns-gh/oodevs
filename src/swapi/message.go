// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

import (
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"fmt"
	"io"
	"sword"
)

const (
	MaxMessageSize = 1 << 26
	// client -> * tags
	ClientToAarTag            = 496354747
	ClientToAuthenticationTag = 3393095543
	ClientToMessengerTag      = 2651046712
	ClientToReplayTag         = 745507513
	ClientToSimulationTag     = 496449857
	// * -> client tags
	AarToClientTag            = 3335378374
	AuthenticationToClientTag = 468259588
	DispatcherToClientTag     = 3012412245
	MessengerToClientTag      = 4281023046
	ReplayToClientTag         = 2824568610
	SimulationToClientTag     = 3540368649
	// probably useless, for reference...
	AdminToLauncherTag        = 487143762
	DispatcherToSimulationTag = 4036107609
	LauncherToAdminTag        = 1796776068
)

type Header struct {
	Size uint32
	Tag  uint32
}

type SwordMessage struct {
	tag      uint32
	Context  int32
	ClientId int32
	// * -> client
	AarToClient            *sword.AarToClient
	AuthenticationToClient *sword.AuthenticationToClient
	DispatcherToClient     *sword.DispatcherToClient
	MessengerToClient      *sword.MessengerToClient
	ReplayToClient         *sword.ReplayToClient
	SimulationToClient     *sword.SimToClient
	// client -> *
	ClientToAar            *sword.ClientToAar
	ClientToAuthentication *sword.ClientToAuthentication
	ClientToMessenger      *sword.ClientToMessenger
	ClientToReplay         *sword.ClientToReplay
	ClientToSimulation     *sword.ClientToSim
}

func (m *SwordMessage) SetContext(context int32) {
	protoctx := proto.Int32(context)
	if m.ClientToAar != nil {
		m.ClientToAar.Context = protoctx
		m.tag = ClientToAarTag
		return
	}
	if m.ClientToAuthentication != nil {
		m.ClientToAuthentication.Context = protoctx
		m.tag = ClientToAuthenticationTag
		return
	}
	if m.ClientToMessenger != nil {
		m.ClientToMessenger.Context = protoctx
		m.tag = ClientToMessengerTag
		return
	}
	if m.ClientToReplay != nil {
		m.ClientToReplay.Context = protoctx
		m.tag = ClientToReplayTag
		return
	}
	if m.ClientToSimulation != nil {
		m.ClientToSimulation.Context = protoctx
		m.tag = ClientToSimulationTag
		return
	}
	panic(fmt.Sprintf("unsupported client message: %v", m))
}

func (m *SwordMessage) GetMessage() proto.Message {
	if m.SimulationToClient != nil {
		return m.SimulationToClient
	}
	if m.ReplayToClient != nil {
		return m.ReplayToClient
	}
	if m.AarToClient != nil {
		return m.AarToClient
	}
	if m.AuthenticationToClient != nil {
		return m.AuthenticationToClient
	}
	if m.DispatcherToClient != nil {
		return m.DispatcherToClient
	}
	if m.MessengerToClient != nil {
		return m.MessengerToClient
	}
	if m.ClientToAar != nil {
		return m.ClientToAar
	}
	if m.ClientToAuthentication != nil {
		return m.ClientToAuthentication
	}
	if m.ClientToMessenger != nil {
		return m.ClientToMessenger
	}
	if m.ClientToReplay != nil {
		return m.ClientToReplay
	}
	if m.ClientToSimulation != nil {
		return m.ClientToSimulation
	}
	panic(fmt.Sprintf("unsupported client message: %v", m))
}

type Writer struct {
	io io.Writer
}

func NewWriter(io io.Writer) *Writer {
	return &Writer{io: io}
}

func EncodePayload(dst io.Writer, tag uint32, data []byte) (int, error) {
	header := Header{
		Size: 4 /*tag*/ + uint32(len(data)),
		Tag:  tag,
	}
	err := binary.Write(dst, binary.BigEndian, &header)
	if err != nil {
		return 0, err
	}
	_, err = dst.Write(data)
	return 4 + int(header.Size), err
}

func (w *Writer) Encode(tag uint32, msg proto.Message) (int, error) {
	data, err := proto.Marshal(msg)
	if err != nil {
		return 0, err
	}
	return EncodePayload(w.io, tag, data)
}

func (w *Writer) EncodeMessage(msg *SwordMessage) (int, error) {
	return w.Encode(msg.tag, msg.GetMessage())
}

type RawMessageHandler func(size, tag uint32, data []byte)

type Reader struct {
	io      io.Reader
	cache   []byte
	handler RawMessageHandler
}

func NewReader(r io.Reader) *Reader {
	return &Reader{io: r, cache: make([]byte, 64)}
}

func (r *Reader) SetHandler(handler RawMessageHandler) {
	r.handler = handler
}

func DecodeMessage(msg *SwordMessage, tag uint32, data []byte) error {
	var err error
	msg.tag = tag
	switch tag {
	case ClientToAarTag:
		msg.ClientToAar = &sword.ClientToAar{}
		err = proto.Unmarshal(data, msg.ClientToAar)
		msg.Context = msg.ClientToAar.GetContext()
	case ClientToAuthenticationTag:
		msg.ClientToAuthentication = &sword.ClientToAuthentication{}
		err = proto.Unmarshal(data, msg.ClientToAuthentication)
		msg.Context = msg.ClientToAuthentication.GetContext()
	case ClientToMessengerTag:
		msg.ClientToMessenger = &sword.ClientToMessenger{}
		err = proto.Unmarshal(data, msg.ClientToMessenger)
		msg.Context = msg.ClientToMessenger.GetContext()
	case ClientToReplayTag:
		msg.ClientToReplay = &sword.ClientToReplay{}
		err = proto.Unmarshal(data, msg.ClientToReplay)
		msg.ClientId = msg.ClientToReplay.GetClientId()
		msg.Context = msg.ClientToReplay.GetContext()
	case ClientToSimulationTag:
		msg.ClientToSimulation = &sword.ClientToSim{}
		err = proto.Unmarshal(data, msg.ClientToSimulation)
		msg.ClientId = msg.ClientToSimulation.GetClientId()
		msg.Context = msg.ClientToSimulation.GetContext()
	case AarToClientTag:
		msg.AarToClient = &sword.AarToClient{}
		err = proto.Unmarshal(data, msg.AarToClient)
		msg.Context = msg.AarToClient.GetContext()
	case AuthenticationToClientTag:
		// XXX: context field in AuthenticationToClient is invalid
		msg.AuthenticationToClient = &sword.AuthenticationToClient{}
		err = proto.Unmarshal(data, msg.AuthenticationToClient)
		msg.ClientId = msg.AuthenticationToClient.GetClientId()
		msg.Context = msg.AuthenticationToClient.GetContext()
	case DispatcherToClientTag:
		// XXX: missing context field in DispatcherToClient
		msg.DispatcherToClient = &sword.DispatcherToClient{}
		err = proto.Unmarshal(data, msg.DispatcherToClient)
	case MessengerToClientTag:
		// XXX: context field not set by sender
		msg.MessengerToClient = &sword.MessengerToClient{}
		err = proto.Unmarshal(data, msg.MessengerToClient)
		msg.Context = msg.MessengerToClient.GetContext()
	case ReplayToClientTag:
		msg.ReplayToClient = &sword.ReplayToClient{}
		err = proto.Unmarshal(data, msg.ReplayToClient)
		msg.ClientId = msg.ReplayToClient.GetClientId()
		msg.Context = msg.ReplayToClient.GetContext()
	case SimulationToClientTag:
		msg.SimulationToClient = &sword.SimToClient{}
		err = proto.Unmarshal(data, msg.SimulationToClient)
		msg.Context = msg.SimulationToClient.GetContext()
		msg.ClientId = msg.SimulationToClient.GetClientId()
	default:
		return fmt.Errorf("unknown tag: %d", tag)
	}
	return err
}

func (r *Reader) Parse(header *Header, buffer []byte) ([]byte, error) {
	err := binary.Read(r.io, binary.BigEndian, header)
	if err != nil {
		return nil, err
	}
	size := int(header.Size - 4)
	if size > MaxMessageSize {
		return nil, fmt.Errorf("packet size too big %d", size)
	}
	data := buffer
	if int(size) > cap(buffer) {
		data = make([]byte, size)
	}
	n, err := io.ReadFull(r.io, data[:size])
	if err != nil && (n == 0 || err != io.EOF) {
		return nil, err
	}
	return data[:n], nil
}

func (r *Reader) Decode(msg *SwordMessage) (int, error) {
	header := Header{}
	data, err := r.Parse(&header, r.cache)
	if err != nil {
		return 0, err
	}
	r.cache = data
	if r.handler != nil {
		r.handler(header.Size, header.Tag, r.cache)
	}
	return int(4 + header.Size), DecodeMessage(msg, header.Tag, r.cache)
}
