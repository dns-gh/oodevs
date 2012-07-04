# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

error_template = Handlebars.compile $("#error_template").html()

print_error = (text) ->
    display_error "error_login", error_template, text

$(".log_in").click ->
    pajax "/api/login",
        username: $("#username").val()
        password: $("#password").val(),
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
            uri += window.location.hash
            window.location.href = uri
        ->
            print_error "Invalid username/email password combination"

$("#sign_in_form").keypress (e) ->
    if e.which == 13
        $(".log_in").click()
