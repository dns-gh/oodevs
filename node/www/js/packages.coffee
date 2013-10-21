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
    counter: 0

    initialize: (obj) ->
        @model = new Package
        @model.bind 'change', @render
        @model.fetch()
        setTimeout @delta, 5000

    toggle_load: (group) =>
        @counter++
        toggle_spinner $ group if group

    delete_items: (list) =>
        next = []
        for it in list
            next.push $(it).parent().attr "data-rel"
        ajax "/api/delete_install", id: uuid, items: next.join(','),
            (item) =>
                @toggle_load list
                @model.set item
                @enabled = true
            () =>
                @toggle_load list
                print_error "Unable to delete package(s)"
                @enabled = true

    render: =>
        @$el.empty()
        return unless @model.attributes.items?

        @$el.html package_template @model.attributes
        briefings = @$el.find ".briefing_content"
        briefings.find("style, meta").remove()

        for it in @$el.find ".package_header .remove_all a"
            $(it).click =>
                modal_confirm
                    message: "Are you sure you want to delete all packages?"
                    accept: "Delete"
                    reject: "Cancel",
                    =>
                        items = []
                        for btn in @$el.find ".action .delete"
                            @enabled = false
                            @toggle_load btn
                            items.push btn
                        @delete_items items if items.length

        for it in @$el.find ".action .more"
            $(it).click ->
                $("#briefing_" + $(@).parent().attr "data-rel").toggle "fast"

        for it in @$el.find ".name .error"
            $(it).tooltip placement: "top"

        for it in @$el.find ".action .delete"
            $(it).click it, (e) =>
                modal_confirm
                    message: "Are you sure you want to delete this package?"
                    accept: "Delete"
                    reject: "Cancel",
                    =>
                        @enabled = false
                        @toggle_load e.data
                        @delete_items [e.data]
        return

    delta: =>
        now  = @counter++
        list = new Package
        list.fetch
            success: =>
                if @enabled and now+1 == @counter
                    if list.attributes.items?
                        @model.set list.attributes
                    else
                        @model.clear()
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView
