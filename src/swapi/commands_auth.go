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
	"errors"
	"fmt"
	"sword"
)

var (
	ErrInvalidLogin = errors.New("invalid login")
)

type authHandler func(msg *sword.AuthenticationToClient_Content,
	context int32, err error, quit chan error) bool

func (c *Client) postAuthRequest(msg SwordMessage, handler authHandler,
	quit chan error) int32 {
	wrapper := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.AuthenticationToClient == nil ||
			msg.AuthenticationToClient.GetMessage() == nil ||
			msg.Context != context {
			return false
		}
		return handler(msg.AuthenticationToClient.GetMessage(), context, err, quit)
	}
	return c.Post(msg, wrapper)
}

func (c *Client) LoginWithVersion(username, password, version string) error {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: &sword.AuthenticationRequest{
					Login:    proto.String(username),
					Password: proto.String(password),
					Version: &sword.ProtocolVersion{
						Value: proto.String(version),
					},
				},
			},
		},
	}
	handler := func(msg *sword.AuthenticationToClient_Content, context int32,
		err error, quit chan error) bool {

		// we cannot check context as sword always set it to 0
		reply := msg.GetAuthenticationResponse()
		if reply == nil {
			return false
		}
		code := reply.GetErrorCode()
		if code == sword.AuthenticationResponse_success {
			quit <- nil
			return true
		}
		err = ErrInvalidLogin
		name, ok := sword.AuthenticationResponse_ErrorCode_name[int32(code)]
		if ok {
			err = errors.New(name)
		}
		quit <- err
		return true
	}
	quit := make(chan error)
	c.postAuthRequest(msg, handler, quit)
	err := <-quit
	return err
}

func (c *Client) Login(username, password string) error {
	return c.LoginWithVersion(username, password, "5.0")
}

type Profile struct {
	Login      string
	Password   string
	Supervisor bool
}

func (c *Client) ListConnectedProfiles() ([]*Profile, error) {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ConnectedProfilesRequest: &sword.ConnectedProfilesRequest{},
			},
		},
	}
	profiles := []*Profile{}
	handler := func(msg *sword.AuthenticationToClient_Content, context int32,
		err error, quit chan error) bool {

		reply := msg.GetConnectedProfileList()
		if reply == nil {
			return false
		}
		for _, p := range reply.GetElem() {
			profile := Profile{
				Login:      p.GetLogin(),
				Password:   p.GetPassword(),
				Supervisor: p.GetSupervisor(),
			}
			profiles = append(profiles, &profile)
		}
		quit <- err
		return true
	}
	quit := make(chan error)
	c.postAuthRequest(msg, handler, quit)
	err := <-quit
	return profiles, err
}

func (c *Client) CreateProfile(profile *Profile) (string, error) {
	p := &sword.Profile{
		Login:      proto.String(profile.Login),
		Password:   proto.String(profile.Password),
		Supervisor: proto.Bool(profile.Supervisor),
	}

	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ProfileCreationRequest: &sword.ProfileCreationRequest{
					Profile: p,
				},
			},
		},
	}
	login := ""
	handler := func(msg *sword.AuthenticationToClient_Content, context int32,
		err error, quit chan error) bool {

		if reply := msg.GetProfileCreationRequestAck(); reply != nil {
			code := reply.GetErrorCode()
			if code == sword.ProfileCreationRequestAck_success {
				login = reply.GetLogin()
				err = nil
			} else {
				err = errors.New("unknown error")
				name, ok := sword.ProfileCreationRequestAck_ErrorCode_name[int32(code)]
				if ok {
					err = errors.New(name)
				}
			}
		} else {
			err = errors.New(fmt.Sprintf("Got unexpected %v", msg))
		}
		quit <- err
		return true
	}
	quit := make(chan error)
	c.postAuthRequest(msg, handler, quit)
	err := <-quit
	return login, err
}
