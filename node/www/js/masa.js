(function() {
  var ajax, invalidate_session, on_session_click, on_session_hide;

  ajax = function(url, data, success, error) {
    return $.ajax({
      url: url,
      data: data,
      success: success,
      error: error,
      dataType: "json"
    });
  };

  invalidate_session = function(control, box, name) {
    control.parent().parent().addClass("error");
    box.html("Missing " + name);
    return box.show();
  };

  on_session_click = function() {
    var box, data, done, error, exercise, name;
    box = $("#session_create .modal-footer .alert");
    name = $("#session_name");
    exercise = $("#session_exercise");
    if (!name.val().length) return invalidate_session(name, box, "name");
    if (!exercise.val().length) {
      return invalidate_session(exercise, box, "exercise");
    }
    data = {
      name: name.val(),
      exercise: exercise.val()
    };
    done = function() {
      return $("#session_create").modal("hide");
    };
    error = function(obj, text, data) {
      box.html("Unexpected error " + text + " " + data);
      return box.show();
    };
    return ajax("create_session", data, done, error);
  };

  on_session_hide = function() {
    $("#session_name").parent().parent().removeClass("error");
    return $("#session_create .modal-footer .alert").hide();
  };

  $("#session_create .modal-footer .btn_click").click(on_session_click);

  $("#session_create").on("hidden", on_session_hide);

}).call(this);
