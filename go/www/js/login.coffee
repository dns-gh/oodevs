# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

$(".log_in").click ->
    usr = $("#username")
    pwd = $("#password")
    $("input").blur()

    err = false
    if usr.val().length == 0
        toggle_input_error usr, "Missing"
        err = true
    if pwd.val().length == 0
        toggle_input_error pwd, "Missing"
        err = true
    return if err

    pajax "/api/login", {},
        username: usr.val()
        password: pwd.val(),
        (obj) ->
            uri = get_url window.location.pathname
            first = true
            uri_params = parse_parameters()
            delete uri_params.sid
            $.cookie "sid", obj.sid, expires: 7, path: '/'
            for k,v of uri_params
                uri += if first then "?" else "&"
                first = false
                uri += k
                uri += "=" + v if v?
            load_url uri + window.location.hash
        ->
            toggle_input_error pwd, "Invalid", true

$("#sign_in_form").keypress (e) ->
    if e.which == 13
        $(".log_in").click()
