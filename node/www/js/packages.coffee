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

class Package extends Backbone.Model
    view: PackageView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_install", id: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

class PackageView extends Backbone.View
    el: $("#packages")

    initialize: (obj) ->
        @model = new Package
        @model.bind 'change', @render
        @model.fetch()
        setTimeout @delta, 5000

    render: =>
        $(@el).empty()
        return unless @model.attributes.items?

        $(@el).html package_template @model.attributes

        for it in $(@el).find ".action .more"
            $(it).click ->
                $("#briefing_" + $(@).parent().parent().attr "data-rel").toggle "fast"
        return

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
