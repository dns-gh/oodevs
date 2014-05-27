// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swfake

import (
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"io"
	"net"
	"swapi"
	"sword"
	"sync"
	"sync/atomic"
	"time"
	"util"
)

var (
	SwordTimeLayout = "20060102T150405"
)

type SwordOutput struct {
	tag uint32
	msg proto.Message
}

type SwordLink struct {
	link    net.Conn
	id      int32
	writes  chan SwordOutput
	errors  chan error
	pending sync.WaitGroup
	quit    chan struct{}
}

type Error struct {
	Code int32
	Text string
}

type SwordServer struct {
	log         util.Logger
	host        string
	verbose     bool
	listener    net.Listener
	quit        chan bool
	pending     sync.WaitGroup
	mutex       sync.Mutex
	lastOrderId uint32
	links       map[*SwordLink]struct{}
	lastError   *Error
}

func NewSwordServer(log util.Logger, host string, ticks, verbose bool) (*SwordServer, error) {
	listener, err := net.Listen("tcp", host)
	if err != nil {
		return nil, err
	}
	s := SwordServer{
		log:      log,
		host:     host,
		verbose:  verbose,
		listener: listener,
		quit:     make(chan bool),
		links:    map[*SwordLink]struct{}{},
	}
	s.pending.Add(1)
	go s.Run()
	if ticks {
		s.pending.Add(1)
		go s.RunTicks()
	}
	return &s, nil
}

func (s *SwordServer) Run() {
	id := int32(0)
	for {
		link, err := s.listener.Accept()
		if err != nil {
			break
		}
		s.pending.Add(1)
		id++
		go s.HandleLink(link, id)
	}
	close(s.quit)
	s.pending.Done()
}

func (s *SwordServer) Close() {
	s.listener.Close()
	s.pending.Wait()
}

func (s *SwordServer) RunTicks() {
	defer s.pending.Done()
	tick := int32(0)
	for {
		select {
		case <-s.quit:
			return
		case now := <-time.After(time.Second):
			s.broadcastContent(0, &sword.SimToClient_Content{
				ControlBeginTick: &sword.ControlBeginTick{
					CurrentTick: proto.Int32(tick),
					DateTime: &sword.DateTime{
						Data: proto.String(now.Format(SwordTimeLayout)),
					},
				},
			})
			tick++
		}
	}
}

func (s *SwordServer) CreateProfile(profile *sword.Profile) {
	s.broadcastContent(0, &sword.AuthenticationToClient_Content{
		ProfileCreation: &sword.ProfileCreation{
			Profile: profile,
		},
	})
}

func (s *SwordServer) DestroyProfile(name string) {
	s.broadcastContent(0, &sword.AuthenticationToClient_Content{
		ProfileDestruction: &sword.ProfileDestruction{
			Login: proto.String(name),
		},
	})
}

func (s *SwordServer) replyAuthenticationRequest(slink *SwordLink, ctx int32, auth *sword.AuthenticationRequest) {
	username := auth.GetLogin()
	password := auth.GetPassword()
	key := auth.GetAuthenticationKey()
	code := sword.AuthenticationResponse_success
	if len(key) == 0 || username != "" || password != "" {
		code = sword.AuthenticationResponse_invalid_login
	}
	s.writeContent(slink, ctx, &sword.AuthenticationToClient_Content{
		AuthenticationResponse: &sword.AuthenticationResponse{
			ErrorCode: code.Enum(),
			ServerVersion: &sword.ProtocolVersion{
				Value: proto.String(sword.Default_ProtocolVersion_Value),
			},
		},
	})
}

func (s *SwordServer) replyAuthenticationKeyRequest(slink *SwordLink, ctx int32) {
	s.writeContent(slink, ctx, &sword.AuthenticationToClient_Content{
		AuthenticationKeyResponse: &sword.AuthenticationKeyResponse{
			AuthenticationKey: proto.String("key"),
		},
	})
}

func (s *SwordLink) Fatal(err error) {
	select {
	case <-s.quit:
	case s.errors <- err:
	}
}

func (s *SwordLink) GetId() int32 {
	return s.id
}

