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
	"masa/sword/sword"
)

var (
	ErrContinue = errors.New("continue")
	ErrNotFound = errors.New("not found")
)

func mismatch(name string, a, b interface{}) error {
	return fmt.Errorf("%v mismatch %v != %v", name, a, b)
}

func invalid(name string, value interface{}) error {
	return fmt.Errorf("invalid %v: %v", name, value)
}

func makeLogisticHistoryRequest(requestIds ...uint32) *sword.LogisticHistoryRequest {
	ids := MakeIdList(requestIds...).Elem
	return &sword.LogisticHistoryRequest{
		Requests: ids,
	}
}

func handleLogisticHistoryAck(ack *sword.LogisticHistoryAck) []*sword.LogHistoryEntry {
	entries := []*sword.LogHistoryEntry{}
	DeepCopy(&entries, ack.GetEntries())
	return entries
}

func makeListLogisticRequests(currentTick, maxCount int,
	entities ...uint32) *sword.ListLogisticRequests {

	ids := MakeIdList(entities...).Elem
	var count *uint32
	if maxCount >= 0 {
		count = proto.Uint32(uint32(maxCount))
	}
	return &sword.ListLogisticRequests{
		Entities:    ids,
		MaxCount:    count,
		CurrentTick: proto.Int32(int32(currentTick)),
	}
}

func handleListLogisticRequestsAck(ack *sword.ListLogisticRequestsAck) []*sword.LogHistoryEntry {
	entries := []*sword.LogHistoryEntry{}
	DeepCopy(&entries, ack.GetEntries())
	return entries
}
