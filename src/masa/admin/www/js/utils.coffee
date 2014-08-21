# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

get_url = (url) ->
    rpy = window.location.protocol + "//" + window.location.hostname
    if window.location.port?.length
        rpy += ":" + window.location.port
    return rpy + url

load_url = (url) ->
    if url == window.location.href
        location.reload()
    else
        window.location.href = url

get_file_content = ( url ) ->
    return $.ajax(
       url : url
       async: false
    ).responseText

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

pajax = (url, params, body, success, error) ->
    suffix = $.param params
    url = url + "?" + suffix if suffix?.length
    raw_ajax url, JSON.stringify(body), success, error, "POST"

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
        hwaccel:    false
        className:  'spinner'
        zIndex:     2e9
    spinner = new Spinner(spin_opts).spin()
    btn.html spinner.el

Handlebars.registerHelper "is_option", (value, options) ->
    if value of options.hash
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "equal", (lhs, rhs, options) ->
    regexp = new RegExp "^" + rhs + "$"
    if regexp.test lhs
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

Handlebars.registerHelper "is_positive", (value, options) ->
    if value > 0
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "iterate_pair", (list, options) ->
    ret = ""
    while list?.length > 1
        ret += options.fn list: [list[0], list[1]]
        list = list[2..]
    if list?.length
        ret += options.fn item: list[0]
    return ret

convert_to_boolean = (value) ->
    unless value?
        return false
    else if _.isBoolean value
        return value
    else if _.isNumber value
        return value > 0
    else if _.isString
        return value == "true" or value == "1"
    return false

Handlebars.registerHelper "bool", (value, options) ->
    if convert_to_boolean value
        return options.fn this
    return options.inverse this

display_error = (id, template, text, no_timeout) ->
    ctl = $ "#" + id
    ctl.html template content: text
    ctl.show()
    $("html, body").animate scrollTop: 0, "fast"
    setTimeout (-> ctl.hide()), 4000 unless no_timeout?

text_compare = (lhs, rhs) ->
    lhs = lhs.toLowerCase()
    rhs = rhs.toLowerCase()
    if lhs > rhs
        return +1
    if lhs < rhs
        return -1
    return 0

toggle_spinner = (group) ->
    spin = group.next ".spin_btn"
    if spin?.length
        spin.remove()
        group.show()
    else
        group.hide()
        next = $ "<a class='btn disabled spin_btn'></a>"
        set_spinner next
        group.first().after next
    return

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
        ->  load_url get_url window.location.pathname,
        ->  load_url get_url window.location.pathname

toggle_input_error = (el, txt, reset) ->
    root = el.parents ".control-group"
    return if root.hasClass "error"
    el.val '' if reset?
    root.addClass "error"
    if txt?
        msg = $ "<span class='help-inline'>" + txt + "</span>"
        root.find(".controls").append msg
    reset_error = ->
        msg.remove() if msg?
        root.removeClass "error"
    on_input_event el, reset_error
    setTimeout reset_error, 3000

is_clipped = (num, min, max) ->
    return false if num < min
    return false if num > max
    return true

is_disabled = (evt) ->
    return $(evt.currentTarget).hasClass "disabled"

pad = (txt, max, prefix) ->
    while txt.length < max
        txt = prefix + txt
    return txt

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
    if txt.length
        txt += ', '
    txt +=  n + suffix
    if n > 1
        txt += 's'
    ms -= n * step
    return [ ms, txt ]

ms_to_duration = (ms) ->
    [ ms, rpy ] = duration_reduce ms, 1000*60*60*24, '', ' day'
    [ ms, rpy ] = duration_reduce ms, 1000*60*60, rpy, ' hour'
    [ ms, rpy ] = duration_reduce ms, 1000*60, rpy, ' minute'
    [ ms, rpy ] = duration_reduce ms, 1000, rpy, ' second'
    return rpy

get_ms_duration_from = (start_ms) ->
    start = new Date start_ms
    now = new Date()
    now = now.getTime() - now.getTimezoneOffset()*60*1000
    diff = now - start.getTime()
    return ms_to_duration diff

force_input_regexp = (regexp, control) ->
    control.keypress (e) ->
        return if e.which == 13
        return unless e.which && e.charCode
        return regexp.test String.fromCharCode e.which

parse_scalar = (ui, f, def) ->
    return def unless ui?.val()?
    val = f ui.val()
    return def if _.isNaN val
    return val

get_number = (ui) ->
    return parse_scalar ui, parseInt, 0

get_double = (ui) ->
    return parse_scalar ui, parseFloat, null

link_checkbox_to_input = (ui, e) ->
    ui.prop "checked", get_number($ e.target) > 0

on_input_event = (widget, operand) ->
    widget.bind "input keyup", operand
    widget.on "paste", operand

attach_checkbox_and_input = (input, cbox) ->
    wrapper = _.wrap link_checkbox_to_input, (f, e) -> f cbox, e
    on_input_event input, wrapper
    cbox.click ->
        val = get_number input
        if cbox.is ":checked"
            input.val 1 if !val
        else
            input.val 0

select_attributes = (item, list) ->
    data = {}
    for it in list
        v = item[it]
        unless v?
            continue
        else if _.isObject v
            data[it] = $.extend {}, v
        else
            data[it] = v
    return data

modal_confirm = (opts, fn, args...) ->
    msg = $ """
<div class="modal fade in modal_confirm">
    <div class="alert alert-error alert-block">
        <h4 class="alert-heading">Warning</h4>
        <p> #{opts.message} </p>
        <div class="buttons">
            <a class="btn btn-danger">#{opts.accept}</a>
            <a class="btn" data-dismiss="modal">#{opts.reject}</a>
        </div>
    </div>
</div>
"""
    msg.modal "show"
    msg.find(".btn-danger").click (e) ->
        msg.modal "hide"
        fn args...
