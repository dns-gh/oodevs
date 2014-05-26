// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package server

import (
	"sdk"
	"services"
)

type ShowOnlyFilter struct{}

func (s ShowOnlyFilter) GetFilters(config services.EventFilterConfig) []services.EventFilter {
	filters := []services.EventFilter{}
	uuid, ok := config["filter_show_only"].(string)
	if !ok {
		return filters
	}
	return append(filters, func(event *sdk.Event) bool {
		return event.GetUuid() != uuid && event.GetParent() != uuid
	})
}

func (s ShowOnlyFilter) Filter(event *sdk.Event, config services.EventFilterConfig) bool {
	filter := s.GetFilters(config)
	if len(filter) == 0 {
		return false
	}
	return filter[0](event)
}
