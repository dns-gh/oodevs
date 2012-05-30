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

    switch: (next, reset) =>
        @enabled = next
        if reset? and !next
            $(@el).empty()

    render: =>
        return unless @enabled
        $(@el).empty()
        return unless @model.attributes.items?

        $(@el).html package_template @model.attributes

        for it in $(@el).find ".action .more"
            $(it).click ->
                $("#briefing_" + $(@).parent().attr "data-rel").toggle "fast"

        for it in $(@el).find ".name .error"
            $(it).tooltip placement: "top"

        for it in $(@el).find ".action .delete"
            $(it).click it, (e) =>
                @switch false
                id = transform_to_spinner e.data, true
                ajax "/api/delete_install", id: uuid, items: id,
                    (item) =>
                        @switch true
                        @update item, true
                    () =>
                        @switch true
                        print_error "Unable to delete package(s)"
        return

    update: (data, buttons) =>
        if buttons?
            for it in $(@el).find ".btn"
                if $(it).hasClass "spin_btn"
                    $(it).remove()
                else
                    $(it).show()
                    $(it).removeClass "active"
        @model.set data

    delta: =>
        list = new Package
        list.fetch
            success: =>
                if list.attributes.items?
                    @update list.attributes
                else
                    @model.clear()
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView
