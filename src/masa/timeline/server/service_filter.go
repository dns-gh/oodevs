// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package server

import (
	"masa/timeline/sdk"
	"masa/timeline/services"
	"net/url"
)

type ServiceFilter struct{}

func (s ServiceFilter) GetFilters(config services.EventFilterConfig) []services.EventFilter {
	filters := []services.EventFilter{}
	enabled, ok := config["filter_service"].(map[string]string)
	if !ok {
		return filters
	}
	return append(filters, func(event *sdk.Event) bool {
		return s.filterService(enabled, event)
	})
}

func (s ServiceFilter) Filter(event *sdk.Event, config services.EventFilterConfig) bool {
	filter := s.GetFilters(config)
	if len(filter) == 0 {
		return false
	}
	return filter[0](event)
}

func parseFilter(filter, host string) bool {
	if filter == "*" {
		return false
	}
	if filter == "0" {
		return true
	}
	return len(host) == 0 || filter != host
}

func (ServiceFilter) filterService(enabled map[string]string, event *sdk.Event) bool {
	none := enabled["none"]
	url, err := url.Parse(event.GetAction().GetTarget())
	if err != nil {
		return parseFilter(none, "")
	}
	value, found := enabled[url.Scheme]
	if !found {
		return parseFilter(none, "")
	}
	return parseFilter(value, url.Host)
}
