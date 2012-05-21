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

Handlebars.registerHelper "equal", (lhs, rhs, options) ->
    if lhs == rhs
        return options.fn this
    return options.inverse this

Handlebars.registerHelper "is_exercise", (type, options) ->
    if type == "exercise"
        return options.fn this
    return options.inverse this

package_template = Handlebars.compile $("#package_template").html()

class Package extends Backbone.Model
    view: PackageView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_pack", id: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

lastPop = null

class PackageView extends Backbone.View
    el: $("#packages")

    initialize: (obj) ->
        @model = new Package
        @model.bind 'change', @render
        @model.fetch()
        setTimeout @delta, 5000

    render: =>
        $(@el).empty()
        if @model.attributes.name?
            $(@el).html package_template @model.attributes
            for it in @model.attributes.items
                continue unless it.type == "exercise"
                $("#package_" + it.id).popover
                    trigger:   "manual",
                    placement: "bottom",
                    title:     it.name,
                    content:   $("#package_" + it.id + "_briefing").contents()
                .click ->
                    if lastPop?
                        if lastPop == @
                            lastPop = null
                            $(@).popover "hide"
                            return
                        else
                            $(lastPop).popover "hide"
                            $(@).popover "show"
                    lastPop = @
                    return
                .mouseenter ->
                    unless lastPop?
                        $(@).popover "show"
                    return
                .mouseleave ->
                    unless lastPop?
                        $(@).popover "hide"
                    return
        return

    update: (data) =>
        @model.set data

    delta: =>
        item = new Package
        item.fetch
            success: =>
                @update item.attributes
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView

$("#upload_form").attr "action", (get_url "/api/upload_pack") + "?id=" + uuid

spin_opts =
    lines:      12
    length:     4
    width:      2
    radius:     4
    rotate:     0
    color:      '#000'
    speed:      1
    trail:      60
    shadow:     false
    hwaccel:    true
    className:  'spinner'
    zIndex:     2e9

spinner = new Spinner(spin_opts).spin()
$(".spin_btn").html spinner.el

toggle_load = ->
    $("#upload_form .upload").toggleClass "disabled"
    $(".upload_alert").toggle()

$("#upload_form .upload").click ->
    return if $(@).hasClass "disabled"
    toggle_load()
    $("#upload_form").submit()

$("#upload_target").load (data) ->
    toggle_load()
    package_view.update jQuery.parseJSON $(@).contents().find("body").html()
