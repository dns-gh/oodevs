ajax = (url, data, success, error) ->
    $.ajax
        cache:    false
        data:     data,
        dataType: "json"
        error:    error,
        success:  success,
        url:      url,

Handlebars.registerHelper "is_option", (value, options) ->
    if value of options.hash
        return options.fn this
    return options.inverse this

session_template = Handlebars.compile $("#session_template").html()
session_error_template = Handlebars.compile $("#session_error_template").html()

print_error = (text) ->
    $("#session_error").html session_error_template content: text

class SessionItem extends Backbone.Model
    sync: (method, model, options) =>
        if method == "create"
            params =
                name:     model.get "name"
                exercise: model.get "exercise"
            return ajax "/api/create_session", params, options.success, options.error
        if method == "read"
            return ajax "/api/get_session", id: model.id,
                options.success, options.error
        if method == "delete"
            return ajax "/api/delete_session", id: model.id,
                options.success, options.error
        return Backbone.sync method, model, options

class SessionList extends Backbone.Collection
    model: SessionItem

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_sessions", null, options.success, options.error
        return Backbone.sync method, model, options

class SessionItemView extends Backbone.View
    tagName:   "div"
    className: "row"

    initialize: ->
        @model.bind 'change', @render

    events:
        "click .delete" : "delete"
        "click .stop" : "stop"
        "click .play" : "play"

    render: =>
        $(@el).html session_template @model.attributes
        return this

    delete: =>
        @model.destroy wait: true, error: => print_error "Unable to delete session " + @model.get "name"

    stop: =>
        ajax "/api/stop_session", id: @model.id,
            (item) => @model.set item,
            => print_error "Unable to stop session " + @model.get "name"

    play: =>
        ajax "/api/start_session", id: @model.id,
            (item) => @model.set item,
            => print_error "Unable to start session " + @model.get "name"

diff_models = (prev, next) ->
    not_found = []
    found = []
    prev_ids = _(prev).map (item) -> item.id
    for item in next
        if prev_ids.indexOf(item.id) == -1
            not_found.push item
        else
            found.push item
    return [not_found, found]

class SessionListView extends Backbone.View
    el: $( "#sessions" )

    initialize: ->
        @model = new SessionList
        @model.bind "add",    @add
        @model.bind "remove", @remove
        @model.bind "reset",  @reset
        @model.fetch error: -> print_error "Unable to fetch sessions"
        setInterval @delta, 5*1000

    reset: (list, options) =>
        for item in list.models
            @add item
        return

    add: (item) =>
        view = new SessionItemView model: item
        $(@el).append view.render().el

    remove: (item, list, index) =>
        $("#id_" + item.get "id").parent().remove()

    create: (data) =>
        item = new SessionItem
        item.set data
        @model.create item, wait: true, error: => print_error "Unable to create session " + item.get "name"

    delta: =>
        next = new SessionList
        next.fetch
            success: =>
                rpy = diff_models next.models, @model.models
                @model.remove rpy[0]
                rpy = diff_models @model.models, next.models
                @model.add rpy[0]
                for item in rpy[1]
                    @model.get(item.id).set item.attributes
                return
            error: => print_error "Unable to fetch sessions"

session_view = new SessionListView

invalidate_session = (control, box, name) ->
    control.parent().parent().addClass "error"
    box.html "Missing " + name
    box.show()

on_session_click = ->
    name = $("#session_name")
    exercise = $("#session_exercise")
    if !name.val().length
        return invalidate_session name, box, "name"
    if !exercise.val()?
        return invalidate_session exercise, box, "exercise"
    $("#session_create").modal "hide"
    session_view.create name: name.val(), exercise: exercise.val()

on_session_hide = ->
    $("#session_name").parent().parent().removeClass "error"
    $("#session_exercise").parent().parent().removeClass "error"
    $("#session_create .modal-footer .alert").hide()

on_session_load = ->
    select = $("#session_exercise")
    select.children().remove().end()
    done = (data) ->
        for item in data
            select.append "<option>" + item + "</option>"
    error = (obj, text, data) ->
        box = $("#session_create .modal-footer .alert")
        box.html "Unable to fetch exercises"
        box.show()
    ajax "api/list_exercises", limit: 40, done, error

$("#session_create .modal-footer .btn_click").click on_session_click
$("#session_create").on "hidden", on_session_hide
$("#session_create").on "show", on_session_load
