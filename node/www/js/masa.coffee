$("#session_create .modal-footer .btn_click").click( (event) -> (
    name = $("#session_name")
    if !name.val().length
        name.parent().parent().addClass("error")
        box = $("#session_create .modal-footer .alert")
        box.html("Unable to use empty name")
        box.show()
        return
    data = { name: name.val(), exercise: $("#session_exercise").val() }
    done = (data) -> (
        $("#session_create").modal("hide")
    )
    error = (obj, textStatus, data) -> (
        box.html("Unexpected error " + textStatus + " " + data)
        box.show()
    )
    $.ajax({url: "create_session", data: data, dataType: "json", success: done, error: error})
))

$("#session_create").on("hidden", () -> (
    $("#session_name").parent().parent().removeClass("error")
    $("#session_create .modal-footer .alert").hide()
))
