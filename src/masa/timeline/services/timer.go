// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	"code.google.com/p/goprotobuf/proto"
	"errors"
	"masa/timeline/sdk"
	"net/url"
	"time"
)

var (
	TimePeriod      = 400 * time.Millisecond
	ErrInvalidApply = errors.New("invalid apply")
)

type Timer struct {
	root   Observer
	base   time.Time
	offset time.Duration
	quit   chan struct{}
	ticker *time.Ticker
}

func NewTimer(root Observer, base time.Time) *Timer {
	return &Timer{
		root:   root,
		base:   base,
		offset: base.Sub(time.Now()),
		quit:   make(chan struct{}),
	}
}

func (t *Timer) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(true),
		Timer: &sdk.Timer{
			Base: proto.String(t.base.Format(time.RFC3339Nano)),
		},
	}
}

func (*Timer) AttachClock() {
	// NOTHING
}

func (*Timer) HasClock() bool {
	return true
}

func (*Timer) IsLocked() bool {
	return false
}

func (t *Timer) Start() error {
	t.ticker = time.NewTicker(TimePeriod)
	go func() {
		for {
			select {
			case <-t.quit:
				return
			case now := <-t.ticker.C:
				t.root.Tick(now.Add(t.offset))
			}
		}
	}()
	return nil
}

func (t *Timer) Stop() error {
	if t.ticker == nil {
		return nil
	}
	t.quit <- struct{}{}
	t.ticker = nil
	return nil
}

func (t *Timer) Apply(string, url.URL, []byte) {
	t.root.Abort(ErrInvalidApply)
}
