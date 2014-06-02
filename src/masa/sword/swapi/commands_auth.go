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
	"crypto/hmac"
	"crypto/md5"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"masa/sword/sword"
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
	_, err := w.Encode(ClientToAuthenticationTag, msg)
	if err != nil {
		return true
	}
	wait := make(chan bool)
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
	select {
	case <-time.After(timeout):
		return false
	case <-wait:
		break
	}
	return true
}

type authHandler func(msg *sword.AuthenticationToClient_Content, clientId int32) error

func (c *Client) postAuthRequestWithCheckingClientId(msg SwordMessage, handler authHandler, checkClientId bool) <-chan error {
	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, client, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.AuthenticationToClient == nil ||
			msg.AuthenticationToClient.GetMessage() == nil ||
			(checkClientId && (msg.ClientId != client || msg.ClientId == 0)) ||
			msg.Context != context {
			return false
		}

		quit <- handler(msg.AuthenticationToClient.GetMessage(),
			msg.AuthenticationToClient.GetClientId())
		return true
	}
	c.Post(msg, wrapper)
	return quit
}

func (c *Client) postAuthRequest(msg SwordMessage, handler authHandler) <-chan error {
	return c.postAuthRequestWithCheckingClientId(msg, handler, true)
}

func (c *Client) GetAuthenticationKey() (string, error) {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationKeyRequest: &sword.AuthenticationKeyRequest{},
			},
		},
	}
	key := ""
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetAuthenticationKeyResponse()
		if reply == nil {
			return unexpected(msg)
		}
		key = reply.GetAuthenticationKey()
		return nil
	}
	err := <-c.postAuthRequestWithCheckingClientId(msg, handler, false)
	return key, err
}

func (c *Client) LoginWithVersion(username, password, version, key string) error {
	var keyVal *string
	if len(key) != 0 {
		mac := hmac.New(md5.New, []byte("29Ma500SaGroUp75"))
		mac.Write([]byte(key))
		keyVal = proto.String(hex.EncodeToString(mac.Sum(nil)))
	}
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: &sword.AuthenticationRequest{
					Login:    proto.String(username),
					Password: proto.String(password),
					Version: &sword.ProtocolVersion{
						Value: proto.String(version),
					},
					AuthenticationKey: keyVal,
				},
			},
		},
	}
	id := int32(0)
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetAuthenticationResponse()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.AuthenticationResponse_success {
			return makeError(reply, int32(code),
				sword.AuthenticationResponse_ErrorCode_name)
		}
		id = clientId

		if id == 0 {
			return errors.New("invalid nul client identifier")
		}
		c.clientId = id
		return nil
	}
	err := <-c.postAuthRequestWithCheckingClientId(msg, handler, false)
	return err
}

func (c *Client) LoginWithAuthenticationKey(username, password, key string) error {
	return c.LoginWithVersion(username, password, "5.0", key)
}

func (c *Client) Login(username, password string) error {
	return c.LoginWithVersion(username, password, "5.0", "")
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
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetConnectedProfileList()
		if reply == nil {
			return unexpected(msg)
		}
		for _, p := range reply.GetElem() {
			profiles = append(profiles, NewProfile(p))
		}
		return nil
	}
	err := <-c.postAuthRequest(msg, handler)
	return profiles, err
}

func (p *Profile) Proto() *sword.Profile {
	return &sword.Profile{
		Login:       proto.String(p.Login),
		Password:    proto.String(p.Password),
		Supervisor:  proto.Bool(p.Supervisor),
		TimeControl: proto.Bool(p.TimeControl),
	}
}

func (c *Client) CreateProfile(profile *Profile) (*Profile, error) {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				ProfileCreationRequest: &sword.ProfileCreationRequest{
					Profile: profile.Proto(),
				},
			},
		},
	}
	var created *Profile
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetProfileCreationRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileCreationRequestAck_success {
			return makeError(reply, int32(code),
				sword.ProfileCreationRequestAck_ErrorCode_name)
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
					Profile: profile.Proto(),
				},
			},
		},
	}
	var updated *Profile
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetProfileUpdateRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileUpdateRequestAck_success {
			return makeError(reply, int32(code),
				sword.ProfileUpdateRequestAck_ErrorCode_name)
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
	handler := func(msg *sword.AuthenticationToClient_Content, clientId int32) error {
		reply := msg.GetProfileDestructionRequestAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ProfileDestructionRequestAck_success {
			return makeError(reply, int32(code),
				sword.ProfileDestructionRequestAck_ErrorCode_name)
		}
		removed := c.Model.GetProfile(reply.GetLogin())
		if removed != nil {
			return fmt.Errorf(
				"Profile has not been destroyed: %v", removed)
		}
		return nil
	}
	return <-c.postAuthRequest(msg, handler)
}
