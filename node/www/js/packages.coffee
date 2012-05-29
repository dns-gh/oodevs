get_url = (url) ->
    return window.location.protocol + "//" + window.location.hostname + ":" + proxy + url

ajax = (url, data, success, error) ->
    $.ajax
        cache:    false
        data:     data,
        dataType: "json"
        error:    error,
        success:  success,
        url:      get_url url

Handlebars.registerHelper "is_header", (items, type, options) ->
    if items[0]?.type == type
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "has_item_type", (items, type, options) ->
    for it in items
        if it.type == type
            return options.fn this
    return options.inverse this

Handlebars.registerHelper "forall", (items, type, options) ->
    buffer = ''
    for it in items
        if it.type == type
            buffer += options.fn it
    return buffer

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
                $("#" + $(@).parent().parent().attr "rel").toggle "fast"
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
