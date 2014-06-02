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
	"time"
)

func ParseTime(value string) (time.Time, error) {
	reply, err := time.Parse(time.RFC3339Nano, value)
	if err == nil {
		return reply, nil
	}
	return time.Parse(time.RFC3339, value)
}

func FormatTime(value time.Time) string {
	return value.UTC().Format(time.RFC3339Nano)
}

func MinTime(left, right time.Time) time.Time {
	if left.Before(right) {
		return left
	}
	return right
}

func MaxTime(left, right time.Time) time.Time {
	if left.After(right) {
		return left
	}
	return right
}

func AbsDuration(value time.Duration) time.Duration {
	if value < 0 {
		return -value
	}
	return value
}
