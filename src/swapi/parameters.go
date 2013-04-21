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
	"sword"
)

func MakeParameters(args ...*sword.MissionParameter) *sword.MissionParameters {
	reply := &sword.MissionParameters{}
	for _, arg := range args {
		reply.Elem = append(reply.Elem, arg)
	}
	return reply
}

func MakeParameter(args ...*sword.MissionParameter_Value) *sword.MissionParameter {
	reply := &sword.MissionParameter{
		NullValue: proto.Bool(false),
	}
	for _, arg := range args {
		reply.Value = append(reply.Value, arg)
	}
	return reply
}

func MakeEmpty() *sword.MissionParameter {
	return MakeParameter()
}

func MakeFloat(value float32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			AReal: proto.Float32(value),
		})
}

func MakeString(value string) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			ACharStr: proto.String(value),
		})
}

func MakeIdentifier(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Identifier: proto.Uint32(value),
		})
}

// Return the first value of the index-th parameter of params, or nil.
func GetParameterValue(params *sword.MissionParameters,
	index int) *sword.MissionParameter_Value {

	elems := params.GetElem()
	if elems != nil && len(elems) > index {
		values := elems[index].GetValue()
		if values != nil && len(values) > 0 {
			return values[0]
		}
	}
	return nil
}
