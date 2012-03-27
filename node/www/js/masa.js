(function() {

  $("#session_create .modal-footer .btn_click").click(function(event) {
    var box, data, done, error, name;
    name = $("#session_name");
    if (!name.val().length) {
      name.parent().parent().addClass("error");
      box = $("#session_create .modal-footer .alert");
      box.html("Unable to use empty name");
      box.show();
      return;
    }
    data = {
      name: name.val(),
      exercise: $("#session_exercise").val()
    };
    done = function(data) {
      return $("#session_create").modal("hide");
    };
    error = function(obj, textStatus, data) {
      box.html("Unexpected error " + textStatus + " " + data);
      return box.show();
    };
    return $.ajax({
      url: "create_session",
      data: data,
      dataType: "json",
      success: done,
      error: error
    });
  });

  $("#session_create").on("hidden", function() {
    $("#session_name").parent().parent().removeClass("error");
    return $("#session_create .modal-footer .alert").hide();
  });

}).call(this);
