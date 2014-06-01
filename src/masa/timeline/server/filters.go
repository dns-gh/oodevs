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
	"fmt"
	"services"
	"strconv"
	"strings"
)

type Getter interface {
	FormValue(string) string
}

func parseSwordProfileFilter(dst services.EventFilterConfig, req Getter) {
	if profile := req.FormValue("sword_profile"); len(profile) > 0 {
		dst["sword_profile"] = profile
	}
}

func parseSwordCustomFilter(dst services.EventFilterConfig, req Getter) error {
	filter := req.FormValue("sword_filter")
	if len(filter) == 0 {
		return nil
	}
	filters := map[string]map[uint32]struct{}{
		"automats":    map[uint32]struct{}{},
		"crowds":      map[uint32]struct{}{},
		"formations":  map[uint32]struct{}{},
		"inhabitants": map[uint32]struct{}{},
		"parties":     map[uint32]struct{}{},
		"units":       map[uint32]struct{}{},
	}
	dst["sword_filter"] = filters
	for _, token := range strings.Split(filter, ",") {
		tab := strings.Split(token, ":")
		if len(tab) != 2 {
			return fmt.Errorf("invalid sword_filter format: %s, token %s", filter, tab)
		}
		id64, err := strconv.ParseUint(tab[1], 10, 32)
		if err != nil {
			return fmt.Errorf("invalid sword_filter format: %s, id %s %s", filter, tab[1], err)
		}
		id := uint32(id64)
		switch tab[0] {
		case "a":
			filters["automats"][id] = struct{}{}
		case "c":
			filters["crowds"][id] = struct{}{}
		case "f":
			filters["formations"][id] = struct{}{}
		case "i":
			filters["inhabitants"][id] = struct{}{}
		case "p":
			filters["parties"][id] = struct{}{}
		case "u":
			filters["units"][id] = struct{}{}
		default:
			return fmt.Errorf("invalid sword_filter format: %s, token %s", filter, tab[0])
		}
	}
	return nil
}

func parseSwordEngagedFilter(dst services.EventFilterConfig, req Getter) {
	engaged, err := strconv.ParseBool(req.FormValue("sword_filter_engaged"))
	dst["sword_filter_engaged"] = err == nil && engaged
}

func parseKeywordFilter(dst services.EventFilterConfig, req Getter) {
	keyword := strings.Trim(req.FormValue("filter_keyword"), " ")
	if len(keyword) > 0 {
		dst["filter_keyword"] = keyword
	}
}

func parseServiceFilter(dst services.EventFilterConfig, req Getter) error {
	filters := map[string]bool{
		"sword": true,
		"none":  true,
	}
	dst["filter_service"] = filters
	filter := req.FormValue("filter_service")
	if len(filter) == 0 {
		return nil
	}
	for _, token := range strings.Split(filter, ",") {
		service := strings.Split(token, ":")
		if len(service) != 2 {
			return fmt.Errorf("invalid filter_service format: %s, token %s", filter, service)
		}
		value, err := strconv.ParseBool(service[1])
		if err != nil {
			return fmt.Errorf("invalid filter_service format: %s, value %s %s", filter, service[1], err)
		}
		_, ok := filters[service[0]]
		if !ok {
			return fmt.Errorf("invalid filter_service format: %s, token %s", filter, service[0])
		}
		filters[service[0]] = value
	}
	return nil
}

func parseShowOnlyFilter(dst services.EventFilterConfig, req Getter) {
	if uuid := req.FormValue("filter_show_only"); len(uuid) > 0 {
		dst["filter_show_only"] = uuid
	}
}

func parseHideHierarchiesFilter(dst services.EventFilterConfig, req Getter) {
	if filter := req.FormValue("filter_hide_hierarchies"); len(filter) > 0 {
		if uuids := strings.Split(filter, ","); len(uuids) > 0 {
			filters := map[string]struct{}{}
			dst["filter_hide_hierarchies"] = filters
			for _, token := range uuids {
				filters[token] = struct{}{}
			}
		}
	}
}

func ParseEventFilterConfig(req Getter) (services.EventFilterConfig, error) {
	config := services.EventFilterConfig{}
	parseSwordProfileFilter(config, req)
	parseSwordEngagedFilter(config, req)
	err := parseSwordCustomFilter(config, req)
	if err != nil {
		return nil, err
	}
	parseKeywordFilter(config, req)
	err = parseServiceFilter(config, req)
	if err != nil {
		return nil, err
	}
	parseShowOnlyFilter(config, req)
	parseHideHierarchiesFilter(config, req)
	return config, nil
}
