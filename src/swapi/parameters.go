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
