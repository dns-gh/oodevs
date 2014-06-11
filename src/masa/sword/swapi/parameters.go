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
	"masa/sword/sword"
	"time"
)

const (
	BoostTimeLayout = "20060102T150405"
)

func MakeId(id uint32) *sword.Id {
	return &sword.Id{Id: proto.Uint32(id)}
}

func MakeIdList(ids ...uint32) *sword.IdList {
	list := make([]*sword.Id, 0, len(ids))
	for _, id := range ids {
		list = append(list, MakeId(id))
	}
	return &sword.IdList{Elem: list}
}

func MakeParameters(args ...interface{}) *sword.MissionParameters {
	reply := &sword.MissionParameters{}
	for _, arg := range args {
		var value *sword.MissionParameter
		if arg == nil {
			value = MakeNullValue()
		} else {
			switch v := arg.(type) {
			case *sword.MissionParameter:
				value = v
			case string:
				value = MakeString(v)
			case float32:
				value = MakeFloat(v)
			}
		}
		reply.Elem = append(reply.Elem, value)
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

func MakeAgent(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Agent: MakeId(value),
		})
}

func MakeAgentKnowledge(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			AgentKnowledge: MakeId(value),
		})
}

func MakeAutomat(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Automat: MakeId(value),
		})
}

func MakeFormation(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Formation: MakeId(value),
		})
}

func MakeObject(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Object: MakeId(value),
		})
}

func MakeKnowledgeGroup(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			KnowledgeGroup: MakeId(value),
		})
}

func MakeParty(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Party: MakeId(value),
		})
}

func MakeQuantity(value int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Quantity: proto.Int32(value),
		})
}

func MakeCoordLatLong(point Point) *sword.CoordLatLong {
	return &sword.CoordLatLong{
		Latitude:  proto.Float64(point.Y),
		Longitude: proto.Float64(point.X),
	}
}

func MakeCoords(points ...Point) *sword.CoordLatLongList {
	coords := []*sword.CoordLatLong{}
	for _, p := range points {
		coords = append(coords, MakeCoordLatLong(p))
	}
	return &sword.CoordLatLongList{
		Elem: coords,
	}
}

func MakeAdhesions(adhesions map[uint32]float32) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range adhesions {
		list = append(list, MakeList(
			MakeIdentifier(k),
			MakeFloat(v),
		))
	}
	return MakeParameter(list...)
}

type Quantity struct {
	Id       uint32
	Quantity int
}

func MakeQuantities(quantities []Quantity) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for _, qty := range quantities {
		list = append(list, MakeList(
			MakeIdentifier(qty.Id),
			MakeQuantity(int32(qty.Quantity)),
		))
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

func ReadPoints(msg *sword.CoordLatLongList) []Point {
	points := []Point{}
	for _, v := range msg.Elem {
		points = append(points, ReadPoint(v))
	}
	return points
}

func MakePointLocation(point Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_point.Enum(),
		Coordinates: MakeCoords(point),
	}
}

func MakeLineLocation(points ...Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_line.Enum(),
		Coordinates: MakeCoords(points...),
	}
}

func MakeRectangleLocation(from, to Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_rectangle.Enum(),
		Coordinates: MakeCoords(from, to),
	}
}

func MakePolygonLocation(points ...Point) *sword.Location {
	return &sword.Location{
		Type:        sword.Location_polygon.Enum(),
		Coordinates: MakeCoords(points...),
	}
}

func MakeRectangleParam(from, to Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Location: MakeRectangleLocation(from, to),
		})
}

func MakeLocation(location *sword.Location) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Location: location,
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

func MakeLinePathParam(points ...Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Path: &sword.Path{
				Location: MakeLineLocation(points...),
			},
		})
}

func MakePolygonParam(points ...Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Area: &sword.Polygon{
				Location: MakePolygonLocation(points...),
			},
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

func MakeLimit(points ...Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Limit: &sword.Line{
				Location: MakeLineLocation(points...),
			},
		})
}

func MakeEnumeration(value int32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			Enumeration: proto.Int32(value),
		})
}

func MakePlannedWork(objectType string, point Point) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			PlannedWork: &sword.PlannedWork{
				Type:     proto.String(objectType),
				Position: MakePointLocation(point),
			},
		})
}

func MakePathfindRequest(unitId uint32, ignoreDynamicObjects bool, points ...Point) *sword.MissionParameter {
	positions := make([]*sword.CoordLatLong, len(points))
	for i, p := range points {
		positions[i] = MakeCoordLatLong(p)
	}
	return MakeParameter(
		&sword.MissionParameter_Value{
			PathfindRequest: &sword.PathfindRequest{
				Unit:                 MakeId(unitId),
				Positions:            positions,
				IgnoreDynamicObjects: proto.Bool(ignoreDynamicObjects),
			},
		})
}

