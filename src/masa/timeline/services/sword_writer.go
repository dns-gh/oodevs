// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/util"
	"time"
)

// Those functions WILL be called in *some* thread
// so the receiver MUST NOT block on those calls
// and MUST pay attention to synchronisation
type SwordWriterPoster interface {
	Close()
	CloseAction(err error)
	Log(format string, args ...interface{})
}

type SwordWriter struct {
	poster SwordWriterPoster
	action string
	async  *SwapiAsync
}

func NewSwordWriter(poster SwordWriterPoster, action string) *SwordWriter {
	return &SwordWriter{
		poster: poster,
		action: action,
		async:  NewSwapiAsync(),
	}
}

func processAck(code int32, msg, def string) error {
	if code == 0 {
		return nil
	}
	if len(msg) == 0 {
		return util.NewError(int32(code), def)
	}
	return util.NewError(int32(code), msg)
}

func (s *SwordWriter) waitAck(msg *swapi.SwordMessage, client, context int32, err error) bool {
	if err != nil {
		s.closeAction(err)
		return true
	}
	if msg.SimulationToClient == nil {
		return false
	}
	if msg.ClientId != client || msg.Context != context {
		return false
	}
	m := msg.SimulationToClient.GetMessage()
	if ack := m.OrderAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.FragOrderAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.MagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.UnitMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(code, ack.GetErrorMsg(), proto.EnumName(sword.UnitActionAck_ErrorCode_name, code))
	} else if ack := m.ObjectMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.KnowledgeGroupMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.SetAutomatModeAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), code.String(), code.String())
	}
	s.closeAction(err)
	return true
}

func (s *SwordWriter) Write(client *swapi.Client, action *Action) {
	s.async.Start(s.waitAck)
	client.PostWithTimeout(action.msg, s.async.Write, 5*time.Minute)
}

func (s *SwordWriter) Close() {
	s.async.Close()
}

func (s *SwordWriter) closeAction(err error) {
	defer s.poster.Close()
	// ignore connection errors
	// target action will be tried again eventually
	if swapi.IsConnectionError(err) || err == swapi.ErrConnectionClosed {
		s.poster.Log("<- action %v error: %v (will retry)", s.action, err)
		return
	}
	suffix := ""
	if err != nil {
		suffix = fmt.Sprintf(" error: %v", err)
	}
	s.poster.Log("<- action %v%v", s.action, suffix)
	s.poster.CloseAction(err)
}
