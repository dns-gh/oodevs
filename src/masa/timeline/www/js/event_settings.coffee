# *****************************************************************************
# This file is part of a MASA library or program.
# Refer to the included end-user license agreement for restrictions.
# Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
# *****************************************************************************

is_input_empty = (input) ->
    return !(input.val()?.length > 0)

link_checkbox_to_input = (ui, e) ->
    ui.prop "checked", !is_input_empty $ e.target

on_input_event = (widget, operand) ->
    widget.on "input keyup paste", operand

attach_checkbox_and_input = (input, checkbox, fmt) ->
    wrapper = _.wrap link_checkbox_to_input, (f, e) -> f checkbox, e
    on_input_event input, wrapper
    checkbox.click ->
        next = ""
        if checkbox.is(":checked") and is_input_empty input
            next = moment().format fmt
        if input.val() != next
            input.val next

toggle_input_error = (el, txt, reset) ->
    root = el.parents ".control-group"
    return if root.hasClass "error"
    el.val '' if reset?
    root.addClass "error"
    if txt?
        msg = $ "<span class='help-inline'>#{txt}</span>"
        root.find(".controls").children().last().after msg
    reset_error = ->
        msg.remove() if msg?
        root.removeClass "error"
    on_input_event el, reset_error
    setTimeout reset_error, 3000

check_value = (ui, cond, msg) ->
    toggle_input_error ui, msg unless cond
    return cond

class EventSettings
    @defaults: {}
    fmt: "YYYY-MM-DD HH:mm:ss"
    el:  $ "#settings"

    load: (timeline, template) =>
        data = $.extend {}, EventSettings.defaults
        data.begin   = timeline.get_current() unless data.begin?
        data.begin   = @utc_format data.begin
        data.has_end = data.end?
        data.end     = @utc_format data.end if data.end?
        data.header  = i18n "Create Event"
        data.accept  = i18n "Create"
        data.reject  = i18n "Discard"
        @el.html template data
        mod = @el.find ".modal"
        for it in @el.find ".control_date"
            @set_picker $ it
        @el.find(".apply").on "click", @on_save
        mod.modal "show"

    set_picker: (it) =>
        id = it.find(".control-label").attr "for"
        btn = it.find ".btn"
        input = it.find "input.field"
        checkbox = it.find "##{id}_check"
        attach_checkbox_and_input input, checkbox, @fmt
        text = input.val()
        current = moment text
        unless current?.isValid()
            current = moment()
            text = current.format @fmt
        picker = btn.datepicker
            format:    @fmt
            weekStart: 1
        @set_picker_date btn, current
        picker.on "changeDate", (event) =>
            picker.datepicker "hide"
            @set_date_field input, event.date
        input.focusout =>
            now = moment input.val()
            unless now?.isValid()
                now = moment()
            @set_picker_date btn, now

    set_date_field: (ui, next) =>
        now = moment ui.val()
        if now?.isValid()
            next.hour now.hour()
            next.minute now.minute()
            next.second now.second()
            next.millisecond now.millisecond()
            return if now.isSame next
        checkbox = ui.parent().find "input[type='checkbox']"
        checkbox.prop "checked", true
        ui.val next.format @fmt

    set_picker_date: (it, next) ->
        next = next.clone().startOf("day")
        it.datepicker "setValue", next

    format: (x) => moment(x).format @fmt
    utc_format: (x) => moment(x).utc().format @fmt

    validate_settings: (ui) =>
        data = {}

        name = ui.find "#evt_name"
        return unless check_value name, name.val()?.length, "Missing"
        data.name = name.val()

        data.done = ui.find("#evt_done").is ":checked"

        begin = ui.find "#evt_begin"
        return unless check_value begin, moment(begin.val())?.isValid(), "Invalid"
        data.begin = moment(begin.val() + "Z").format()

        end = ui.find "#evt_end"
        data.end = null
        unless is_input_empty end
            return unless check_value end, moment(end.val())?.isValid(), "Invalid"
            data.end = moment(end.val()).format()

        data.info = ui.find("#evt_info").val()
        EventSettings.defaults = $.extend {}, data
        return data

    on_save: =>
        data = @validate_settings @el
        return unless data?
        @el.find(".modal").modal "hide"
        $(@).trigger "save", data