func MakeDateTime(value time.Time) *sword.DateTime {
	return &sword.DateTime{
		Data: proto.String(value.UTC().Format(BoostTimeLayout)),
	}
}

func MakeTime(value time.Time) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			DateTime: MakeDateTime(value),
		})
}

func MakeResourceType(value uint32) *sword.MissionParameter {
	return MakeParameter(
		&sword.MissionParameter_Value{
			ResourceType: MakeId(value),
		})
}

func EnumerateHumans(humans map[int32]int32) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range humans {
		list = append(list, MakeList(
			MakeQuantity(v),
			MakeEnumeration(k),
		))
	}
	return MakeParameter(list...)
}

func MakeHumans(humans []*Human) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for _, v := range humans {
		list = append(list, MakeList(
			MakeQuantity(v.Quantity),
			MakeEnumeration(v.Rank),
			MakeEnumeration(v.State),
			MakeParameter(MakeList(
				MakeParameter(MakeList(
					MakeIdentifier(0), // not used
					MakeEnumeration(v.Injury),
				)),
			)),
			MakeBoolean(v.Psyop),
			MakeBoolean(v.Contaminated),
		))
	}
	return MakeParameter(list...)
}

func MakeResources(resources map[uint32]*Resource) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range resources {
		list = append(list, MakeList(
			MakeIdentifier(k),
			MakeQuantity(v.Quantity),
			MakeFloat(v.LowThreshold),
			MakeFloat(v.HighThreshold),
		))
	}
	return MakeParameter(list...)
}

func MakeEquipment(equipments map[uint32]*Equipment) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range equipments {
		breakdowns := []*sword.MissionParameter{}
		for _, u := range v.Breakdowns {
			breakdowns = append(breakdowns, MakeIdentifier(uint32(u)))
		}
		list = append(list, MakeList(
			MakeIdentifier(k),
			MakeQuantity(v.Available),
			MakeQuantity(v.Unavailable),
			MakeQuantity(v.Repairable),
			MakeQuantity(v.OnSiteFixable),
			MakeQuantity(v.Repairing),
			MakeQuantity(v.Captured),
			MakeParameter(MakeList(breakdowns...)),
		))
	}
	return MakeParameter(list...)
}

func MakeBreakdowns(equipments map[uint32]*Equipment) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range equipments {
		if len(v.Breakdowns) != 0 {
			list = append(list, MakeList(
				MakeIdentifier(k),
				MakeQuantity(v.Repairable),
				MakeIdentifier(uint32(v.Breakdowns[0])),
			))
		}
	}
	return MakeParameter(list...)
}

func GetTime(value string) (time.Time, error) {
	if len(value) == 0 {
		return time.Time{}, ErrInvalidTime
	}
	return time.Parse(BoostTimeLayout, value)
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

func MakeAutomatTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Automat: MakeId(id)}
}

func MakeCrowdTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Crowd: MakeId(id)}
}

func MakeFormationTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Formation: MakeId(id)}
}

func MakePartyTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Party: MakeId(id)}
}

func MakePopulationTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Population: MakeId(id)}
}

func MakeUnitTasker(id uint32) *sword.Tasker {
	return &sword.Tasker{Unit: MakeId(id)}
}

func MakeSupplyFlowResources(supplies map[uint32]uint32) []*sword.SupplyFlowResource {
	resources := []*sword.SupplyFlowResource{}
	for resource, quantity := range supplies {
		resources = append(resources, &sword.SupplyFlowResource{
			ResourceType: MakeId(resource),
			Quantity:     proto.Uint32(quantity),
		})
	}
	return resources
}

func MakeSupplyFlowTransporters(equipments map[uint32]uint32) []*sword.SupplyFlowTransporter {
	transporters := []*sword.SupplyFlowTransporter{}
	for equipment, quantity := range equipments {
		transporters = append(transporters, &sword.SupplyFlowTransporter{
			EquipmentType: MakeId(equipment),
			Quantity:      proto.Uint32(quantity),
		})
	}
	return transporters
}

func MakeStocks(resources map[uint32]*Resource) *sword.MissionParameter {
	list := []*sword.MissionParameter_Value{}
	for k, v := range resources {
		list = append(list, MakeList(
			MakeIdentifier(k),
			MakeQuantity(v.Quantity),
		))
	}
	return MakeParameter(list...)
}
