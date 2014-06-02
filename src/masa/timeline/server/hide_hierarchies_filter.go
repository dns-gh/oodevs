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
	"masa/timeline/sdk"
	"masa/timeline/services"
)

type HideHierarchiesFilter struct{}

func (s HideHierarchiesFilter) GetFilters(config services.EventFilterConfig) []services.EventFilter {
	filters := []services.EventFilter{}
	uuids, ok := config["filter_hide_hierarchies"].(map[string]struct{})
	if !ok {
		return filters
	}
	return append(filters, func(event *sdk.Event) bool {
		_, found := uuids[event.GetParent()]
		return found
	})
}

func (s HideHierarchiesFilter) Filter(event *sdk.Event, config services.EventFilterConfig) bool {
	filter := s.GetFilters(config)
	if len(filter) == 0 {
		return false
	}
	return filter[0](event)
}
