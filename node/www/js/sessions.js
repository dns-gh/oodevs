(function() {
  var SessionItem, SessionItemView, SessionList, SessionListView, ajax, diff_models, get_filters, item, on_session_click, on_session_hide, on_session_load, print_error, reset_input_session, session_error_template, session_template, session_view, spin_opts, status_order, validate_input_session, _i, _len, _ref,
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

  spin_opts = {
    lines: 12,
    length: 4,
    width: 2,
    radius: 4,
    rotate: 0,
    color: '#000',
    speed: 1,
    trail: 60,
    shadow: false,
    hwaccel: true,
    className: 'spinner',
    zIndex: 2e9
  };

  SessionItemView = (function(_super) {

    __extends(SessionItemView, _super);

    function SessionItemView() {
      this.set_load = __bind(this.set_load, this);
      this.set_filter = __bind(this.set_filter, this);
      this.play = __bind(this.play, this);
      this.stop = __bind(this.stop, this);
      this["delete"] = __bind(this["delete"], this);
      this.render = __bind(this.render, this);
      SessionItemView.__super__.constructor.apply(this, arguments);
    }

    SessionItemView.prototype.tagName = "div";

    SessionItemView.prototype.className = "row";

    SessionItemView.prototype.filters = [];

    SessionItemView.prototype.loading = false;

    SessionItemView.prototype.initialize = function(obj) {
      this.model.bind('change', this.render);
      this.filters = obj.filters;
      return this.render();
    };

    SessionItemView.prototype.events = {
      "click .delete": "delete",
      "click .stop": "stop",
      "click .play": "play"
    };

    SessionItemView.prototype.render = function() {
      var btn, context, filter, spinner, _i, _len, _ref;
      $(this.el).empty();
      _ref = this.filters;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        filter = _ref[_i];
        if (filter === this.model.get("status")) return;
      }
      context = {
        loading: this.loading
      };
      $(this.el).html(session_template(_.extend(context, this.model.attributes)));
      btn = $(this.el).find(".session_top_right .load");
      if (!btn) return;
      btn.css({
        width: 14,
        height: 18
      });
      spinner = new Spinner(spin_opts).spin();
      $(spinner.el).css({
        position: "absolute",
        top: "13px",
        left: "17px"
      });
      return btn.html(spinner.el);
    };

    SessionItemView.prototype["delete"] = function() {
      var _this = this;
      this.set_load(true);
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete session " + _this.model.get("name"));
        }
      });
    };

    SessionItemView.prototype.stop = function() {
      var _this = this;
      this.set_load(true);
      return ajax("/api/stop_session", {
        id: this.model.id
      }, function(item) {
        _this.model.set(item, {
          silent: true
        });
        return _this.set_load(false);
      }, function() {
        print_error("Unable to stop session " + _this.model.get("name"));
        return _this.set_load(false);
      });
    };

    SessionItemView.prototype.play = function() {
      var _this = this;
      this.set_load(true);
      return ajax("/api/start_session", {
        id: this.model.id
      }, function(item) {
        _this.model.set(item, {
          silent: true
        });
        return _this.set_load(false);
      }, function() {
        print_error("Unable to start session " + _this.model.get("name"));
        return _this.set_load(false);
      });
    };

    SessionItemView.prototype.set_filter = function(list) {
      this.filters = list;
      return this.render();
    };

    SessionItemView.prototype.set_load = function(value) {
      this.loading = value;
      return this.render();
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

  get_filters = function() {
    return _.pluck($("#session_filters input:not(:checked)"), "name");
  };

  SessionListView = (function(_super) {

    __extends(SessionListView, _super);

    function SessionListView() {
      this.set_filter = __bind(this.set_filter, this);
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
        model: item,
        filters: get_filters()
      });
      item.view = view;
      previous = (_ref = this.model.at(this.model.indexOf(item) - 1)) != null ? _ref.view : void 0;
      if (previous) {
        return $(previous.el).after(view.el);
      } else {
        return $(this.el).prepend(view.el);
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

    SessionListView.prototype.set_filter = function() {
      var it, list, _i, _len, _ref;
      list = get_filters();
      _ref = this.model.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        it.view.set_filter(list);
      }
    };

    return SessionListView;

  })(Backbone.View);

  session_view = new SessionListView;

  reset_input_session = function(control) {
    control.parent().parent().removeClass("error");
    return control.parent().find(".help-inline").hide();
  };

  validate_input_session = function(control, result) {
    if (!result) {
      control.parent().parent().addClass("error");
      control.parent().find(".help-inline").show();
      return false;
    }
    reset_input_session(control);
    return true;
  };

  on_session_click = function() {
    var exercise, name;
    name = $("#session_name");
    if (!validate_input_session(name, name.val().length)) return;
    exercise = $("#session_exercise");
    if (!validate_input_session(exercise, exercise.val() != null)) return;
    $("#session_create").modal("hide");
    return session_view.create({
      name: name.val(),
      exercise: exercise.val()
    });
  };

  on_session_hide = function() {
    reset_input_session($("#session_name"));
    reset_input_session($("#session_exercise"));
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

  _ref = $("#session_filters input");
  for (_i = 0, _len = _ref.length; _i < _len; _i++) {
    item = _ref[_i];
    $(item).click(function() {
      return session_view.set_filter();
    });
  }

}).call(this);
