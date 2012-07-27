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

raw_ajax = (url, data, success, error, type) ->
    $.ajax
        cache:    false
        data:     data,
        dataType: "json"
        error:    error,
        success:  success,
        url:      get_url url
        type:     type

ajax = (url, data, success, error) ->
    raw_ajax url, data, success, error, "GET"

pajax = (url, data, success, error) ->
    raw_ajax url, data, success, error, "POST"

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

update_model = (current, next) ->
    rpy = diff_models next.models, current.models
    current.remove rpy[0]
    rpy = diff_models current.models, next.models
    current.add rpy[0]
    for it in rpy[1]
        current.get(it.id).set it.attributes

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

parse_parameters = ->
    a = /\+/g
    r = /([^&;=]+)=?([^&;]*)/g
    d = (s) -> decodeURIComponent s.replace a, " "
    q = window.location.search.substring 1
    params = {}
    while (e = r.exec q)
      v = d e[2]
      params[d e[1]] = if v.length then v else null
    return params

$(".sign_out").click ->
    ajax "/api/logout", {},
        ->  location.reload(),
        ->  location.reload()

toggle_input_error = (el, txt, def = '') ->
    el.val def if def?
    root = el.parent().parent()
    return if root.hasClass "error"
    root.addClass "error"
    if txt?
        msg = $ "<span class='help-inline'>" + txt + "</span>"
        el.after msg
    reset_error = ->
        msg.empty() if msg?
        root.removeClass "error"
    el.focus -> reset_error()
    setTimeout reset_error, 3000

is_disabled = (evt) ->
    return $(evt.currentTarget).hasClass "disabled"

bytes_to_size = (n, precision) ->
    kb = 1000
    mb = kb * 1000
    gb = mb * 1000
    tb = gb * 1000
    if n < kb
        return n + ' B'
    if n < mb
        return ( n / kb ).toFixed( precision ) + ' KB'
    if n < gb
        return ( n / mb ).toFixed( precision ) + ' MB'
    if n < tb
        return ( n / gb ).toFixed( precision ) + ' GB'
    return ( n / tb ).toFixed( precision ) + ' TB'

duration_reduce = (ms, step, txt, suffix) ->
    return [ ms, txt ] if ms < step
    n = Math.floor( ms / step )
    txt += n + suffix + ' '
    ms -= n * step
    return [ ms, txt ]

ms_to_duration = (ms) ->
    [ ms, rpy ] = duration_reduce ms, 1000*60*60*24, "", 'd'
    [ ms, rpy ] = duration_reduce ms, 1000*60*60, rpy, 'h'
    [ ms, rpy ] = duration_reduce ms, 1000*60, rpy, 'm'
    [ ms, rpy ] = duration_reduce ms, 1000, rpy, 's'
    return rpy

get_ms_duration_from = (start_ms) ->
    start = new Date start_ms
    now = new Date()
    now = now.getTime() - now.getTimezoneOffset()*60*1000
    diff = now - start.getTime()
    return ms_to_duration diff
