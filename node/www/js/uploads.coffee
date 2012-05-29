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
upload_error_template = Handlebars.compile $("#upload_error_template").html()

print_error = (text) ->
    ctl = $("#upload_error")
    ctl.html upload_error_template content: text
    ctl.show()
    $("html, body").animate scrollTop: 0, "fast"
    setTimeout (-> ctl.hide()), 3000

class Package extends Backbone.Model
    view: PackageView

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/get_cache", id: uuid,
                options.success, options.error
        return Backbone.sync method, model, options

setSpinner = (btn) ->
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
    btn.html spinner.el

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
        if @model.attributes.name?
            $(@el).html package_template @model.attributes

            for it in $(@el).find ".action .more"
                $(it).click ->
                    $("#" + $(@).parent().parent().attr "data-rel").toggle "fast"

            for it in $(@el).find ".name .error"
                $(it).tooltip placement: "right"

            $(".form-actions .discard").click =>
                @switch false, true
                ajax "/api/delete_cache", id: uuid,
                    => @switch true
                    => @switch true

            $(".form-actions .save").click =>
                list = []
                for it in $(@el).find ".action .add, .action .update"
                    continue unless $(it).hasClass "active"
                    @switch false
                    spin = $ "<a class='btn disabled spin_btn'></a>"
                    setSpinner spin
                    spin.appendTo $(it).parent()
                    $(it).hide()
                    rel = $(it).parent().parent().attr "rel"
                    rel = rel.replace /^briefing_/, ''
                    list.push rel if rel?
                if !list.length
                    print_error "Please select at least one package to install"
                    return
                ajax "/api/install_from_cache", id: uuid, items: list.join ',',
                    (item) =>
                        @switch true
                        @update item, true
                    () =>
                        @switch true
                        print_error "Unable to save package(s)"
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
        item = new Package
        item.fetch
            success: =>
                if item.attributes.name?
                    @update item.attributes
                else
                    @model.clear()
                setTimeout @delta, 5000
            error: =>
                setTimeout @delta, 5000

package_view = new PackageView

$("#upload_form").attr "action", (get_url "/api/upload_cache") + "?id=" + uuid

setSpinner $(".spin_btn")

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
    package_view.switch false, true
    $("#upload_form").submit()

$("#upload_target").load ->
    toggle_load()
    package_view.switch true
    package_view.update jQuery.parseJSON $(@).contents().text()
