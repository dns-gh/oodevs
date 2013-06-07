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

type header struct {
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

func (w *Writer) Encode(tag uint32, msg proto.Message) error {
	data, err := proto.Marshal(msg)
	if err != nil {
		return err
	}
	header := header{
		Size: 4 /*tag*/ + uint32(len(data)),
		Tag:  tag,
	}
	err = binary.Write(w.io, binary.BigEndian, &header)
	if err != nil {
		return err
	}
	_, err = w.io.Write(data)
	return err
}

type Reader struct {
	io    io.Reader
	cache []uint8
}

func NewReader(r io.Reader) *Reader {
	return &Reader{r, make([]uint8, 64)}
}

func decode(msg *SwordMessage, tag uint32, data []uint8) error {
	var err error
	msg.tag = tag
	switch tag {
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

func (r *Reader) Decode(msg *SwordMessage) error {
	header := header{}
	err := binary.Read(r.io, binary.BigEndian, &header)
	if err != nil {
		return err
	}
	size := int(header.Size - 4)
	if size > MaxMessageSize {
		return fmt.Errorf("packet size too big %d", size)
	}
	if int(size) > len(r.cache) {
		r.cache = make([]uint8, size)
	}
	n, err := r.io.Read(r.cache[:size])
	if err != nil {
		return err
	}
	return decode(msg, header.Tag, r.cache[:n])
}
