// Generated by CoffeeScript 1.3.3
(function() {
  var NodeItem, NodeItemView, NodeList, NodeListView, ajax, bytes_to_size, diff_models, display_error, get_url, is_disabled, node_default, node_error_template, node_settings, node_template, node_view, pajax, parse_parameters, pop_settings, print_error, raw_ajax, set_spinner, text_compare, toggle_input_error, transform_to_spinner, update_model, validate_input_node, validate_settings,
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; };

  get_url = function(url) {
    return window.location.protocol + "//" + window.location.hostname + ":" + window.location.port + url;
  };

  raw_ajax = function(url, data, success, error, type) {
    return $.ajax({
      cache: false,
      data: data,
      dataType: "json",
      error: error,
      success: success,
      url: get_url(url),
      type: type
    });
  };

  ajax = function(url, data, success, error) {
    return raw_ajax(url, data, success, error, "GET");
  };

  pajax = function(url, data, success, error) {
    return raw_ajax(url, data, success, error, "POST");
  };

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

  update_model = function(current, next) {
    var it, rpy, _i, _len, _ref, _results;
    rpy = diff_models(next.models, current.models);
    current.remove(rpy[0]);
    rpy = diff_models(current.models, next.models);
    current.add(rpy[0]);
    _ref = rpy[1];
    _results = [];
    for (_i = 0, _len = _ref.length; _i < _len; _i++) {
      it = _ref[_i];
      _results.push(current.get(it.id).set(it.attributes));
    }
    return _results;
  };

  set_spinner = function(btn) {
    var spin_opts, spinner;
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
    spinner = new Spinner(spin_opts).spin();
    return btn.html(spinner.el);
  };

  Handlebars.registerHelper("is_option", function(value, options) {
    if (value in options.hash) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("equal", function(lhs, rhs, options) {
    if (lhs === rhs) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("is_first_item", function(items, type, options) {
    var _ref;
    if (((_ref = items[0]) != null ? _ref.type : void 0) === type) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("has_item_type", function(items, type, options) {
    var it, _i, _len;
    for (_i = 0, _len = items.length; _i < _len; _i++) {
      it = items[_i];
      if (it.type === type) {
        return options.fn(this);
      }
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("is_odd_item_row", function(items, type, options) {
    var idx, it, types, _i, _len;
    types = {};
    idx = 0;
    for (_i = 0, _len = items.length; _i < _len; _i++) {
      it = items[_i];
      if (it.type === type) {
        break;
      }
      if (types[it.type] == null) {
        ++idx;
      }
      types[it.type] = true;
      ++idx;
    }
    if (idx & 1) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("for_all_items", function(items, type, options) {
    var buffer, it, _i, _len;
    buffer = '';
    for (_i = 0, _len = items.length; _i < _len; _i++) {
      it = items[_i];
      if (it.type === type) {
        buffer += options.fn(it);
      }
    }
    return buffer;
  });

  display_error = function(id, template, text) {
    var ctl;
    ctl = $("#" + id);
    ctl.html(template({
      content: text
    }));
    ctl.show();
    $("html, body").animate({
      scrollTop: 0
    }, "fast");
    return setTimeout((function() {
      return ctl.hide();
    }), 3000);
  };

  text_compare = function(lhs, rhs) {
    lhs = lhs.toLowerCase();
    rhs = rhs.toLowerCase();
    if (lhs > rhs) {
      return +1;
    }
    if (lhs < rhs) {
      return -1;
    }
    return 0;
  };

  transform_to_spinner = function(btn, append) {
    var ctl, id, spin;
    id = $(btn).parent().attr("data-rel");
    spin = $("<a class='btn disabled spin_btn'></a>");
    set_spinner(spin);
    ctl = $(btn).parent();
    if (append != null) {
      spin.appendTo(ctl);
    } else {
      spin.prependTo(ctl);
    }
    $(btn).hide();
    return id;
  };

  parse_parameters = function() {
    var a, d, e, params, q, r, v;
    a = /\+/g;
    r = /([^&;=]+)=?([^&;]*)/g;
    d = function(s) {
      return decodeURIComponent(s.replace(a, " "));
    };
    q = window.location.search.substring(1);
    params = {};
    while ((e = r.exec(q))) {
      v = d(e[2]);
      params[d(e[1])] = v.length ? v : null;
    }
    return params;
  };

  $(".sign_out").click(function() {
    return ajax("/api/logout", {}, function() {
      return location.reload();
    }, function() {
      return location.reload();
    });
  });

  toggle_input_error = function(el, txt, def) {
    var msg, reset_error, root;
    if (def == null) {
      def = '';
    }
    if (def != null) {
      el.val(def);
    }
    root = el.parent().parent();
    if (root.hasClass("error")) {
      return;
    }
    root.addClass("error");
    if (txt != null) {
      msg = $("<span class='help-inline'>" + txt + "</span>");
      el.after(msg);
    }
    reset_error = function() {
      if (msg != null) {
        msg.empty();
      }
      return root.removeClass("error");
    };
    el.focus(function() {
      return reset_error();
    });
    return setTimeout(reset_error, 3000);
  };

  is_disabled = function(evt) {
    return $(evt.currentTarget).hasClass("disabled");
  };

  bytes_to_size = function(n, precision) {
    var gb, kb, mb, tb;
    kb = 1000;
    mb = kb * 1000;
    gb = mb * 1000;
    tb = gb * 1000;
    if (n < kb) {
      return n + ' B';
    }
    if (n < mb) {
      return (n / kb).toFixed(precision) + ' KB';
    }
    if (n < gb) {
      return (n / mb).toFixed(precision) + ' MB';
    }
    if (n < tb) {
      return (n / gb).toFixed(precision) + ' GB';
    }
    return (n / tb).toFixed(precision) + ' TB';
  };

  node_settings = Handlebars.compile($("#node_settings_template").html());

  node_template = Handlebars.compile($("#node_template").html());

  node_error_template = Handlebars.compile($("#node_error_template").html());

  print_error = function(text) {
    return display_error("node_error", node_error_template, text);
  };

  pop_settings = function(ui, data) {
    var mod;
    ui.html(node_settings(data));
    mod = ui.find(".modal");
    mod.modal("show");
    return [ui, mod];
  };

  validate_settings = function(ui, item) {
    var data, max, name, par;
    name = ui.find(".name");
    max = ui.find(".num_sessions");
    par = ui.find(".parallel_sessions");
    data = {
      num_sessions: parseInt(max.val(), 10),
      parallel_sessions: parseInt(par.val(), 10)
    };
    if (name.val() != null) {
      data.name = name.val();
    }
    return data;
  };

  NodeItem = (function(_super) {

    __extends(NodeItem, _super);

    function NodeItem() {
      this.sync = __bind(this.sync, this);
      return NodeItem.__super__.constructor.apply(this, arguments);
    }

    NodeItem.prototype.view = NodeItemView;

    NodeItem.prototype.defaults = {
      num_sessions: 0,
      parallel_sessions: 0
    };

    NodeItem.prototype.sync = function(method, model, options) {
      if (method === "create") {
        return ajax("/api/create_node", model.attributes, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_node", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "update") {
        return ajax("/api/update_node", model.attributes, options.success, options.error);
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
      return NodeList.__super__.constructor.apply(this, arguments);
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
      return text_compare(lhs.get("ident"), rhs.get("ident"));
    };

    return NodeList;

  })(Backbone.Collection);

  NodeItemView = (function(_super) {

    __extends(NodeItemView, _super);

    function NodeItemView() {
      this.toggle_load = __bind(this.toggle_load, this);

      this.edit = __bind(this.edit, this);

      this.play = __bind(this.play, this);

      this.stop = __bind(this.stop, this);

      this["delete"] = __bind(this["delete"], this);

      this.render = __bind(this.render, this);
      return NodeItemView.__super__.constructor.apply(this, arguments);
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
      "click .edit": "edit"
    };

    NodeItemView.prototype.render = function() {
      var data, is_open, _ref;
      is_open = (_ref = $(this.el).find(".collapse")) != null ? _ref.hasClass("in") : void 0;
      $(this.el).empty();
      data = $.extend({}, this.model.attributes);
      data.data_size = bytes_to_size(data.data_size, 2);
      if (is_open) {
        data.is_open = true;
      }
      return $(this.el).html(node_template(data));
    };

    NodeItemView.prototype["delete"] = function() {
      var _this = this;
      this.toggle_load();
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete node " + _this.model.get("ident"));
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
        print_error("Unable to stop node " + _this.model.get("ident"));
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
        print_error("Unable to start node " + _this.model.get("ident"));
        return _this.toggle_load();
      });
    };

    NodeItemView.prototype.edit = function(evt) {
      var mod, previous, ui, _ref,
        _this = this;
      if (is_disabled(evt)) {
        return;
      }
      previous = this.model.get("ident");
      _ref = pop_settings($(this.el).find(".node_settings"), this.model.attributes), ui = _ref[0], mod = _ref[1];
      return mod.find(".apply").click(function() {
        var data;
        data = validate_settings(ui, _this.model);
        if (data == null) {
          return;
        }
        mod.modal("hide");
        return _this.model.save(data, {
          wait: true,
          error: function() {
            return print_error("Unable to update node " + _this.model.get("ident"));
          }
        });
      });
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

  NodeListView = (function(_super) {

    __extends(NodeListView, _super);

    function NodeListView() {
      this.delta = __bind(this.delta, this);

      this.create = __bind(this.create, this);

      this.erase = __bind(this.erase, this);

      this.add = __bind(this.add, this);

      this.reset = __bind(this.reset, this);
      return NodeListView.__super__.constructor.apply(this, arguments);
    }

    NodeListView.prototype.el = $("#nodes");

    NodeListView.prototype.initialize = function() {
      this.model = new NodeList;
      this.model.bind("add", this.add);
      this.model.bind("remove", this.erase);
      this.model.bind("reset", this.reset);
      this.model.bind("change:name", this.model.sort);
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

    NodeListView.prototype.erase = function(item, list, index) {
      return $("#id_" + item.id).parent().remove();
    };

    NodeListView.prototype.create = function(data) {
      var item,
        _this = this;
      item = new NodeItem;
      item.set(data);
      return this.model.create(item, {
        wait: true,
        error: function() {
          return print_error("Unable to create node " + item.get("ident"));
        }
      });
    };

    NodeListView.prototype.delta = function() {
      var next,
        _this = this;
      next = new NodeList;
      return next.fetch({
        success: function() {
          update_model(_this.model, next);
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

  node_default = new NodeItem;

  validate_input_node = function(control, result) {
    var group;
    if (!result) {
      group = control.parent().parent();
      group.addClass("error");
      setTimeout((function() {
        return group.removeClass("error");
      }), 3000);
      print_error("Missing short name");
      return false;
    }
    return true;
  };

  $("#node_ident").keypress(function(e) {
    if (e.which === 13) {
      return;
    }
    if (!(e.which && e.charCode)) {
      return;
    }
    return /[a-z0-9-_]+/.test(String.fromCharCode(e.which));
  });

  $("#node_create").click(function() {
    var ident;
    ident = $("#node_ident");
    if (!validate_input_node(ident, ident.val().length)) {
      return;
    }
    node_default.set("ident", ident.val());
    node_view.create(node_default);
    return ident.val('');
  });

  $("#node_edit").click(function() {
    var mod, ui, _ref;
    _ref = pop_settings($("#node_settings"), node_default.attributes), ui = _ref[0], mod = _ref[1];
    return mod.find(".apply").click(function() {
      var data;
      data = validate_settings(ui, node_default);
      if (data == null) {
        return;
      }
      if (data.name != null) {
        node_default.set("name", data.name);
      }
      node_default.set("num_sessions", data.num_sessions);
      node_default.set("parallel_sessions", data.parallel_sessions);
      return mod.modal("hide");
    });
  });

  $(".create_form").keypress(function(e) {
    if (e.which === 13) {
      return $("#node_create").click();
    }
  });

}).call(this);
