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
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"sdk"
	"server"
)

type Commands []sdk.ClientRequest

func encodecommands(data []byte) ([]byte, error) {
	var raw interface{}
	err := json.Unmarshal(data, &raw)
	if err != nil {
		return nil, err
	}
	list, ok := raw.([]interface{})
	if !ok {
		return nil, server.ErrInvalidType
	}
	for _, arg := range list {
		server.FixRawPayloads(arg, server.Encode)
	}
	return json.Marshal(raw)
}

func ParseFile(name string) (Commands, error) {
	file, err := os.Open(name)
	if err != nil {
		return nil, err
	}
	data, err := ioutil.ReadAll(file)
	file.Close()
	if err != nil {
		return nil, err
	}
	data, err = encodecommands(data)
	if err != nil {
		return nil, err
	}
	return ParseReader(bytes.NewBuffer(data))
}

func ParseReader(reader io.Reader) (Commands, error) {
	decoder := json.NewDecoder(reader)
	data := Commands{}
	err := decoder.Decode(&data)
	if err != nil {
		return nil, err
	}
	return data, nil
}

func ParseCommands(args []string) (Commands, error) {
	cmds := Commands{}
	for _, arg := range args {
		data, err := ParseFile(arg)
		if err != nil {
			return nil, errors.New(fmt.Sprintf("unable to parse file %v: %v", arg, err))
		}
		cmds = append(cmds, data...)
	}
	return cmds, nil
}
