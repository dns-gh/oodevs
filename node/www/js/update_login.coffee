# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

$(".cancel").click ->
    $(".sign_out").click()

$("a.update_login").click ->
    cur = $("#current")
    pwd = $("#password")
    bis = $("#password_bis")
    $("input").blur()

    err = false
    if !cur.val().length
        toggle_input_error cur, "Missing"
        err = true
    if !pwd.val().length
        toggle_input_error pwd, "Missing"
        err = true
    if !bis.val().length
        toggle_input_error bis, "Missing"
        err = true
    return if err

    if pwd.val() != bis.val()
        toggle_input_error pwd, "Invalid", true
        toggle_input_error bis, "Invalid", true
        return

    pajax "/api/update_login", {},
        username: user.username
        current:  cur.val()
        password: pwd.val()
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
            toggle_input_error cur, "Invalid", true

$("form.update_login").keypress (e) ->
    if e.which == 13
        $("a.update_login").click()
