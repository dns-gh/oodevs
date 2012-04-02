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
    view: SessionItemView

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

status_order =
    playing:   0
    paused:    1
    replaying: 2
    stopped:   3

class SessionList extends Backbone.Collection
    model: SessionItem
    order: "name"

    sync: (method, model, options) =>
        if method == "read"
            return ajax "/api/list_sessions", null, options.success, options.error
        return Backbone.sync method, model, options

    comparator: (lhs, rhs) =>
        if @order == "status"
            return @status_compare lhs, rhs
        return @name_compare lhs, rhs

    name_compare: (lhs, rhs) =>
        a = lhs.get("name").toLowerCase()
        b = rhs.get("name").toLowerCase()
        if a > b
            return +1
        if a < b
            return -1
        return 0

    status_compare: (lhs, rhs) =>
        a = lhs.get "status"
        b = rhs.get "status"
        if a == b
            return @name_compare lhs, rhs
        return status_order[a] - status_order[b]

    set_order: (order) =>
        @order = order
        @sort()

spin_opts =
  lines:     12
  length:    4
  width:     2
  radius:    4
  rotate:    0
  color:     '#000'
  speed:     1
  trail:     60
  shadow:    false
  hwaccel:   true
  className: 'spinner'
  zIndex:    2e9

class SessionItemView extends Backbone.View
    tagName:   "div"
    className: "row"
    filters:   []
    spinner:   null
    search:    null

    initialize: (obj) ->
        @model.bind 'change', @render
        @filters = obj.filters
        @search  = obj.search
        @spinner = new Spinner(spin_opts).spin()
        @render()

    events:
        "click .delete" : "delete"
        "click .stop" : "stop"
        "click .play" : "play"

    is_search: =>
        targets = ["name", "exercise"]
        for it in targets
            if @model.get(it).toLowerCase().indexOf(@search) >= 0
                return true
        return false

    render: =>
        $(@el).empty()
        for filter in @filters
            if filter == @model.get "status"
                return
        if @search and !@is_search()
            return
        $(@el).html session_template @model.attributes
        $(@el).find(".session_top_right .spin_btn").html @spinner.el

    delete: =>
        @toggle_load()
        @model.destroy wait: true, error: => print_error "Unable to delete session " + @model.get "name"

    stop: =>
        @toggle_load()
        ajax "/api/stop_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to stop session " + @model.get "name"
                @toggle_load()

    play: =>
        @toggle_load()
        ajax "/api/start_session", id: @model.id,
            (item) =>
                @toggle_load()
                @model.set item
            () =>
                print_error "Unable to start session " + @model.get "name"
                @toggle_load()

    set_filter: (list) =>
        @filters = list
        @render()

    toggle_load: =>
        for it in $(@el).find(".session_top_right .btn")
            $(it).toggle()

    set_search: (item) =>
        @search = item
        @render()

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

get_filters = ->
    _.pluck $("#session_filters input:not(:checked)"), "name"

get_search = ->
    $(".session_search .search-query").val()

class SessionListView extends Backbone.View
    el: $( "#sessions" )

    initialize: ->
        @model = new SessionList
        @model.bind "add",    @add
        @model.bind "remove", @remove
        @model.bind "reset",  @reset
        @model.bind "change", @model.sort
        @model.fetch error: -> print_error "Unable to fetch sessions"
        setInterval @delta, 5*1000

    reset: (list, options) =>
        $(@el).empty()
        for item in list.models
            @add item
        return

    add: (item) =>
        view = new SessionItemView model: item, search: get_search(), filters: get_filters()
        item.view = view
        previous = @model.at(@model.indexOf(item) - 1)?.view
        if previous
            $(previous.el).after view.el
        else
            $(@el).prepend view.el

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

    set_filter: =>
        list = get_filters()
        for it in @model.models
            it.view.set_filter list
        return

    set_search: (value) =>
        value = value.toLowerCase()
        for it in @model.models
            it.view.set_search value

session_view = new SessionListView

reset_input_session = (control) ->
    control.parent().parent().removeClass "error"
    control.parent().find(".help-inline").hide()

validate_input_session = (control, result) ->
    if !result
        control.parent().parent().addClass "error"
        control.parent().find(".help-inline").show()
        return false
    reset_input_session control
    return true

on_session_click = ->
    name = $("#session_name")
    if !validate_input_session name, name.val().length
        return
    exercise = $("#session_exercise")
    if !validate_input_session exercise, exercise.val()?
        return
    $("#session_create").modal "hide"
    session_view.create name: name.val(), exercise: exercise.val()

on_session_hide = ->
    reset_input_session $("#session_name")
    reset_input_session $("#session_exercise")
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
$("#session_sort_name").click -> session_view.model.set_order "name"
$("#session_sort_status").click -> session_view.model.set_order "status"
for item in $("#session_filters input")
    $(item).click -> session_view.set_filter()
$(".session_search .btn").click -> session_view.set_search get_search()
$(".session_search input").change -> session_view.set_search get_search()
