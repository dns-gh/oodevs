// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package server

import (
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
)

func Dump(msg proto.Message) string {
	data, err := json.Marshal(msg)
	if err != nil {
		return "unable to convert message to json " + err.Error()
	}
	bytes, err := FixJsonPayloads(data, Decode)
	if err != nil {
		return string(data)
	}
	return string(bytes)
}
