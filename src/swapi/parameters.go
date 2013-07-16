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
	"time"
)

const (
	BoostTimeLayout = "20060102T150405"
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

func MakeList(args ...*sword.MissionParameter) *sword.MissionParameter_Value {
	replyValue := &sword.MissionParameter_Value{}
	for _, arg := range args {
		replyValue.List = append(replyValue.List, GetFirstValue(arg))
	}
	return replyValue
}

func MakeEmpty() *sword.MissionParameter {
	return MakeParameter()
}

func MakeInt(value int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			IntValue: proto.Int32(value),
		})
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

func MakeBoolean(value bool) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			BooleanValue: proto.Bool(value),
		})
}

func MakeIdentifier(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Identifier: proto.Uint32(value),
		})
}

func MakeAutomat(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Automat: &sword.AutomatId{
				Id: proto.Uint32(value),
			},
		})
}

func MakeQuantity(value int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Quantity: proto.Int32(value),
		})
}

func MakeCoords(points ...Point) *sword.CoordLatLongList {
	coords := []*sword.CoordLatLong{}
	for _, p := range points {
		coords = append(coords, &sword.CoordLatLong{
			Latitude:  proto.Float64(p.Y),
			Longitude: proto.Float64(p.X),
		})
	}
	return &sword.CoordLatLongList{
		Elem: coords,
	}
}

func MakeAdhesions(adhesions map[uint32]float32) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range adhesions {
		party := []*sword.MissionParameter_Value{
			{Identifier: proto.Uint32(k)},
			{AReal: proto.Float32(v)},
		}
		list = append(list, &sword.MissionParameter_Value{
			List: party,
		})
	}
	return MakeParameter(list...)
}

func MakeExtensions(extensions *map[string]string) *sword.MissionParameter {
	list := []*sword.Extension_Entry{}
	for k, v := range *extensions {
		extension := &sword.Extension_Entry{
			Name:  proto.String(k),
			Value: proto.String(v),
		}
		list = append(list, extension)
	}
	value := &sword.Extension{
		Entries: list,
	}
	return MakeParameter(&sword.MissionParameter_Value{ExtensionList: value})
}

func ReadPoint(value *sword.CoordLatLong) Point {
	if value == nil {
		return Point{}
	}
	return Point{X: value.GetLongitude(), Y: value.GetLatitude()}
}

func MakePointLocation(point Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_point.Enum(),
		Coordinates: MakeCoords(point),
	}
}

func MakeLineLocation(from, to Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_line.Enum(),
		Coordinates: MakeCoords(from, to),
	}
}

func MakeRectangleLocation(from, to Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_rectangle.Enum(),
		Coordinates: MakeCoords(from, to),
	}
}

func MakeRectangleParam(from, to Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Location: MakeRectangleLocation(from, to),
		})
}

func MakePointParam(point Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Point: &sword.Point{
				Location: MakePointLocation(point),
			},
		})
}

func MakeLocationParam(point Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Location: MakePointLocation(point),
		})
}

func MakeHeading(heading int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Heading: &sword.Heading{
				Heading: proto.Int32(heading),
			},
		})
}

func MakeNullValue() *sword.MissionParameter {
	return &sword.MissionParameter{
		NullValue: proto.Bool(true),
	}
}

func MakeLimit(from, to Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Limit: &sword.Line{
				Location: MakeLineLocation(from, to),
			},
		})
}

func MakeEnumeration(value int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Enumeration: proto.Int32(value),
		})
}

func MakeTime(value time.Time) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			DateTime: &sword.DateTime{
				Data: proto.String(value.UTC().Format(BoostTimeLayout)),
			},
		})
}

func MakeResourceType(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			ResourceType: &sword.ResourceType{
				Id: proto.Uint32(value),
			},
		})
}

func GetTime(value *sword.DateTime) (time.Time, error) {
	if value == nil {
		return time.Time{}, ErrInvalidTime
	}
	return time.Parse(BoostTimeLayout, value.GetData())
}

func GetFirstValue(param *sword.MissionParameter) *sword.MissionParameter_Value {
	values := param.GetValue()
	if values != nil && len(values) > 0 {
		return values[0]
	}
	return nil
}

// Return the first value of the index-th parameter of params, or nil.
func GetParameterValue(params *sword.MissionParameters,
	index int) *sword.MissionParameter_Value {

	elems := params.GetElem()
	if elems != nil && len(elems) > index {
		return GetFirstValue(elems[index])
	}
	return nil
}
