# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

license_template = Handlebars.compile $("#license_template").html()
license_error_template = Handlebars.compile $("#license_error_template").html()

scope = (model) ->
    model ?= {}
    if uuid?
        model.node = uuid
    return model

print_error = (text) ->
    display_error "license_error", license_error_template, text

class LicenseItem extends Backbone.Model
    view: LicenseItemView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_licenses", scope( id: model.id ),
                options.success, options.error

class LicenseItemView extends Backbone.View
    tagName: "tr"
    el: $ "#licenses"

    initialize: (obj) ->
        @model = new LicenseItem
        @model.bind 'change', @render
        @delta()

    render: =>
        @$el.empty()
        data = []
        for k, v of @model.attributes
            it = {}
            it.name = k
            it.validity = "success"
            it.expiration = v.date
            if v.validity == "none"
                it.validity = "error"
                it.expiration = "No license available"
            else if v.validity == "expired"
                it.validity = "warning"
            if v.connections
                it.connections = v.connections
            else
               it.connections = "-"
            data.push it
        @$el.html license_template model: data

    delta: =>
        @model.fetch
            success: (model, response, options) =>
                @model.set response
                setTimeout @delta, 5000
            error: =>
                print_error "Unable to fetch licenses"
                setTimeout @delta, 5000

license_view = new LicenseItemView

$("#upload_form").attr "action", (get_url "/api/upload_licenses")

$("#upload_form input:file").change ->
    ctl = $("#upload_form .upload")
    ctl.toggleClass "disabled", !@value?
    return

toggle_upload = ->
    $("#upload_form .upload").toggleClass "disabled"
    $(".upload_alert").toggle()

$("#upload_form .upload").click ->
    return if $(@).hasClass "disabled"
    $("#upload_form").submit()

$("#upload_target").load ->
    toggle_upload()
    data = $(@).contents().text()
    if !data.length
        print_error "Unable to upload license"
        return
    license_view.model.set jQuery.parseJSON data