func (s *SwordServer) clientToAuthentication(slink *SwordLink, msg *sword.ClientToAuthentication) {
	ctx := msg.GetContext()
	if msg.GetMessage().GetDisconnectionRequest() != nil {
		slink.Fatal(io.EOF)
	} else if auth := msg.GetMessage().GetAuthenticationRequest(); auth != nil {
		s.replyAuthenticationRequest(slink, ctx, auth)
	} else if auth := msg.GetMessage().GetAuthenticationKeyRequest(); auth != nil {
		s.replyAuthenticationKeyRequest(slink, ctx)
	} else {
		slink.Fatal(fmt.Errorf("unable to reply to %v", msg))
	}
}

func (s *SwordServer) getOrderId() uint32 {
	return atomic.AddUint32(&s.lastOrderId, 1)
}

func (slink *SwordLink) write(msg SwordOutput) {
	select {
	case <-slink.quit:
	case slink.writes <- msg:
	}
}

func (s *SwordServer) writeContent(slink *SwordLink, ctx int32, message proto.Message) {
	var output SwordOutput
	switch field := message.(type) {
	case *sword.SimToClient_Content:
		output = SwordOutput{
			swapi.SimulationToClientTag,
			&sword.SimToClient{
				Context:  proto.Int32(ctx),
				ClientId: proto.Int32(slink.id),
				Message:  field,
			},
		}
	case *sword.AuthenticationToClient_Content:
		output = SwordOutput{
			swapi.AuthenticationToClientTag,
			&sword.AuthenticationToClient{
				Context:  proto.Int32(ctx),
				ClientId: proto.Int32(slink.id),
				Message:  field,
			},
		}
	default:
		panic("unsupported message type " + message.String())
	}
	slink.write(output)
}

func (s *SwordServer) broadcastContent(ctx int32, message proto.Message) {
	s.mutex.Lock()
	for it, _ := range s.links {
		it.pending.Add(1)
		go func(slink *SwordLink) {
			defer slink.pending.Done()
			s.writeContent(slink, ctx, message)
		}(it)
	}
	s.mutex.Unlock()
}

func (s *SwordServer) resetLastError() (int32, *string) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if s.lastError == nil {
		return 0, nil
	}
	err := s.lastError
	s.lastError = nil
	return err.Code, &err.Text
}

func (s *SwordServer) writeOrderAck(slink *SwordLink, ctx int32, id *uint32, tasker *sword.Tasker) (int32, *string) {
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		OrderAck: &sword.TaskCreationRequestAck{
			Tasker:    tasker,
			ErrorCode: sword.OrderAck_ErrorCode(code).Enum(),
			ErrorMsg:  text,
			Id:        id,
		},
	})
	return code, text
}

func (s *SwordServer) replyUnitOrder(slink *SwordLink, ctx int32, order *sword.UnitOrder) {
	order.Id = proto.Uint32(s.getOrderId())
	code, text := s.writeOrderAck(slink, ctx, order.Id, &sword.Tasker{
		Unit: order.GetTasker(),
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		UnitOrder: order,
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:        order.Id,
			UnitOrder: order,
			ErrorCode: &code,
			ErrorMsg:  text,
		},
	})
}

func (s *SwordServer) replyAutomatOrder(slink *SwordLink, ctx int32, order *sword.AutomatOrder) {
	order.Id = proto.Uint32(s.getOrderId())
	code, text := s.writeOrderAck(slink, ctx, order.Id, &sword.Tasker{
		Automat: order.GetTasker(),
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		AutomatOrder: order,
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:           order.Id,
			AutomatOrder: order,
			ErrorCode:    &code,
			ErrorMsg:     text,
		},
	})
}

func (s *SwordServer) replyCrowdOrder(slink *SwordLink, ctx int32, order *sword.CrowdOrder) {
	order.Id = proto.Uint32(s.getOrderId())
	code, text := s.writeOrderAck(slink, ctx, order.Id, &sword.Tasker{
		Crowd: order.GetTasker(),
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		CrowdOrder: order,
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:         order.Id,
			CrowdOrder: order,
			ErrorCode:  &code,
			ErrorMsg:   text,
		},
	})
}

