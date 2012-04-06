(function() {
  var NodeItem, NodeItemView, NodeList, NodeListView, ajax, diff_models, node_error_template, node_template, node_view, on_node_click, on_node_hide, print_error, reset_input_node, validate_input_node,
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

  node_template = Handlebars.compile($("#node_template").html());

  node_error_template = Handlebars.compile($("#node_error_template").html());

  print_error = function(text) {
    return $("#node_error").html(node_error_template({
      content: text
    }));
  };

  NodeItem = (function(_super) {

    __extends(NodeItem, _super);

    function NodeItem() {
      this.sync = __bind(this.sync, this);
      NodeItem.__super__.constructor.apply(this, arguments);
    }

    NodeItem.prototype.view = NodeItemView;

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
      this["delete"] = __bind(this["delete"], this);
      this.render = __bind(this.render, this);
      NodeItemView.__super__.constructor.apply(this, arguments);
    }

    NodeItemView.prototype.tagName = "div";

    NodeItemView.prototype.className = "row";

    NodeItemView.prototype.initialize = function(obj) {
      this.model.bind('change', this.render);
      return this.render();
    };

    NodeItemView.prototype.render = function() {
      $(this.el).empty();
      return $(this.el).html(node_template(this.model.attributes));
    };

    NodeItemView.prototype["delete"] = function() {
      var _this = this;
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete node " + _this.model.get("name"));
        }
      });
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
      return setInterval(this.delta, 5 * 1000);
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
        },
        error: function() {
          return print_error("Unable to fetch nodes");
        }
      });
    };

    return NodeListView;

  })(Backbone.View);

  node_view = new NodeListView;

  reset_input_node = function(control) {
    control.parent().parent().removeClass("error");
    return control.parent().find(".help-inline").hide();
  };

  validate_input_node = function(control, result) {
    if (!result) {
      control.parent().parent().addClass("error");
      control.parent().find(".help-inline").show();
      return false;
    }
    reset_input_node(control);
    return true;
  };

  on_node_click = function() {
    var name;
    name = $("#node_name");
    if (!validate_input_node(name, name.val().length)) return;
    $("#node_create").modal("hide");
    return node_view.create({
      name: name.val()
    });
  };

  on_node_hide = function() {
    reset_input_node($("#node_name"));
    return $("#node_create .modal-footer .alert").hide();
  };

  $("#node_create .modal-footer .btn_click").click(on_node_click);

  $("#node_create").on("hidden", on_node_hide);

}).call(this);
