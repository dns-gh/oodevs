(function() {
  var SessionItem, SessionItemView, SessionList, SessionListView, ajax, diff_models, invalidate_session, on_session_click, on_session_hide, on_session_load, session_template, session_view,
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

  SessionItem = (function(_super) {

    __extends(SessionItem, _super);

    function SessionItem() {
      this.sync = __bind(this.sync, this);
      SessionItem.__super__.constructor.apply(this, arguments);
    }

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
          id: model.get("id")
        }, options.success, options.error);
      }
      if (method === "delete") {
        return ajax("/api/delete_session", {
          id: model.get("id")
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return SessionItem;

  })(Backbone.Model);

  SessionList = (function(_super) {

    __extends(SessionList, _super);

    function SessionList() {
      this.sync = __bind(this.sync, this);
      SessionList.__super__.constructor.apply(this, arguments);
    }

    SessionList.prototype.model = SessionItem;

    SessionList.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_sessions", null, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return SessionList;

  })(Backbone.Collection);

  Handlebars.registerHelper("is_option", function(value, options) {
    if (value in options.hash) return options.fn(this);
    return options.inverse(this);
  });

  session_template = Handlebars.compile($("#session_template").html());

  SessionItemView = (function(_super) {

    __extends(SessionItemView, _super);

    function SessionItemView() {
      this["delete"] = __bind(this["delete"], this);
      this.render = __bind(this.render, this);
      SessionItemView.__super__.constructor.apply(this, arguments);
    }

    SessionItemView.prototype.tagName = "div";

    SessionItemView.prototype.className = "row";

    SessionItemView.prototype.initialize = function() {
      return this.model.bind('change', this.render);
    };

    SessionItemView.prototype.events = {
      "click .delete": "delete"
    };

    SessionItemView.prototype.render = function() {
      $(this.el).html(session_template(this.model.attributes));
      return this;
    };

    SessionItemView.prototype["delete"] = function() {
      return this.model.destroy({
        wait: true
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
      this.model.fetch();
      return setInterval(this.delta, 5 * 1000);
    };

    SessionListView.prototype.reset = function(list, options) {
      var item, _i, _len, _ref;
      _ref = list.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        item = _ref[_i];
        this.add(item);
      }
    };

    SessionListView.prototype.add = function(item) {
      var view;
      view = new SessionItemView({
        model: item
      });
      return $(this.el).append(view.render().el);
    };

    SessionListView.prototype.remove = function(item, list, index) {
      return $("#id_" + item.get("id")).parent().remove();
    };

    SessionListView.prototype.create = function(data) {
      var item;
      item = new SessionItem;
      item.set(data);
      return this.model.create(item, {
        wait: true
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
      box.html(data + " [" + text + "]");
      return box.show();
    };
    return ajax("api/list_exercises", {
      limit: 40
    }, done, error);
  };

  $("#session_create .modal-footer .btn_click").click(on_session_click);

  $("#session_create").on("hidden", on_session_hide);

  $("#session_create").on("show", on_session_load);

}).call(this);