func (s *SwordServer) replyFragOrder(slink *SwordLink, ctx int32, order *sword.FragOrder) {
	order.Id = proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		FragOrderAck: &sword.FragOrderAck{
			Tasker:    order.GetTasker(),
			ErrorCode: sword.OrderAck_ErrorCode(code).Enum(),
			ErrorMsg:  text,
			Id:        order.Id,
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		FragOrder: order,
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:        order.Id,
			FragOrder: order,
			ErrorCode: &code,
			ErrorMsg:  text,
		},
	})
}

func (s *SwordServer) replyMagicOrder(slink *SwordLink, ctx int32, order *sword.MagicAction) {
	id := proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		MagicActionAck: &sword.MagicActionAck{
			Id:        id,
			ErrorCode: sword.MagicActionAck_ErrorCode(code).Enum(),
			ErrorMsg:  text,
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:          id,
			MagicAction: order,
			ErrorCode:   &code,
			ErrorMsg:    text,
		},
	})
}

func (s *SwordServer) replyUnitMagicOrder(slink *SwordLink, ctx int32, order *sword.UnitMagicAction) {
	id := proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		UnitMagicActionAck: &sword.UnitMagicActionAck{
			Id:        id,
			Unit:      order.GetTasker().GetUnit(),
			ErrorCode: sword.UnitActionAck_ErrorCode(code).Enum(),
			ErrorMsg:  text,
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:              id,
			UnitMagicAction: order,
			ErrorCode:       &code,
			ErrorMsg:        text,
		},
	})
}

func (s *SwordServer) replyObjectMagicOrder(slink *SwordLink, ctx int32, order *sword.ObjectMagicAction) {
	id := proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		ObjectMagicActionAck: &sword.ObjectMagicActionAck{
			Id:        id,
			ErrorCode: sword.ObjectMagicActionAck_ErrorCode(code).Enum(),
			ErrorMsg:  text,
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:                id,
			ObjectMagicAction: order,
			ErrorCode:         &code,
			ErrorMsg:          text,
		},
	})
}

func (s *SwordServer) replyKnowledgeMagicOrder(slink *SwordLink, ctx int32, order *sword.KnowledgeMagicAction) {
	id := proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		KnowledgeGroupMagicActionAck: &sword.KnowledgeGroupMagicActionAck{
			Id:             id,
			KnowledgeGroup: order.KnowledgeGroup,
			ErrorCode:      sword.KnowledgeGroupAck_ErrorCode(code).Enum(),
			ErrorMsg:       text,
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:                   id,
			KnowledgeMagicAction: order,
			ErrorCode:            &code,
			ErrorMsg:             text,
		},
	})
}

func (s *SwordServer) replyAutomatModeMagicOrder(slink *SwordLink, ctx int32, order *sword.SetAutomatMode) {
	id := proto.Uint32(s.getOrderId())
	code, text := s.resetLastError()
	s.writeContent(slink, ctx, &sword.SimToClient_Content{
		SetAutomatModeAck: &sword.SetAutomatModeAck{
			Id:        id,
			Automate:  order.Automate,
			ErrorCode: sword.SetAutomatModeAck_ErrorCode(code).Enum(),
		},
	})
	s.broadcastContent(ctx, &sword.SimToClient_Content{
		Action: &sword.Action{
			Id:             id,
			SetAutomatMode: order,
			ErrorCode:      &code,
			ErrorMsg:       text,
		},
	})
}

