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
            window.location.href = window.location.href + "?sid=" + obj.sid
        ->
            print_error "Invalid username/email password combination"
