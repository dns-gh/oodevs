// Generated by CoffeeScript 1.3.3
(function() {
  var NodeItem, NodeItemView, NodeList, NodeListView, ajax, attach_checkbox_and_input, bytes_to_size, convert_to_boolean, diff_models, display_error, duration_reduce, force_input_regexp, get_double, get_ms_duration_from, get_number, get_url, is_clipped, is_disabled, link_checkbox_to_input, load_url, ms_to_duration, node_default, node_error_template, node_settings, node_template, node_view, on_input_event, pad, pajax, parse_parameters, parse_scalar, plugin_list, pop_settings, print_error, raw_ajax, select_attributes, set_checkbox, set_plugins, set_spinner, text_compare, toggle_input_error, toggle_spinner, update_model, validate_input_node, validate_number, validate_settings,
    __indexOf = [].indexOf || function(item) { for (var i = 0, l = this.length; i < l; i++) { if (i in this && this[i] === item) return i; } return -1; },
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; };

  get_url = function(url) {
    var rpy, _ref;
    rpy = window.location.protocol + "//" + window.location.hostname;
    if ((_ref = window.location.port) != null ? _ref.length : void 0) {
      rpy += ":" + window.location.port;
    }
    return rpy + url;
  };

  load_url = function(url) {
    if (url === window.location.href) {
      return location.reload();
    } else {
      return window.location.href = url;
    }
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

  pajax = function(url, params, body, success, error) {
    var suffix;
    suffix = $.param(params);
    if (suffix != null ? suffix.length : void 0) {
      url = url + "?" + suffix;
    }
    return raw_ajax(url, JSON.stringify(body), success, error, "POST");
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

  Handlebars.registerHelper("is_positive", function(value, options) {
    if (value > 0) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("count", function(value, options) {
    if (value != null ? value.length : void 0) {
      return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("iterate_pair", function(list, options) {
    var ret;
    ret = "";
    while ((list != null ? list.length : void 0) > 1) {
      ret += options.fn({
        list: [list[0], list[1]]
      });
      list = list.slice(2);
    }
    if (list != null ? list.length : void 0) {
      ret += options.fn({
        item: list[0]
      });
    }
    return ret;
  });

  convert_to_boolean = function(value) {
    if (value == null) {
      return false;
    } else if (_.isBoolean(value)) {
      return value;
    } else if (_.isNumber(value)) {
      return value > 0;
    } else if (_.isString) {
      return value === "true" || value === "1";
    }
    return false;
  };

  Handlebars.registerHelper("bool", function(value, options) {
    if (convert_to_boolean(value)) {
      return options.fn(this);
    }
    return options.inverse(this);
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

  toggle_spinner = function(group) {
    var next, spin;
    spin = group.next(".spin_btn");
    if (spin != null ? spin.length : void 0) {
      spin.remove();
      group.show();
    } else {
      group.hide();
      next = $("<a class='btn disabled spin_btn'></a>");
      set_spinner(next);
      group.first().after(next);
    }
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

  toggle_input_error = function(el, txt, reset) {
    var msg, reset_error, root;
    root = el.parents(".control-group");
    if (root.hasClass("error")) {
      return;
    }
    if (reset != null) {
      el.val('');
    }
    root.addClass("error");
    if (txt != null) {
      msg = $("<span class='help-inline'>" + txt + "</span>");
      el.parent().children().last().after(msg);
    }
    reset_error = function() {
      if (msg != null) {
        msg.remove();
      }
      return root.removeClass("error");
    };
    on_input_event(el, reset_error);
    return setTimeout(reset_error, 3000);
  };

  is_clipped = function(num, min, max) {
    if (num < min) {
      return false;
    }
    if (num > max) {
      return false;
    }
    return true;
  };

  is_disabled = function(evt) {
    return $(evt.currentTarget).hasClass("disabled");
  };

  pad = function(txt, max, prefix) {
    while (txt.length < max) {
      txt = prefix + txt;
    }
    return txt;
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

  duration_reduce = function(ms, step, txt, suffix) {
    var n;
    if (ms < step) {
      return [ms, txt];
    }
    n = Math.floor(ms / step);
    if (txt.length) {
      txt += ', ';
    }
    txt += n + suffix;
    if (n > 1) {
      txt += 's';
    }
    ms -= n * step;
    return [ms, txt];
  };

  ms_to_duration = function(ms) {
    var rpy, _ref, _ref1, _ref2, _ref3;
    _ref = duration_reduce(ms, 1000 * 60 * 60 * 24, '', ' day'), ms = _ref[0], rpy = _ref[1];
    _ref1 = duration_reduce(ms, 1000 * 60 * 60, rpy, ' hour'), ms = _ref1[0], rpy = _ref1[1];
    _ref2 = duration_reduce(ms, 1000 * 60, rpy, ' minute'), ms = _ref2[0], rpy = _ref2[1];
    _ref3 = duration_reduce(ms, 1000, rpy, ' second'), ms = _ref3[0], rpy = _ref3[1];
    return rpy;
  };

  get_ms_duration_from = function(start_ms) {
    var diff, now, start;
    start = new Date(start_ms);
    now = new Date();
    now = now.getTime() - now.getTimezoneOffset() * 60 * 1000;
    diff = now - start.getTime();
    return ms_to_duration(diff);
  };

  force_input_regexp = function(regexp, control) {
    return control.keypress(function(e) {
      if (e.which === 13) {
        return;
      }
      if (!(e.which && e.charCode)) {
        return;
      }
      return regexp.test(String.fromCharCode(e.which));
    });
  };

  set_checkbox = function(w, enabled) {
    if (enabled) {
      return w.attr("checked", true);
    } else {
      return w.removeAttr("checked");
    }
  };

  parse_scalar = function(ui, f, def) {
    var val;
    if ((ui != null ? ui.val() : void 0) == null) {
      return def;
    }
    val = f(ui.val());
    if (_.isNaN(val)) {
      return def;
    }
    return val;
  };

  get_number = function(ui) {
    return parse_scalar(ui, parseInt, 0);
  };

  get_double = function(ui) {
    return parse_scalar(ui, parseFloat, null);
  };

  link_checkbox_to_input = function(ui, e) {
    return set_checkbox(ui, get_number($(e.target)) > 0);
  };

  on_input_event = function(widget, operand) {
    widget.bind("input keyup", operand);
    return widget.live("paste", operand);
  };

  attach_checkbox_and_input = function(input, cbox) {
    var wrapper;
    wrapper = _.wrap(link_checkbox_to_input, function(f, e) {
      return f(cbox, e);
    });
    on_input_event(input, wrapper);
    return cbox.click(function() {
      var val;
      val = get_number(input);
      if (cbox.is(":checked")) {
        if (!val) {
          return input.val(1);
        }
      } else {
        return input.val(0);
      }
    });
  };

  select_attributes = function(item, list) {
    var data, it, v, _i, _len;
    data = {};
    for (_i = 0, _len = list.length; _i < _len; _i++) {
      it = list[_i];
      v = item[it];
      if (v == null) {
        continue;
      } else if (_.isObject(v)) {
        data[it] = $.extend({}, v);
      } else {
        data[it] = v;
      }
    }
    return data;
  };

  node_settings = Handlebars.compile($("#node_settings_template").html());

  node_template = Handlebars.compile($("#node_template").html());

  node_error_template = Handlebars.compile($("#node_error_template").html());

  print_error = function(text) {
    return display_error("node_error", node_error_template, text);
  };

  plugin_list = [];

  ajax("/api/list_plugins", null, function(data) {
    return plugin_list = data;
  }, function() {
    return print_error("Unable to list plugins");
  });

  set_plugins = function(data) {
    var it, next, plugins, _i, _len;
    plugins = [];
    for (_i = 0, _len = plugin_list.length; _i < _len; _i++) {
      it = plugin_list[_i];
      next = {};
      next.id = it;
      next.name = it.charAt(0).toUpperCase() + it.slice(1);
      if (__indexOf.call(data.plugins, it) >= 0) {
        next.checked = "checked=\"checked\"";
      }
      plugins.push(next);
    }
    data.plugins = plugins;
    return data;
  };

  pop_settings = function(ui, data) {
    var mod;
    data = set_plugins($.extend({}, data));
    ui.html(node_settings(data));
    force_input_regexp(/\d/, ui.find("input[type='number']"));
    attach_checkbox_and_input($("#sessions_max_play"), $("#sessions_max_play_check"));
    attach_checkbox_and_input($("#sessions_max_parallel"), $("#sessions_max_parallel_check"));
    mod = ui.find(".modal");
    mod.modal("show");
    return [ui, mod];
  };

  validate_number = function(data, key, ui, id, min, max, msg) {
    var val, widget;
    widget = ui.find("#" + id);
    val = get_number(widget);
    if (!is_clipped(val, min, max)) {
      toggle_input_error(widget, msg);
      return false;
    }
    data[key] = val;
    return true;
  };

  validate_settings = function(ui) {
    var data, it, name, next, _i, _len, _ref;
    data = {};
    name = $("#name");
    if (name.val() != null) {
      data.name = name.val();
    }
    next = data.sessions = {};
    if (!validate_number(next, "max_play", ui, "sessions_max_play", 0, Number.MAX_VALUE, "Invalid")) {
      return;
    }
    if (!validate_number(next, "max_parallel", ui, "sessions_max_parallel", 0, Number.MAX_VALUE, "Invalid")) {
      return;
    }
    next.reset = ui.find("#sessions_reset").is(":checked");
    next = data.plugins = [];
    _ref = ui.find("#tab_plugins input[type=checkbox]:checked");
    for (_i = 0, _len = _ref.length; _i < _len; _i++) {
      it = _ref[_i];
      next.push(it.id);
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
      sessions: {
        max_play: 0,
        max_parallel: 0,
        reset: true
      },
      plugins: []
    };

    NodeItem.prototype.sync = function(method, model, options) {
      var data;
      data = select_attributes(model.attributes, ["name", "sessions", "plugins"]);
      if (method === "create") {
        data.ident = model.get("ident");
        return pajax("/api/create_node", {}, data, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_node", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "update") {
        data.id = model.id;
        return pajax("/api/update_node", {}, data, options.success, options.error);
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
      _ref = pop_settings($("#node_settings"), this.model.attributes), ui = _ref[0], mod = _ref[1];
      return mod.find(".apply").click(function() {
        var data;
        data = validate_settings(ui);
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
      return toggle_spinner($(this.el).find(".btn-group"));
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

  force_input_regexp(/[a-z0-9-_]/, $("#node_ident"));

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
      node_default.set(data);
      return mod.modal("hide");
    });
  });

  $(".create_form").keypress(function(e) {
    if (e.which === 13) {
      return $("#node_create").click();
    }
  });

}).call(this);
