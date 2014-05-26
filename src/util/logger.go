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
	"launchpad.net/gocheck"
	"log"
	"strings"
	"sync"
	"time"
)

type Logger interface {
	Printf(format string, a ...interface{})
}

type TestLogger struct {
	mutex sync.Mutex
	c     *gocheck.C
}

func (t *TestLogger) Printf(format string, a ...interface{}) {
	fmt := time.Now().Format(time.Stamp) + ": " + strings.TrimSpace(format)
	if false {
		t.mutex.Lock()
		defer t.mutex.Unlock()
		t.c.Logf(fmt, a...)
	} else {
		log.Printf(fmt, a...)
	}
}

func MakeGocheckLogger(c *gocheck.C) Logger {
	return &TestLogger{c: c}
}
