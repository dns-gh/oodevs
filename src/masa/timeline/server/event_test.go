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
	. "launchpad.net/gocheck"
	"time"
	"util"
)

func checkEmptyEndDate(c *C, date *string) {
	begin := time.Now()
	end := time.Now().Add(1 * time.Hour)
	event := Event{
		uuid:  "a",
		begin: begin,
		end:   end,
	}
	next := event.Proto()
	next.End = nil
	if date != nil {
		next.End = proto.String(*date)
	}
	modified, triggered, updateChildren, err := event.Update(next, end, EventSlice{})
	c.Assert(err, IsNil)
	c.Assert(modified, Equals, true)
	c.Assert(triggered, Equals, false)
	c.Assert(updateChildren, Equals, false)
	c.Assert(event.end.IsZero(), Equals, true)
	c.Assert(event.Proto().GetEnd(), Equals, "")
}

func (TestSuite) TestEventEmptyEndUpdate(c *C) {
	end := ""
	checkEmptyEndDate(c, &end)
}

func (TestSuite) TestEventNilEndUpdate(c *C) {
	checkEmptyEndDate(c, nil)
}

func (TestSuite) TestEventCreation(c *C) {
	begin := time.Now().UTC()
	end := time.Now().Add(1 * time.Hour).UTC()
	parent := &Event{
		uuid:     "a",
		begin:    begin,
		end:      end,
		children: map[*Event]struct{}{},
	}
	child := &Event{
		uuid:     "b",
		begin:    begin,
		parent:   parent,
		children: map[*Event]struct{}{},
	}
	slice := EventSlice{parent}

	// valid parent
	childProto := child.Proto()
	event, err := NewEvent(childProto, slice)
	c.Assert(err, IsNil)
	c.Assert(event, DeepEquals, child)

	// valid parent, need update
	parent.children = map[*Event]struct{}{}
	child.begin = time.Now().Add(2 * time.Hour).UTC()
	childProto = child.Proto()
	event, err = NewEvent(childProto, slice)
	c.Assert(err, IsNil)
	c.Assert(event, DeepEquals, child)

	// invalid child, durable child
	child.end = time.Now().Add(4 * time.Hour).UTC()
	childProto = child.Proto()
	event, err = NewEvent(childProto, slice)
	c.Assert(err, DeepEquals, ErrInvalidChild)
	c.Assert(event, IsNil)

	// invalid parent, ephemeral parent
	child.end = time.Time{}
	parent.end = time.Time{}
	childProto = child.Proto()
	event, err = NewEvent(childProto, slice)
	c.Assert(err, DeepEquals, ErrInvalidParent)
	c.Assert(event, IsNil)

	// invalid parent, parent not found
	slice.Remove(parent.uuid)
	event, err = NewEvent(childProto, slice)
	c.Assert(err, DeepEquals, ErrParentNotFound)
	c.Assert(event, IsNil)
}

func (TestSuite) TestEventUpdate(c *C) {
	begin := time.Now().UTC()
	end := time.Now().Add(1 * time.Hour).UTC()
	child := &Event{
		uuid:     "b",
		begin:    begin,
		children: map[*Event]struct{}{},
	}
	parent := &Event{
		uuid:     "a",
		begin:    begin,
		end:      end,
		children: map[*Event]struct{}{},
	}
	slice := EventSlice{parent}

	// valid child
	childProto := child.Proto()
	childProto.Parent = proto.String(parent.uuid)
	childProto.Begin = proto.String(util.FormatTime(begin.Add(2 * time.Hour).UTC()))

	modified, triggered, updateChildren, err := child.Update(childProto, end, slice)
	c.Assert(err, IsNil)
	c.Assert(modified, Equals, true)
	c.Assert(triggered, Equals, false)
	c.Assert(updateChildren, Equals, false)
	c.Assert(child.parent, Equals, parent)

	// valid parent
	parentProto := parent.Proto()
	parentProto.Begin = proto.String(util.FormatTime(begin.Add(2 * time.Minute)))
	parentProto.End = proto.String(util.FormatTime(end.Add(2 * time.Minute)))

	modified, triggered, updateChildren, err = parent.Update(parentProto, end, slice)
	c.Assert(err, IsNil)
	c.Assert(modified, Equals, true)
	c.Assert(triggered, Equals, false)
	c.Assert(updateChildren, Equals, true)
	child.begin = child.begin.Add(2 * time.Minute)
	c.Assert(parent.children, DeepEquals, map[*Event]struct{}{child: struct{}{}})

	// invalid parent update
	child.begin = begin
	parent.begin = begin
	parent.end = end
	parentProto = parent.Proto()
	copyParent := parent
	copyChild := child
	parentProto.Begin = proto.String(util.FormatTime(begin.Add(2 * time.Minute)))
	modified, triggered, updateChildren, err = parent.Update(parentProto, end, slice)
	c.Assert(err, DeepEquals, ErrInvalidParentUpdate)
	c.Assert(modified, Equals, false)
	c.Assert(triggered, Equals, false)
	c.Assert(updateChildren, Equals, false)
	c.Assert(parent, DeepEquals, copyParent)
	c.Assert(child, DeepEquals, copyChild)
}
