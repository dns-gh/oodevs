# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

get_url = (url) ->
    return window.location.protocol + "//" + window.location.hostname + ":" + window.location.port + url

ajax = (url, data, success, error) ->
    $.ajax
        cache:    false
        data:     data,
        dataType: "json"
        error:    error,
        success:  success,
        url:      get_url url

diff_models = (prev, next) ->
    not_found = []
    found = []
    prev_ids = _(prev).map (item) -> item.id
    for item in next
        if prev_ids.indexOf(item.id) == -1
            not_found.push item
        else
            found.push item
    return [not_found, found]

set_spinner = (btn) ->
    spin_opts =
        lines:      12
        length:     4
        width:      2
        radius:     4
        rotate:     0
        color:      '#000'
        speed:      1
        trail:      60
        shadow:     false
        hwaccel:    true
        className:  'spinner'
        zIndex:     2e9
    spinner = new Spinner(spin_opts).spin()
    btn.html spinner.el

Handlebars.registerHelper "is_option", (value, options) ->
    if value of options.hash
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "equal", (lhs, rhs, options) ->
    if lhs == rhs
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "is_first_item", (items, type, options) ->
    if items[0]?.type == type
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "has_item_type", (items, type, options) ->
    for it in items
        if it.type == type
            return options.fn this
    return options.inverse this

Handlebars.registerHelper "is_odd_item_row", (items, type, options) ->
    types = {}
    idx = 0
    for it in items
        break if it.type == type
        ++idx unless types[it.type]?
        types[it.type] = true
        ++idx
    if idx&1
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "for_all_items", (items, type, options) ->
    buffer = ''
    for it in items
        if it.type == type
            buffer += options.fn it
    return buffer

display_error = (id, template, text) ->
    ctl = $ "#" + id
    ctl.html template content: text
    ctl.show()
    $("html, body").animate scrollTop: 0, "fast"
    setTimeout (-> ctl.hide()), 3000

text_compare = (lhs, rhs) ->
    lhs = lhs.toLowerCase()
    rhs = rhs.toLowerCase()
    if lhs > rhs
        return +1
    if lhs < rhs
        return -1
    return 0

transform_to_spinner = (btn, append) ->
    id = $(btn).parent().attr "data-rel"
    spin = $ "<a class='btn disabled spin_btn'></a>"
    set_spinner spin
    ctl = $(btn).parent()
    if append?
        spin.appendTo ctl
    else
        spin.prependTo ctl
    $(btn).hide()
    return id
