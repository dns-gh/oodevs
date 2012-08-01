package main

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

func set_default(h map[string]interface{}, key, def string) interface{} {
	val, ok := h[key]
	if ok {
		return val
	}
	h[key] = def
	return def
}

func sub(args ...interface{}) map[string]interface{} {
	rpy := make(map[string]interface{})
	var key string
	for _, x := range args {
		if len(key) == 0 {
			switch x := x.(type) {
			case string:
				key = x
			}
		} else {
			rpy[key] = x
			key = ""
		}
	}
	return rpy
}

func reset(h map[string]interface{}, key string) interface{} {
	delete(h, key)
	return h
}

func concat(args ...string) (rpy string) {
	for _, x := range args {
		rpy += x
	}
	return rpy
}

func GetHelpers() template.FuncMap {
	return template.FuncMap{
		"concat":      concat,
		"eq":          eq,
		"ne":          ne,
		"reset":       reset,
		"set_default": set_default,
		"sub":         sub,
	}
}
