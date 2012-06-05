# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

package_template = Handlebars.compile $("#package_template").html()
upload_error_template = Handlebars.compile $("#upload_error_template").html()

print_error = (text) ->
    display_error "upload_error", upload_error_template, text

class Package extends Backbone.Model
    view: PackageView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_cache", id: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

class PackageView extends Backbone.View
    el: $("#packages")
    enabled: true

    initialize: (obj) ->
        @model = new Package
        @model.bind 'change', @render
        @model.fetch()
        setTimeout @delta, 5000

    reset: =>
        @model.clear()
        $(@el).empty()

    render: =>
        return unless @enabled

        $(@el).empty()
        return unless @model.attributes.name?

        $(@el).html package_template @model.attributes

        for it in $(@el).find ".action .more"
            $(it).click ->
                $("#briefing_" + $(@).parent().attr "data-rel").toggle "fast"

        for it in $(@el).find ".name .error"
            $(it).tooltip placement: "top"

        discard = $(".form-actions .discard")
        save = $(".form-actions .save")

        discard.click =>
            return if discard.hasClass "disabled"
            @enabled = false
            @reset()
            ajax "/api/delete_cache", id: uuid,
                => @enabled = true
                => @enabled = true

        save.click =>
            return if save.hasClass "disabled"
            list = []
            for it in $(@el).find ".action .add, .action .update"
                continue unless $(it).hasClass "active"
                @enabled = false
                discard.addClass "disabled"
                save.addClass "disabled"
                id = transform_to_spinner it
                list.push id if id?
            if !list.length
                print_error "Please select at least one package to install"
                return
            ajax "/api/install_from_cache", id: uuid, items: list.join ',',
                (item) =>
                    @reset()
                    @enabled = true
                    @model.set item
                () =>
                    @reset()
                    @enabled = true
                    print_error "Unable to save package(s)"

        $(".toggle a").click ->
            for it in $(".action .add, .action .update")
                $(it).button "toggle"
        return

    delta: =>
        item = new Package
        item.fetch
            success: =>
                if item.attributes.name?
                    @model.set item.attributes
                else
                    @model.clear()
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView

$("#upload_form").attr "action", (get_url "/api/upload_cache") + "?id=" + uuid

set_spinner $(".spin_btn")

$("#upload_form input:file").change ->
    ctl = $("#upload_form .upload")
    if @value?
        ctl.removeClass "disabled"
    else
        ctl.addClass "disabled"
    return

toggle_load = ->
    $("#upload_form .upload").toggleClass "disabled"
    $(".upload_alert").toggle()

$("#upload_form .upload").click ->
    return if $(@).hasClass "disabled"
    toggle_load()
    package_view.enabled = false
    package_view.reset()
    $("#upload_form").submit()

$("#upload_target").load ->
    toggle_load()
    package_view.enabled = true
    package_view.model.set jQuery.parseJSON $(@).contents().text()
