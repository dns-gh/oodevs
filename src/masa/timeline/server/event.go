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
	gouuid "code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"net/http"
	"net/url"
	"reflect"
	"time"
)

var (
	ErrInvalidTime         = util.NewError(http.StatusBadRequest, "invalid time")
	ErrInvalidUrl          = util.NewError(http.StatusBadRequest, "invalid url")
	ErrInvalidChild        = util.NewError(http.StatusBadRequest, "invalid child")
	ErrInvalidParent       = util.NewError(http.StatusBadRequest, "invalid parent")
	ErrInvalidParentUpdate = util.NewError(http.StatusBadRequest, "invalid parent update")
	ErrParentNotFound      = util.NewError(http.StatusBadRequest, "parent not found")
)

type Action struct {
	url     url.URL
	apply   bool
	payload []byte
}

type Event struct {
	uuid     string
	name     string
	info     string
	begin    time.Time
	end      time.Time
	action   Action
	err      error
	done     bool
	readOnly bool
	parent   *Event
	children map[*Event]struct{}
}

func (e *Event) Proto() *sdk.Event {
	code, text := util.ConvertError(e.err)
	event := &sdk.Event{
		Uuid:      proto.String(e.uuid),
		Name:      proto.String(e.name),
		Info:      proto.String(e.info),
		Begin:     proto.String(util.FormatTime(e.begin)),
		End:       proto.String(""),
		Done:      proto.Bool(e.done),
		ReadOnly:  proto.Bool(e.readOnly),
		ErrorCode: proto.Int32(code),
		ErrorText: proto.String(text),
		Parent:    proto.String(""),
	}
	if !e.end.IsZero() {
		event.End = proto.String(util.FormatTime(e.end))
	}
	if url := e.action.url.String(); len(url) > 0 {
		event.Action = &sdk.Action{
			Target:  proto.String(url),
			Apply:   proto.Bool(e.action.apply),
			Payload: append([]byte{}, e.action.payload...),
		}
	}
	if e.parent != nil {
		event.Parent = proto.String(e.parent.uuid)
	}
	return event
}

func NewEvent(msg *sdk.Event, events EventSlice) (*Event, error) {
	uuid := msg.GetUuid()
	if len(uuid) == 0 {
		uuid = gouuid.New()
	}
	begin, err := util.ParseTime(msg.GetBegin())
	if err != nil {
		return nil, ErrInvalidTime
	}
	reply := &Event{
		uuid:     uuid,
		name:     msg.GetName(),
		info:     msg.GetInfo(),
		begin:    begin,
		done:     msg.GetDone(),
		readOnly: msg.GetReadOnly(),
		children: map[*Event]struct{}{},
	}
	code, text := msg.GetErrorCode(), msg.GetErrorText()
	if code != 0 || len(text) > 0 {
		reply.err = util.NewError(code, text)
	}
	if len(msg.GetEnd()) > 0 {
		end, err := util.ParseTime(msg.GetEnd())
		if err != nil {
			return nil, ErrInvalidTime
		}
		if end.Before(begin) || end.Equal(begin) {
			return nil, ErrInvalidTime
		}
		reply.end = end
	}
	if action := msg.GetAction(); action != nil {
		next, err := url.Parse(action.GetTarget())
		if err != nil {
			return nil, ErrInvalidUrl
		}
		reply.action = Action{
			url:     *next,
			payload: append([]byte{}, action.GetPayload()...),
			apply:   action.GetApply(),
		}
	}
	if len(msg.GetParent()) > 0 {
		if !reply.end.IsZero() {
			return nil, ErrInvalidChild
		}
		reply.parent = events.Find(msg.GetParent())
		if reply.parent == nil {
			return nil, ErrParentNotFound
		}
		if reply.parent.end.IsZero() {
			return nil, ErrInvalidParent
		}
	}
	return reply, nil
}

func (e *Event) OnApply(err error, lock bool) bool {
	done := err == nil
	modified := e.done != done || !reflect.DeepEqual(e.err, err) || e.readOnly != (done && lock)
	e.done = done
	e.err = err
	e.readOnly = done && lock
	return modified
}

func (e *Event) TranslateChildren(offset time.Duration) {
	for child, _ := range e.children {
		child.begin = child.begin.Add(offset)
	}
}

func (e *Event) SetChild(child *Event) bool {
	e.children[child] = struct{}{}
	begin, end := e.begin, e.end
	e.begin = util.MinTime(e.begin, child.begin)
	e.end = util.MaxTime(e.end, child.begin)
	return begin != e.begin || end != e.end
}

func (e *Event) needChildrenUpdate(next *Event) (*time.Duration, error) {
	// if no children, no update
	if len(e.children) == 0 {
		return nil, nil
	}
	// if parent's date have not changed, no update
	beginGap := next.begin.Sub(e.begin)
	endGap := next.end.Sub(e.end)
	if beginGap == 0 && endGap == 0 {
		return nil, nil
	}
	// if parent's date have changed the same way, update children
	duration := e.end.Sub(e.begin)
	nextDuration := next.end.Sub(next.begin)
	if util.AbsDuration(duration-nextDuration) < 100*time.Millisecond { // tolerate 100 ms var
		return &beginGap, nil
	}
	// throw if a child is outside the new parent
	for child, _ := range e.children {
		if child.begin.Before(next.begin) || child.begin.After(next.end) {
			return nil, ErrInvalidParentUpdate
		}
	}
	return nil, nil
}

// Returns 'modified', 'triggered', 'updateChildren', 'error'
// 'modified' indicate whether the event have been modified or not
// 'triggered' indicate whether the event have been triggered or not
// 'updateChildren' indicate whether the children of the event should be update or not
// 'error' is not nil if the update is forbidden
func (e *Event) Update(msg *sdk.Event, tick time.Time, events EventSlice) (bool, bool, bool, error) {
	next, err := NewEvent(msg, events)
	if err != nil {
		return false, false, false, err
	}
	next.children = e.children
	modified := !reflect.DeepEqual(e, next)
	triggered := next.done && !e.done
	var offset *time.Duration
	if modified {
		offset, err = e.needChildrenUpdate(next)
		if err != nil {
			return false, false, false, err
		}
		if offset != nil {
			next.TranslateChildren(*offset)
		}
	}
	if triggered {
		next.begin = tick
	}
	if next.parent != nil {
		delete(next.parent.children, next)
		next.parent.children[e] = struct{}{}
	}
	*e = *next
	return modified, triggered, offset != nil, nil
}
