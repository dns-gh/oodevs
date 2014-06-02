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
	. "launchpad.net/gocheck"
	"sort"
	"time"
)

var (
	sometime = time.Now()
	events   = EventSlice{
		&Event{uuid: "a", begin: sometime.Add(0 * time.Hour)},
		&Event{uuid: "b", begin: sometime.Add(1 * time.Hour)},
		&Event{uuid: "c", begin: sometime.Add(1 * time.Hour)},
		&Event{uuid: "d", begin: sometime.Add(2 * time.Hour)},
	}
)

func (TestSuite) TestEventSlice(c *C) {
	for i := 1; i < len(events); i++ {
		c.Assert(events[i-1].Less(events[i]), Equals, true)
		c.Assert(events[i].Less(events[i-1]), Equals, false)
	}
	c.Assert(sort.IsSorted(events), Equals, true)
	slice := EventSlice{}
	for i := range events {
		slice.Append(events[len(events)-i-1])
	}
	c.Assert(slice, HasLen, len(events))
	c.Assert(sort.IsSorted(slice), Equals, true)
	slice.Remove("d")
	c.Assert(slice, DeepEquals, events[0:3])
	slice.Remove("b")
	c.Assert(sort.IsSorted(slice), Equals, true)
	c.Assert(slice, HasLen, 2)
	slice.Remove("a")
	c.Assert(slice, HasLen, 1)
	slice.Remove("a")
	c.Assert(slice, HasLen, 1)
	slice.Remove("c")
	c.Assert(slice, HasLen, 0)
}
