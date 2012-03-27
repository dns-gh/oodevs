ajax = ( url, data, success, error ) ->
    $.ajax( { url: url, data: data, success: success, error: error, dataType: "json" } )

invalidate_session = ( control, box, name ) ->
    control.parent().parent().addClass( "error" )
    box.html( "Missing " + name )
    box.show()

on_session_click = () ->
    box = $( "#session_create .modal-footer .alert" )
    name = $( "#session_name" )
    exercise = $( "#session_exercise" )
    if !name.val().length
        return invalidate_session( name, box, "name" )
    if !exercise.val().length
        return invalidate_session( exercise, box, "exercise" )
    data = { name: name.val(), exercise: exercise.val() }
    done = () ->
        $( "#session_create" ).modal( "hide" )
    error = ( obj, text, data ) ->
        box.html( "Unexpected error " + text + " " + data )
        box.show()
    ajax( "create_session", data, done, error )

on_session_hide = () ->
    $( "#session_name" ).parent().parent().removeClass( "error" )
    $( "#session_create .modal-footer .alert" ).hide()

on_session_load = () ->
    select = $( "#session_exercise" )
    select.children().remove().end()
    done = ( data ) ->
        select.append( "<option>" + item + "</option>" ) for item in data
    error = ( obj, text, data ) ->
        box = $( "#session_create .modal-footer .alert" )
        box.html( "Unexpected error " + text + " " + data )
        box.show()
    ajax( "list_exercises", { limit: 40 }, done, error )

$( "#session_create .modal-footer .btn_click" ).click( on_session_click )
$( "#session_create" ).on( "hidden", on_session_hide )
$( "#session_create" ).on( "show", on_session_load )
