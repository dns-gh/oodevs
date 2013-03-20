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
	"io"
	"sword"
	"time"
)

// Send a DisconnectionRequest over link and wait for connection termination.
// Return true upon disconnection, false on timeout.
func Disconnect(link io.ReadWriter, timeout time.Duration) bool {
	w := NewWriter(link)
	msg := &sword.ClientToAuthentication{
		Message: &sword.ClientToAuthentication_Content{
			DisconnectionRequest: &sword.DisconnectionRequest{},
		},
	}
	err := w.Encode(ClientToAuthenticationTag, msg)
	if err != nil {
		return true
	}
	wait := make(chan bool)
	go func() {
		time.Sleep(timeout)
		wait <- false
	}()
	go func() {
		buffer := make([]uint8, 128)
		for {
			_, err := link.Read(buffer)
			if err != nil {
				break
			}
		}
		wait <- true
	}()
	return <-wait
}

func unexpected(value interface{}) error {
	return errors.New(fmt.Sprintf("got unexpected value %v", value))
}

func nameof(names map[int32]string, code int32) error {
	name, ok := names[code]
	if !ok {
		return unexpected(code)
	}
	return errors.New(name)
}

type authHandler func(msg *sword.AuthenticationToClient_Content) error

func (c *Client) postAuthRequest(msg SwordMessage, handler authHandler) <-chan error {
	quit := make(chan error, 1)
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
		quit <- handler(msg.AuthenticationToClient.GetMessage())
		return true
	}
	c.Post(msg, wrapper)
	return quit
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
	handler := func(msg *sword.AuthenticationToClient_Content) error {
		reply := msg.GetAuthenticationResponse()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.AuthenticationResponse_success {
			return nameof(sword.AuthenticationResponse_ErrorCode_name,
				int32(code))
		}
		return nil
	}
	return <-c.postAuthRequest(msg, handler)
}

func (c *Client) Login(username, password string) error {
	return c.LoginWithVersion(username, password, "5.0")
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
	handler := func(msg *sword.AuthenticationToClient_Content) error {
		reply := msg.GetConnectedProfileList()
		if reply == nil {
			return unexpected(msg)
		}
		for _, p := range reply.GetElem() {
			profile := NewProfile(
				p.GetLogin(),
				p.GetPassword(),
				p.GetSupervisor())
			profiles = append(profiles, profile)
		}
		return nil
	}
	err := <-c.postAuthRequest(msg, handler)
	return profiles, err
}

func profileToProto(profile *Profile) *sword.Profile {
	p := &sword.Profile{
		Login:      proto.String(profile.Login),
		Password:   proto.String(profile.Password),
		Supervisor: proto.Bool(profile.Supervisor),
	}
	return p
}

func (c *Client) CreateProfile(profile *Profile) (*Profile, error) {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ProfileCreationRequest: &sword.ProfileCreationRequest{
					Profile: profileToProto(profile),
				},
			},
		},
	}
	var created *Profile
	handler := func(msg *sword.AuthenticationToClient_Content) error {
		reply := msg.GetProfileCreationRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileCreationRequestAck_success {
			return nameof(sword.ProfileCreationRequestAck_ErrorCode_name,
				int32(code))
		}
		// The profile should be available by now. It is safe to call
		// GetProfile() here:
		// - The model has its own goroutine, so no reentrancy issue
		// - This handler block the others so there we are not racing
		//   for profile creation.
		// - GetProfile() posts on the model goroutine so all pending
		//   pending messages will be processed before we try to
		//   access profiles.
		created = c.Model.GetProfile(reply.GetLogin())
		return nil
	}
	err := <-c.postAuthRequest(msg, handler)
	return created, err
}

func (c *Client) UpdateProfile(login string, profile *Profile) (*Profile, error) {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ProfileUpdateRequest: &sword.ProfileUpdateRequest{
					Login:   proto.String(login),
					Profile: profileToProto(profile),
				},
			},
		},
	}
	var updated *Profile
	handler := func(msg *sword.AuthenticationToClient_Content) error {
		reply := msg.GetProfileUpdateRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileUpdateRequestAck_success {
			return nameof(sword.ProfileUpdateRequestAck_ErrorCode_name,
				int32(code))
		}
		updated = c.Model.GetProfile(reply.GetLogin())
		return nil
	}
	err := <-c.postAuthRequest(msg, handler)
	return updated, err
}

func (c *Client) DeleteProfile(login string) error {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ProfileDestructionRequest: &sword.ProfileDestructionRequest{
					Login: proto.String(login),
				},
			},
		},
	}
	handler := func(msg *sword.AuthenticationToClient_Content) error {
		reply := msg.GetProfileDestructionRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileDestructionRequestAck_success {
			return nameof(sword.ProfileDestructionRequestAck_ErrorCode_name,
				int32(code))
		}
		removed := c.Model.GetProfile(reply.GetLogin())
		if removed != nil {
			return errors.New(fmt.Sprintf(
				"Profile has not been destroyed: %v", removed))
		}
		return nil
	}
	return <-c.postAuthRequest(msg, handler)
}
