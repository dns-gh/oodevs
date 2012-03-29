ajax = (url, data, success, error) ->
    $.ajax
        cache:    false
        data:     data,
        dataType: "json"
        error:    error,
        success:  success,
        url:      url,

class SessionItem extends Backbone.Model
    sync: (method, model, options) =>
        if method == "create"
            params =
                name:     model.get "name"
                exercise: model.get "exercise"
            return ajax "/create_session", params, options.success, options.error
        if method == "read"
            return ajax "/get_session", {id: model.get "id"}, options.success, options.error
        if method == "delete"
            return ajax "/delete_session", {id: model.get "id"}, options.success, options.error
        return Backbone.sync method, model, options

class SessionList extends Backbone.Collection
    model: SessionItem

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/list_sessions", null, options.success, options.error
        return Backbone.sync method, model, options

class SessionItemView extends Backbone.View
    tagName:   "div"
    className: "row"

    initialize: ->
        @model.bind 'change', @render

    events:
        "click .delete" : "delete"

    render: =>
        $(@el).html """
<div id="id_#{@model.get "id"}" class="span9">
    <div class="well run session_row">
        <div class="span3">
            <div class="session_name">#{@model.get "name"}</div>
            <div class="session_exercise">#{@model.get "exercise"}</div>
            <div class="session_model">Model: 2010-12-10</div>
            <span class="label">#{@model.get "id"}</span>
        </div>
        <div class="span4">
            <div>Started 2012-01-01 at 00:00:00</div>
            <div>Time running 00:00:00</div>
            <div>0 clients connected</div>
        </div>
        <div class="btn-group session_top_right">
            <a href="#" class="btn"><i class="icon-play"></i></a>
            <a href="#" class="btn"><i class="icon-pause"></i></a>
            <a href="#" class="btn"><i class="icon-stop"></i></a>
            <a href="#" class="btn"><i class="icon-plus-sign"></i></a>
            <a href="#" class="btn delete"><i class="icon-trash"></i></a>
        </div>
        <div class="session_bottom_right">
            <a href="#" class="btn"><i class="icon-th-list"></i> More <span class="caret"></span></a>
        </div>
    </div>
</div>
"""
        return this

    delete: =>
        @model.destroy wait: true

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
        @model.fetch()
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
        @model.create item, wait: true

    delta: =>
        prev = @model
        next = new SessionList
        next.fetch success: () ->
            rpy = diff_models next.models, prev.models
            prev.remove rpy[0]
            rpy = diff_models prev.models, next.models
            prev.add rpy[0]
            for item in rpy[1]
                prev.get(item.id).set item.attributes
            return

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
        box.html data + " [" + text + "]"
        box.show()
    ajax "list_exercises", limit: 40, done, error

$("#session_create .modal-footer .btn_click").click on_session_click
$("#session_create").on "hidden", on_session_hide
$("#session_create").on "show", on_session_load