func (s *SwordServer) clientToSimulation(slink *SwordLink, msg *sword.ClientToSim) {
	ctx := msg.GetContext()
	if order := msg.GetMessage().GetUnitOrder(); order != nil {
		s.replyUnitOrder(slink, ctx, order)
	} else if order := msg.GetMessage().AutomatOrder; order != nil {
		s.replyAutomatOrder(slink, ctx, order)
	} else if order := msg.GetMessage().CrowdOrder; order != nil {
		s.replyCrowdOrder(slink, ctx, order)
	} else if order := msg.GetMessage().FragOrder; order != nil {
		s.replyFragOrder(slink, ctx, order)
	} else if order := msg.GetMessage().MagicAction; order != nil {
		s.replyMagicOrder(slink, ctx, order)
	} else if order := msg.GetMessage().UnitMagicAction; order != nil {
		s.replyUnitMagicOrder(slink, ctx, order)
	} else if order := msg.GetMessage().ObjectMagicAction; order != nil {
		s.replyObjectMagicOrder(slink, ctx, order)
	} else if order := msg.GetMessage().KnowledgeMagicAction; order != nil {
		s.replyKnowledgeMagicOrder(slink, ctx, order)
	} else if order := msg.GetMessage().SetAutomatMode; order != nil {
		s.replyAutomatModeMagicOrder(slink, ctx, order)
	} else {
		slink.Fatal(fmt.Errorf("unable to reply to %v", msg))
	}
}

func (s *SwordServer) readMessages(slink *SwordLink) {
	defer slink.pending.Done()
	idx := 0
	reader := swapi.NewReader(slink.link)
	for {
		msg := swapi.SwordMessage{}
		_, err := reader.Decode(&msg)
		if err != nil {
			slink.Fatal(err)
			return
		}
		if s.verbose {
			s.log.Printf("read %v %v %v\n", idx, slink.id, msg)
		}
		idx++
		if msg.ClientToAuthentication != nil {
			s.clientToAuthentication(slink, msg.ClientToAuthentication)
		} else if msg.ClientToSimulation != nil {
			s.clientToSimulation(slink, msg.ClientToSimulation)
		}
	}
}

func (s *SwordServer) writeMessages(slink *SwordLink) {
	writer := swapi.NewWriter(slink.link)
	idx := 0
	for output := range slink.writes {
		if s.verbose {
			s.log.Printf("write %v %v %v\n", idx, slink.id, output.msg)
		}
		idx++
		_, err := writer.Encode(output.tag, output.msg)
		if err != nil {
			slink.Fatal(err)
			return
		}
	}
}

func (s *SwordServer) RegisterLink(link net.Conn, id int32) *SwordLink {
	s.mutex.Lock()
	slink := &SwordLink{
		link:   link,
		id:     id,
		writes: make(chan SwordOutput),
		errors: make(chan error),
		quit:   make(chan struct{}),
	}
	s.links[slink] = struct{}{}
	s.mutex.Unlock()
	return slink
}

func (s *SwordServer) UnregisterLink(slink *SwordLink) {
	s.mutex.Lock()
	delete(s.links, slink)
	s.mutex.Unlock()
	// kill everything but the write goroutine
	close(slink.quit)
	slink.link.Close()
	slink.pending.Wait()
	close(slink.writes)
}

func (s *SwordServer) HandleLink(link net.Conn, id int32) {
	defer s.pending.Done()
	slink := s.RegisterLink(link, id)
	defer s.UnregisterLink(slink)
	slink.pending.Add(1) // read goroutine
	go s.readMessages(slink)
	go s.writeMessages(slink)
	select {
	case <-s.quit: // quit on close
	case err := <-slink.errors: // quit on read error
		if err != io.EOF {
			s.log.Printf("unexpected error %v\n", err)
		}
	}
}

func (s *SwordServer) ConnectedCount() int {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	return len(s.links)
}

type DetachedLinks map[*SwordLink]struct{}

func (d DetachedLinks) Close() {
	for slink := range d {
		slink.pending.Done()
	}
}

func (s *SwordServer) GetLinks() DetachedLinks {
	slinks := DetachedLinks{}
	s.mutex.Lock()
	defer s.mutex.Unlock()
	for slink := range s.links {
		slink.pending.Add(1)
		slinks[slink] = struct{}{}
	}
	return slinks
}

func (s *SwordServer) WriteToClient(slink *SwordLink, ctx, client int32, msg *sword.SimToClient_Content) {
	slink.write(SwordOutput{
		swapi.SimulationToClientTag,
		&sword.SimToClient{
			Context:  proto.Int32(ctx),
			ClientId: proto.Int32(client),
			Message:  msg,
		},
	})
}

func (s *SwordServer) SetLastError(err *Error) {
	s.mutex.Lock()
	s.lastError = err
	s.mutex.Unlock()
}
