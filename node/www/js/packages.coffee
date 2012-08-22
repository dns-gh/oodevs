# *****************************************************************************
#
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
#
# Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
#
# *****************************************************************************

package_template = Handlebars.compile $("#package_template").html()
package_error_template = Handlebars.compile $("#package_error_template").html()

print_error = (text) ->
    display_error "upload_error", package_error_template, text

class Package extends Backbone.Model
    view: PackageView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_install", id: uuid,
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

    toggle_load: (next, group) =>
        @enabled = next
        if group?
            toggle_spinner $ group

    delete_items: (list) =>
        next = []
        for it in list
            next.push $(it).parent().attr "data-rel"
        ajax "/api/delete_install", id: uuid, items: next.join ',',
            (item) =>
                @toggle_load true, $ list
                @model.set item
            () =>
                @toggle_load true, $ list
                print_error "Unable to delete package(s)"

    render: =>
        return unless @enabled
        $(@el).empty()
        return unless @model.attributes.items?

        $(@el).html package_template @model.attributes

        for it in $(@el).find ".package_header .remove_all a"
            $(it).click =>
                items = []
                for btn in $(@el).find ".action .delete"
                    @toggle_load false, btn
                    items.push btn
                @delete_items items if items.length

        for it in $(@el).find ".action .more"
            $(it).click ->
                $("#briefing_" + $(@).parent().attr "data-rel").toggle "fast"

        for it in $(@el).find ".name .error"
            $(it).tooltip placement: "top"

        for it in $(@el).find ".action .delete"
            $(it).click it, (e) =>
                @toggle_load false, it
                @delete_items [it]
        return

    delta: =>
        list = new Package
        list.fetch
            success: =>
                if list.attributes.items?
                    @model.set list.attributes
                else
                    @model.clear()
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView
