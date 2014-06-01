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
	"sdk"
	"sort"
)

// EventsSlice maintains a sequence of *Event sorted by ascending dates.
type EventSlice []*Event

func (e *Event) Less(other *Event) bool {
	diff := e.begin.Sub(other.begin)
	if diff == 0 {
		return e.uuid < other.uuid
	}
	return diff < 0
}

func (e EventSlice) Len() int {
	return len(e)
}

func (e EventSlice) Less(i, j int) bool {
	return e[i].Less(e[j])
}

func (e EventSlice) Swap(i, j int) {
	e[i], e[j] = e[j], e[i]
}

func (e *EventSlice) Append(event *Event) {
	slice := *e
	idx := sort.Search(len(slice), func(i int) bool {
		return !slice[i].Less(event)
	})
	slice = append(slice, nil)
	copy(slice[idx+1:], slice[idx:])
	slice[idx] = event
	*e = slice
}

func (e *EventSlice) Remove(uuid string) bool {
	slice := *e
	for i, event := range slice {
		if event.uuid == uuid {
			if event.parent != nil {
				delete(event.parent.children, event)
			}
			for child := range event.children {
				child.parent = nil
			}
			*e = append(slice[:i], slice[i+1:]...)
			return true
		}
	}
	return false
}

func (e EventSlice) Find(uuid string) *Event {
	for _, event := range e {
		if event.uuid == uuid {
			return event
		}
	}
	return nil
}

func (e EventSlice) Proto() []*sdk.Event {
	if len(e) == 0 {
		return nil
	}
	rpy := make([]*sdk.Event, 0, len(e))
	for _, event := range e {
		rpy = append(rpy, event.Proto())
	}
	return rpy
}
