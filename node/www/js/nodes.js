(function() {
  var NodeItem, NodeItemView, NodeList, NodeListView, ajax, diff_models, node_error_template, node_settings, node_template, node_view, on_node_click, on_node_config, print_error, validate_input_node,
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = Object.prototype.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor; child.__super__ = parent.prototype; return child; };

  ajax = function(url, data, success, error) {
    url = window.location.protocol + "//" + window.location.hostname + ":" + proxy + url;
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

  node_settings = Handlebars.compile($("#node_settings_template").html());

  node_template = Handlebars.compile($("#node_template").html());

  node_error_template = Handlebars.compile($("#node_error_template").html());

  print_error = function(text) {
    var ctl;
    ctl = $("#node_error");
    ctl.html(node_error_template({
      content: text
    }));
    ctl.show();
    return setTimeout((function() {
      return ctl.hide();
    }), 3000);
  };

  NodeItem = (function(_super) {

    __extends(NodeItem, _super);

    function NodeItem() {
      this.sync = __bind(this.sync, this);
      NodeItem.__super__.constructor.apply(this, arguments);
    }

    NodeItem.prototype.view = NodeItemView;

    NodeItem.prototype.defaults = {
      max_sessions: 64,
      parallel_sessions: 8
    };

    NodeItem.prototype.sync = function(method, model, options) {
      var params;
      if (method === "create") {
        params = {
          name: model.get("name")
        };
        return ajax("/api/create_node", params, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_node", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "delete") {
        return ajax("/api/delete_node", {
          id: model.id
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return NodeItem;

  })(Backbone.Model);

  NodeList = (function(_super) {

    __extends(NodeList, _super);

    function NodeList() {
      this.name_compare = __bind(this.name_compare, this);
      this.comparator = __bind(this.comparator, this);
      this.sync = __bind(this.sync, this);
      NodeList.__super__.constructor.apply(this, arguments);
    }

    NodeList.prototype.model = NodeItem;

    NodeList.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_nodes", null, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    NodeList.prototype.comparator = function(lhs, rhs) {
      return this.name_compare(lhs, rhs);
    };

    NodeList.prototype.name_compare = function(lhs, rhs) {
      var a, b;
      a = lhs.get("name").toLowerCase();
      b = rhs.get("name").toLowerCase();
      if (a > b) return +1;
      if (a < b) return -1;
      return 0;
    };

    return NodeList;

  })(Backbone.Collection);

  NodeItemView = (function(_super) {

    __extends(NodeItemView, _super);

    function NodeItemView() {
      this.toggle_load = __bind(this.toggle_load, this);
      this.config = __bind(this.config, this);
      this.play = __bind(this.play, this);
      this.stop = __bind(this.stop, this);
      this["delete"] = __bind(this["delete"], this);
      this.render = __bind(this.render, this);
      NodeItemView.__super__.constructor.apply(this, arguments);
    }

    NodeItemView.prototype.tagName = "div";

    NodeItemView.prototype.className = "accordion-group";

    NodeItemView.prototype.initialize = function(obj) {
      this.model.bind('change', this.render);
      return this.render();
    };

    NodeItemView.prototype.events = {
      "click .delete": "delete",
      "click .stop": "stop",
      "click .play": "play",
      "click .config": "config"
    };

    NodeItemView.prototype.render = function() {
      $(this.el).empty();
      return $(this.el).html(node_template(this.model.attributes));
    };

    NodeItemView.prototype["delete"] = function() {
      var _this = this;
      this.toggle_load();
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete node " + _this.model.get("name"));
        }
      });
    };

    NodeItemView.prototype.stop = function() {
      var _this = this;
      this.toggle_load();
      return ajax("/api/stop_node", {
        id: this.model.id
      }, function(item) {
        _this.toggle_load();
        return _this.model.set(item);
      }, function() {
        print_error("Unable to stop node " + _this.model.get("name"));
        return _this.toggle_load();
      });
    };

    NodeItemView.prototype.play = function() {
      var _this = this;
      this.toggle_load();
      return ajax("/api/start_node", {
        id: this.model.id
      }, function(item) {
        _this.toggle_load();
        return _this.model.set(item);
      }, function() {
        print_error("Unable to start node " + _this.model.get("name"));
        return _this.toggle_load();
      });
    };

    NodeItemView.prototype.config = function(evt) {
      if ($(evt.currentTarget).hasClass("disabled")) return;
      on_node_config($(this.el).find(".node_settings"), this.model.get("name"), this.model.get("max_sessions"), this.model.get("parallel_sessions"));
      return $(this.el).find(".node_settings .modal").modal("show");
    };

    NodeItemView.prototype.toggle_load = function() {
      var it, _i, _len, _ref, _results;
      _ref = $(this.el).find(".session_top_right .btn");
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        _results.push($(it).toggle());
      }
      return _results;
    };

    return NodeItemView;

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

  NodeListView = (function(_super) {

    __extends(NodeListView, _super);

    function NodeListView() {
      this.delta = __bind(this.delta, this);
      this.create = __bind(this.create, this);
      this.remove = __bind(this.remove, this);
      this.add = __bind(this.add, this);
      this.reset = __bind(this.reset, this);
      NodeListView.__super__.constructor.apply(this, arguments);
    }

    NodeListView.prototype.el = $("#nodes");

    NodeListView.prototype.initialize = function() {
      this.model = new NodeList;
      this.model.bind("add", this.add);
      this.model.bind("remove", this.remove);
      this.model.bind("reset", this.reset);
      this.model.bind("change", this.model.sort);
      this.model.fetch({
        error: function() {
          return print_error("Unable to fetch nodes");
        }
      });
      return setTimeout(this.delta, 5000);
    };

    NodeListView.prototype.reset = function(list, options) {
      var item, _i, _len, _ref;
      $(this.el).empty();
      _ref = list.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        item = _ref[_i];
        this.add(item);
      }
    };

    NodeListView.prototype.add = function(item) {
      var previous, view, _ref;
      view = new NodeItemView({
        model: item
      });
      item.view = view;
      previous = (_ref = this.model.at(this.model.indexOf(item) - 1)) != null ? _ref.view : void 0;
      if (previous) {
        return $(previous.el).after(view.el);
      } else {
        return $(this.el).prepend(view.el);
      }
    };

    NodeListView.prototype.remove = function(item, list, index) {
      return $("#id_" + item.get("id")).parent().remove();
    };

    NodeListView.prototype.create = function(data) {
      var item,
        _this = this;
      item = new NodeItem;
      item.set(data);
      return this.model.create(item, {
        wait: true,
        error: function() {
          return print_error("Unable to create node " + item.get("name"));
        }
      });
    };

    NodeListView.prototype.delta = function() {
      var next,
        _this = this;
      next = new NodeList;
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
          return setTimeout(_this.delta, 5000);
        },
        error: function() {
          print_error("Unable to fetch nodes");
          return setTimeout(_this.delta, 5000);
        }
      });
    };

    return NodeListView;

  })(Backbone.View);

  node_view = new NodeListView;

  validate_input_node = function(control, result) {
    var group;
    if (!result) {
      group = control.parent().parent();
      group.addClass("error");
      setTimeout((function() {
        return group.removeClass("error");
      }), 3000);
      print_error("Missing node name");
      return false;
    }
    return true;
  };

  on_node_click = function() {
    var name;
    name = $("#node_name");
    if (!validate_input_node(name, name.val().length)) return;
    return node_view.create({
      name: name.val()
    });
  };

  on_node_config = function(container, name, max, parallel) {
    return container.html(node_settings({
      name: name,
      max_sessions: max,
      parallel_sessions: parallel
    }));
  };

  on_node_config($("#node_settings"), "", 64, 8);

  $("#node_create").click(on_node_click);

  $("#node_config").click(function() {
    on_node_config($("#node_settings"), $("#node_name").val(), $("#node_settings .max_sessions").val(), $("#node_settings .parallel_sessions").val());
    return $("#node_settings .modal").modal("show");
  });

}).call(this);
