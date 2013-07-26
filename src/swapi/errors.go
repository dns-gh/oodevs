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
	"fmt"
)

func unexpected(value interface{}) error {
	return fmt.Errorf("got unexpected value %v", value)
}

// SwordError wraps error returned by the Sword protobuf API. It assumes the
// replies have at least an error code, and an optional error string.
type SwordError struct {
	Code    int32
	Name    string
	Message string
}

func (e *SwordError) Error() string {
	if len(e.Message) > 0 {
		return fmt.Sprintf("%s: %s", e.Name, e.Message)
	}
	return e.Name
}

type ProtoErrorWithMsg interface {
	GetErrorMsg() string
}

// Converts a protobuf response, its converted error code and an error mapping
// to a SwordError if possible, or to a regular error otherwise.
func makeError(err interface{}, code int32, names map[int32]string) error {
	name, ok := names[code]
	if !ok {
		return unexpected(code)
	}
	msg := ""
	if errMsg, ok := err.(ProtoErrorWithMsg); ok {
		msg = errMsg.GetErrorMsg()
	}
	return &SwordError{
		Code:    code,
		Name:    name,
		Message: msg,
	}
}
