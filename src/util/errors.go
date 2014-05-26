// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package util

import (
	"net/http"
	"strings"
)

type Error struct {
	Code int32
	Text string
}

func (e *Error) Error() string {
	return e.Text
}

func NewErrorCode(code int32) error {
	return &Error{Code: code}
}

func NewError(code int32, text ...string) error {
	return &Error{
		Code: code,
		Text: strings.Join(text, " "),
	}
}

func ConvertError(err error) (int32, string) {
	if err == nil {
		return 0, ""
	}
	code := int32(http.StatusInternalServerError)
	text := err.Error()
	if custom, ok := err.(*Error); ok {
		code = custom.Code
		text = custom.Text
	}
	return code, text
}
