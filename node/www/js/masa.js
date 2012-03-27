(function() {
  var ajax, invalidate_session, on_session_click, on_session_hide, on_session_load;

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
    if (!(exercise.val() != null)) {
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

  on_session_load = function() {
    var done, error, select;
    select = $("#session_exercise");
    select.children().remove().end();
    done = function(data) {
      var item, _i, _len, _results;
      _results = [];
      for (_i = 0, _len = data.length; _i < _len; _i++) {
        item = data[_i];
        _results.push(select.append("<option>" + item + "</option>"));
      }
      return _results;
    };
    error = function(obj, text, data) {
      var box;
      box = $("#session_create .modal-footer .alert");
      box.html("Unexpected error " + text + " " + data);
      return box.show();
    };
    return ajax("list_exercises", {
      limit: 40
    }, done, error);
  };

  $("#session_create .modal-footer .btn_click").click(on_session_click);

  $("#session_create").on("hidden", on_session_hide);

  $("#session_create").on("show", on_session_load);

}).call(this);
