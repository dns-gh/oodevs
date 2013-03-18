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
	"sword"
)

var (
	ErrInvalidLogin = errors.New("invalid login")
)

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
	quit := make(chan error)
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.AuthenticationToClient == nil || msg.Context != context {
			return false
		}
		// we cannot check context as sword always set it to 0
		reply := msg.AuthenticationToClient.GetMessage().GetAuthenticationResponse()
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
	c.Post(msg, handler)
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
	quit := make(chan error)
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.AuthenticationToClient == nil || msg.Context != context {
			return false
		}
		reply := msg.AuthenticationToClient.GetMessage().GetConnectedProfileList()
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
	c.Post(msg, handler)
	err := <-quit
	return profiles, err
}
