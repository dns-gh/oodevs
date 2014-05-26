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
	"regexp"
	"sdk"
	"services"
)

type KeywordFilter struct{}

func (k KeywordFilter) GetFilters(config services.EventFilterConfig) []services.EventFilter {
	filters := []services.EventFilter{}
	keyword, ok := config["filter_keyword"].(string)
	if !ok {
		return filters
	}
	pattern, err := regexp.Compile("(?i:" + regexp.QuoteMeta(keyword) + ")")
	if err != nil {
		return filters
	}
	return append(filters, func(event *sdk.Event) bool {
		return k.filterKeyword(pattern, event)
	})
}

func (k KeywordFilter) Filter(event *sdk.Event, config services.EventFilterConfig) bool {
	filter := k.GetFilters(config)
	if len(filter) == 0 {
		return false
	}
	return filter[0](event)
}

func (KeywordFilter) filterKeyword(pattern *regexp.Regexp, event *sdk.Event) bool {
	return !pattern.MatchString(event.GetName())
}
