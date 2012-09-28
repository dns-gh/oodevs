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

    toggle_load: (enabled, disable, load) =>
        @enabled = enabled unless enabled
        for it in disable
            it.toggleClass "disabled"
        if load?
            toggle_spinner load
        @enabled = enabled

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
        disable_list = [discard, save]

        discard.click =>
            return if discard.hasClass "disabled"
            modal_confirm
                message: "Are you sure you want to discard this package?"
                accept: "Discard"
                reject: "Cancel",
                =>
                    @toggle_load false, disable_list, discard
                    ajax "/api/delete_cache", id: uuid,
                        =>
                            @reset()
                            @enabled = true
                        =>
                            @toggle_load true, disable_list, discard
                            print_error "Unable to discard package(s)"

        save.click =>
            return if save.hasClass "disabled"
            list = []
            btns = []
            for it in $(@el).find ".action .add, .action .update"
                continue unless $(it).hasClass "active"
                id = $(it).parent().attr "data-rel"
                continue unless id?
                list.push id
                toggle_spinner $ it
                btns.push it
            if !list.length
                print_error "Please select at least one package to install"
                return
            @toggle_load false, disable_list
            ajax "/api/install_from_cache", id: uuid, items: list.join ',',
                (item) =>
                    @toggle_load true, disable_list, $ btns
                    @model.set item
                =>
                    @toggle_load true, disable_list, $ btns
                    print_error "Unable to save package(s)"

        $(".toggle a").click ->
            list  = $ ".action .add, .action .update"
            first = list.first().hasClass "active"
            list.toggleClass "active", !first
            return
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

toggle_upload = ->
    $("#upload_form .upload").toggleClass "disabled"
    $(".upload_alert").toggle()

$("#upload_form .upload").click ->
    return if $(@).hasClass "disabled"
    toggle_upload()
    package_view.enabled = false
    package_view.reset()
    $("#upload_form").submit()

$("#upload_target").load ->
    toggle_upload()
    package_view.enabled = true
    package_view.model.set jQuery.parseJSON $(@).contents().text()
