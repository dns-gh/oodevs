// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package client

import (
	"masa/timeline/sdk"
	"masa/timeline/server"
	"masa/timeline/util"
	"net/http"
)

var (
	ErrMissingService = util.NewError(http.StatusBadRequest, "missing service")
	ErrMissingEvent   = util.NewError(http.StatusBadRequest, "missing event")
	ErrUnknownCommand = util.NewError(http.StatusBadRequest, "invalid command")
)

type Handler interface {
	Process(msg *sdk.ClientRequest) error
}

type SdkHandler struct {
	controller server.SdkController
}

func MakeHandler(controller server.SdkController) *SdkHandler {
	return &SdkHandler{controller}
}

func (s *SdkHandler) Process(msg *sdk.ClientRequest) error {
	switch msg.GetType() {
	case sdk.Type_SESSION_CREATE:
		return s.Create(msg.GetSession().GetCreate())
	case sdk.Type_SESSION_START:
		return s.Start(msg.GetSession().GetStart())
	case sdk.Type_SESSION_ATTACH:
		return s.Attach(msg.GetSession().GetAttach())
	case sdk.Type_EVENT_CREATE:
		return s.CreateEvent(msg.GetEvent().GetCreate())
	}
	return ErrUnknownCommand
}

func (s *SdkHandler) Create(args *sdk.SessionCreate) error {
	_, err := s.controller.CreateSession(args.GetUuid(), args.GetName())
	return err
}

func (s *SdkHandler) Attach(args *sdk.SessionAttach) error {
	err := ErrMissingService
	src := args.GetService()
	uuid := args.GetUuid()
	name := src.GetName()
	if timer := src.GetTimer(); timer != nil {
		_, err = s.controller.AttachTimerService(uuid, name,
			timer.GetBase())
	} else if sword := src.GetSword(); sword != nil {
		_, err = s.controller.AttachSwordService(uuid, name,
			src.GetClock(), sword.GetAddress())
	}
	return err
}

func (s *SdkHandler) Start(args *sdk.SessionStart) error {
	_, err := s.controller.StartSession(args.GetUuid(), args.GetOffset())
	return err
}

func (s *SdkHandler) CreateEvent(args *sdk.EventCreate) error {
	_, err := s.controller.CreateEvent(args.GetSession(), args.GetEvent())
	return err
}
