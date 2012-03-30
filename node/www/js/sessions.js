(function() {
  var SessionItem, SessionItemView, SessionList, SessionListView, ajax, diff_models, invalidate_session, on_session_click, on_session_hide, on_session_load, print_error, session_error_template, session_template, session_view, status_order,
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = Object.prototype.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor; child.__super__ = parent.prototype; return child; };

  ajax = function(url, data, success, error) {
    return $.ajax({
      cache: false,
      data: data,
      dataType: "json",
      error: error,
      success: success,
      url: url
    });
  };

  Handlebars.registerHelper("is_option", function(value, options) {
    if (value in options.hash) return options.fn(this);
    return options.inverse(this);
  });

  session_template = Handlebars.compile($("#session_template").html());

  session_error_template = Handlebars.compile($("#session_error_template").html());

  print_error = function(text) {
    return $("#session_error").html(session_error_template({
      content: text
    }));
  };

  SessionItem = (function(_super) {

    __extends(SessionItem, _super);

    function SessionItem() {
      this.sync = __bind(this.sync, this);
      SessionItem.__super__.constructor.apply(this, arguments);
    }

    SessionItem.prototype.view = SessionItemView;

    SessionItem.prototype.sync = function(method, model, options) {
      var params;
      if (method === "create") {
        params = {
          name: model.get("name"),
          exercise: model.get("exercise")
        };
        return ajax("/api/create_session", params, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_session", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "delete") {
        return ajax("/api/delete_session", {
          id: model.id
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return SessionItem;

  })(Backbone.Model);

  status_order = {
    playing: 0,
    paused: 1,
    replaying: 2,
    stopped: 3
  };

  SessionList = (function(_super) {

    __extends(SessionList, _super);

    function SessionList() {
      this.set_order = __bind(this.set_order, this);
      this.status_compare = __bind(this.status_compare, this);
      this.name_compare = __bind(this.name_compare, this);
      this.comparator = __bind(this.comparator, this);
      this.sync = __bind(this.sync, this);
      SessionList.__super__.constructor.apply(this, arguments);
    }

    SessionList.prototype.model = SessionItem;

    SessionList.prototype.order = "name";

    SessionList.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_sessions", null, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    SessionList.prototype.comparator = function(lhs, rhs) {
      if (this.order === "status") return this.status_compare(lhs, rhs);
      return this.name_compare(lhs, rhs);
    };

    SessionList.prototype.name_compare = function(lhs, rhs) {
      var a, b;
      a = lhs.get("name").toLowerCase();
      b = rhs.get("name").toLowerCase();
      if (a > b) return +1;
      if (a < b) return -1;
      return 0;
    };

    SessionList.prototype.status_compare = function(lhs, rhs) {
      var a, b;
      a = lhs.get("status");
      b = rhs.get("status");
      if (a === b) return this.name_compare(lhs, rhs);
      return status_order[a] - status_order[b];
    };

    SessionList.prototype.set_order = function(order) {
      this.order = order;
      return this.sort();
    };

    return SessionList;

  })(Backbone.Collection);

  SessionItemView = (function(_super) {

    __extends(SessionItemView, _super);

    function SessionItemView() {
      this.play = __bind(this.play, this);
      this.stop = __bind(this.stop, this);
      this["delete"] = __bind(this["delete"], this);
      this.render = __bind(this.render, this);
      SessionItemView.__super__.constructor.apply(this, arguments);
    }

    SessionItemView.prototype.tagName = "div";

    SessionItemView.prototype.className = "row";

    SessionItemView.prototype.initialize = function() {
      this.model.bind('change', this.render);
      return this.render();
    };

    SessionItemView.prototype.events = {
      "click .delete": "delete",
      "click .stop": "stop",
      "click .play": "play"
    };

    SessionItemView.prototype.render = function() {
      return $(this.el).html(session_template(this.model.attributes));
    };

    SessionItemView.prototype["delete"] = function() {
      var _this = this;
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete session " + _this.model.get("name"));
        }
      });
    };

    SessionItemView.prototype.stop = function() {
      var _this = this;
      return ajax("/api/stop_session", {
        id: this.model.id
      }, function(item) {
        return _this.model.set(item);
      }, function() {
        return print_error("Unable to stop session " + _this.model.get("name"));
      });
    };

    SessionItemView.prototype.play = function() {
      var _this = this;
      return ajax("/api/start_session", {
        id: this.model.id
      }, function(item) {
        return _this.model.set(item);
      }, function() {
        return print_error("Unable to start session " + _this.model.get("name"));
      });
    };

    return SessionItemView;

  })(Backbone.View);

  diff_models = function(prev, next) {
    var found, item, not_found, prev_ids, _i, _len;
    not_found = [];
    found = [];
    prev_ids = _(prev).map(function(item) {
      return item.id;
    });
    for (_i = 0, _len = next.length; _i < _len; _i++) {
      item = next[_i];
      if (prev_ids.indexOf(item.id) === -1) {
        not_found.push(item);
      } else {
        found.push(item);
      }
    }
    return [not_found, found];
  };

  SessionListView = (function(_super) {

    __extends(SessionListView, _super);

    function SessionListView() {
      this.delta = __bind(this.delta, this);
      this.create = __bind(this.create, this);
      this.remove = __bind(this.remove, this);
      this.add = __bind(this.add, this);
      this.reset = __bind(this.reset, this);
      SessionListView.__super__.constructor.apply(this, arguments);
    }

    SessionListView.prototype.el = $("#sessions");

    SessionListView.prototype.initialize = function() {
      this.model = new SessionList;
      this.model.bind("add", this.add);
      this.model.bind("remove", this.remove);
      this.model.bind("reset", this.reset);
      this.model.bind("change:status", this.model.sort);
      this.model.fetch({
        error: function() {
          return print_error("Unable to fetch sessions");
        }
      });
      return setInterval(this.delta, 5 * 1000);
    };

    SessionListView.prototype.reset = function(list, options) {
      var item, _i, _len, _ref;
      $(this.el).empty();
      _ref = list.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        item = _ref[_i];
        this.add(item);
      }
    };

    SessionListView.prototype.add = function(item) {
      var previous, view, _ref;
      view = new SessionItemView({
        model: item
      });
      item.view = view;
      previous = (_ref = this.model.at(this.model.indexOf(item) - 1)) != null ? _ref.view : void 0;
      if (previous) {
        return $(previous.el).after(view.el);
      } else {
        return $(this.el).append(view.el);
      }
    };

    SessionListView.prototype.remove = function(item, list, index) {
      return $("#id_" + item.get("id")).parent().remove();
    };

    SessionListView.prototype.create = function(data) {
      var item,
        _this = this;
      item = new SessionItem;
      item.set(data);
      return this.model.create(item, {
        wait: true,
        error: function() {
          return print_error("Unable to create session " + item.get("name"));
        }
      });
    };

    SessionListView.prototype.delta = function() {
      var next,
        _this = this;
      next = new SessionList;
      return next.fetch({
        success: function() {
          var item, rpy, _i, _len, _ref;
          rpy = diff_models(next.models, _this.model.models);
          _this.model.remove(rpy[0]);
          rpy = diff_models(_this.model.models, next.models);
          _this.model.add(rpy[0]);
          _ref = rpy[1];
          for (_i = 0, _len = _ref.length; _i < _len; _i++) {
            item = _ref[_i];
            _this.model.get(item.id).set(item.attributes);
          }
        },
        error: function() {
          return print_error("Unable to fetch sessions");
        }
      });
    };

    return SessionListView;

  })(Backbone.View);

  session_view = new SessionListView;

  invalidate_session = function(control, box, name) {
    control.parent().parent().addClass("error");
    box.html("Missing " + name);
    return box.show();
  };

  on_session_click = function() {
    var exercise, name;
    name = $("#session_name");
    exercise = $("#session_exercise");
    if (!name.val().length) return invalidate_session(name, box, "name");
    if (!(exercise.val() != null)) {
      return invalidate_session(exercise, box, "exercise");
    }
    $("#session_create").modal("hide");
    return session_view.create({
      name: name.val(),
      exercise: exercise.val()
    });
  };

  on_session_hide = function() {
    $("#session_name").parent().parent().removeClass("error");
    $("#session_exercise").parent().parent().removeClass("error");
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
      box.html("Unable to fetch exercises");
      return box.show();
    };
    return ajax("api/list_exercises", {
      limit: 40
    }, done, error);
  };

  $("#session_create .modal-footer .btn_click").click(on_session_click);

  $("#session_create").on("hidden", on_session_hide);

  $("#session_create").on("show", on_session_load);

  $("#session_sort_name").click(function() {
    return session_view.model.set_order("name");
  });

  $("#session_sort_status").click(function() {
    return session_view.model.set_order("status");
  });

}).call(this);
