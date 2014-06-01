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
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"encoding/base64"
	"encoding/json"
	"errors"
	"io"
	"net/url"
	"sdk"
	"strings"
	"sword"
)

/*
   The goal of payload.go is to transcribe binary payloads from event actions
   to a text version.
   For example, a sword action consists of a protobuf binary payload. The text
   version is the JSON version of the same data.
   Currently, only one filter is implemented, for sword.
*/

var (
	ErrInvalidType = errors.New("invalid type")
)

type CodeType int32

const (
	Encode CodeType = iota + 1
	Decode
)

type Coder func(interface{}) (interface{}, error)

type Filter interface {
	Decode(interface{}) (interface{}, error)
	Encode(interface{}) (interface{}, error)
}

type FilterFactory struct {
	scheme  string
	factory func() Filter
}

var (
	payloadFilters = []FilterFactory{
		{"sword", func() Filter { return NewSwordFilter() }},
	}
	payloadNodes = [][]string{
		{"event", "create", "event", "action"},
		{"action"},
	}
)

func fixaction(node map[string]interface{}, filters map[string]Coder) error {
	rawurl, ok := node["target"].(string)
	if !ok {
		return nil
	}
	url, err := url.Parse(rawurl)
	if err != nil {
		return nil
	}
	filter, ok := filters[url.Scheme]
	if !ok {
		return nil
	}
	value, err := filter(node["payload"])
	if err != nil {
		return err
	}
	node["payload"] = value
	return nil
}

func fixarray(value interface{}, filters map[string]Coder, tokens []string) {
	next, ok := value.([]interface{})
	if !ok {
		return
	}
	for _, arg := range next {
		fixnodes(arg, filters, tokens)
	}
}

func fixnodes(value interface{}, filters map[string]Coder, tokens []string) {
	var token string
	for {
		node, ok := value.(map[string]interface{})
		if !ok {
			return
		}
		if len(tokens) == 0 {
			fixaction(node, filters)
			return
		}
		token, tokens = tokens[0], tokens[1:]
		next := strings.TrimLeft(token, "@")
		if token != next {
			fixarray(node[next], filters, tokens)
			return
		}
		value = node[token]
	}
}

func FixRawPayloads(value interface{}, codetype CodeType) {
	filters := map[string]Coder{}
	for _, it := range payloadFilters {
		filter := it.factory()
		coder := filter.Decode
		if codetype != Decode {
			coder = filter.Encode
		}
		filters[it.scheme] = coder
	}
	for _, arg := range payloadNodes {
		fixnodes(value, filters, arg)
	}
}

func jsondecode(src []byte, dst interface{}) error {
	d := json.NewDecoder(bytes.NewReader(src))
	d.UseNumber()
	return d.Decode(dst)
}

func FixJsonPayloads(value []byte, codetype CodeType) ([]byte, error) {
	var raw interface{}
	err := jsondecode(value, &raw)
	if err != nil {
		return []byte{}, err
	}
	FixRawPayloads(raw, codetype)
	data, err := json.Marshal(raw)
	if err != nil {
		return []byte{}, err
	}
	return data, nil
}

type SwordFilter struct {
	buffer bytes.Buffer
	pbuf   proto.Buffer
	jenc   *json.Encoder
	jdec   *json.Decoder
}

func NewSwordFilter() *SwordFilter {
	f := &SwordFilter{}
	f.jenc = json.NewEncoder(&f.buffer)
	f.jdec = json.NewDecoder(&f.buffer)
	f.jdec.UseNumber()
	return f
}

/*
   Returns an untyped map of key/value transcribed from a text binary payload
   Steps are:
   * Decode the base64 encoded binary payload into data
   * Unmarshal this data into a sword.ClientToSim object
   * Encode this object into a JSON string
   * Decode this json string into an untyped map of key/value
*/
func (f *SwordFilter) Decode(value interface{}) (interface{}, error) {
	data, ok := value.(string)
	if !ok {
		return nil, ErrInvalidType
	}
	f.buffer.Reset()
	_, err := io.Copy(&f.buffer, base64.NewDecoder(base64.StdEncoding, strings.NewReader(data)))
	if err != nil {
		return nil, err
	}
	msg := sword.ClientToSim{}
	f.pbuf.SetBuf(f.buffer.Bytes())
	f.pbuf.Unmarshal(&msg)
	if err != nil {
		return nil, err
	}
	f.buffer.Reset()
	err = f.jenc.Encode(&msg)
	if err != nil {
		return nil, err
	}
	var raw interface{}
	err = f.jdec.Decode(&raw)
	if err != nil {
		return nil, err
	}
	return raw, nil
}

/*
   Returns a binary payload from an untyped map of key/value
   Steps are:
   * Encode the map into a JSON string
   * Decode this JSON string into a sword.ClientToSim object
   * Encode this object into a protobuf binary payload
   Base-64 encoding is uneeded because it is done automatically when
   serialiazing a binary buffer
*/
func (f *SwordFilter) Encode(value interface{}) (interface{}, error) {
	f.buffer.Reset()
	err := f.jenc.Encode(value)
	if err != nil {
		return nil, err
	}
	msg := sword.ClientToSim{}
	err = f.jdec.Decode(&msg)
	if err != nil {
		return nil, err
	}
	return proto.Marshal(&msg)
}

func (f *SwordFilter) fixAction(raw map[string]interface{}, coder Coder) error {
	action, ok := raw["action"].(map[string]interface{})
	if !ok {
		return nil
	}
	return fixaction(action, map[string]Coder{
		"sword": coder,
	})
}

/*
   Returns an untyped map of key/value from an *sdk.Event, with its action
   payload encoded in JSON format
*/
func (f *SwordFilter) DecodeEvent(event interface{}) (interface{}, error) {
	f.buffer.Reset()
	err := f.jenc.Encode(event)
	if err != nil {
		return nil, err
	}
	var raw map[string]interface{}
	err = f.jdec.Decode(&raw)
	if err != nil {
		return nil, err
	}
	err = f.fixAction(raw, f.Decode)
	if err != nil {
		return nil, err
	}
	return raw, nil
}

/*
   Returns an *sdk.Event from an untyped map of key/value, with its action
   payload encoded in binary format
*/
func (f *SwordFilter) EncodeEvent(raw map[string]interface{}) (*sdk.Event, error) {
	err := f.fixAction(raw, f.Encode)
	if err != nil {
		return nil, err
	}
	f.buffer.Reset()
	err = f.jenc.Encode(raw)
	if err != nil {
		return nil, err
	}
	event := &sdk.Event{}
	err = f.jdec.Decode(event)
	if err != nil {
		return nil, err
	}
	return event, nil
}

func (f *SwordFilter) EncodeEvents(src []byte) ([]*sdk.Event, error) {
	_, err := f.buffer.Write(src)
	raw := []map[string]interface{}{}
	err = f.jdec.Decode(&raw)
	if err != nil {
		return nil, err
	}
	reply := []*sdk.Event{}
	for _, event := range raw {
		next, err := f.EncodeEvent(event)
		if err != nil {
			return nil, err
		}
		reply = append(reply, next)
	}
	return reply, nil
}
