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

func (c *Client) Login(username, password string) error {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: &sword.AuthenticationRequest{
					Login:    proto.String(username),
					Password: proto.String(password),
					Version:  &sword.ProtocolVersion{Value: proto.String("5.0")},
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
		if msg.AuthenticationToClient == nil {
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
		if !ok {
			err = errors.New(name)
		}
		quit <- err
		return true
	}
	c.Post(msg, handler)
	return <-quit
}
