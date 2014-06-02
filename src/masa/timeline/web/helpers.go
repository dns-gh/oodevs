// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package web

import (
	"reflect"
	"text/template"
)

func eq(args ...interface{}) bool {
	if len(args) == 0 {
		return false
	}
	x := args[0]
	switch x := x.(type) {
	case string, int, int64, byte, float32, float64:
		for _, y := range args[1:] {
			if x == y {
				return true
			}
		}
		return false
	}
	for _, y := range args[1:] {
		if reflect.DeepEqual(x, y) {
			return true
		}
	}
	return false
}

func ne(args ...interface{}) bool {
	return !eq(args...)
}

func mk(args ...interface{}) map[string]interface{} {
	data := make(map[string]interface{})
	key := ""
	for _, x := range args {
		if len(key) == 0 {
			switch x := x.(type) {
			case string:
				key = x
			}
		} else {
			data[key] = x
			key = ""
		}
	}
	return data
}

func GetHelpers() template.FuncMap {
	return template.FuncMap{
		"eq": eq,
		"ne": ne,
		"mk": mk,
	}
}
