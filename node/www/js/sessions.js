// Generated by CoffeeScript 1.3.3
(function() {
  var ExerciseListItem, ExerciseListItemView, SessionItem, SessionItemView, SessionList, SessionListView, ajax, attach_checkbox_and_input, bytes_to_size, check_value, contains, diff_models, display_error, duration_reduce, exercise_view, flatten_item, flatten_item_attributes, force_input_regexp, get_double, get_filters, get_ms_duration_from, get_number, get_search, get_url, is_clipped, is_disabled, item, link_checkbox_to_input, ms_to_duration, on_input_event, pad, pajax, parse_parameters, parse_scalar, pop_settings, print_error, raw_ajax, select_attributes, session_error_template, session_settings_template, session_template, session_view, set_checkbox, set_spinner, status_order, text_compare, toggle_input_error, transform_to_spinner, update_model, validate_double, validate_input_session, validate_number, validate_rng, validate_settings, _i, _len, _ref,
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

  Handlebars.registerHelper("bool", function(value, options) {
    var valid;
    valid = false;
    if (valid == null) {
      valid = false;
    } else if (_.isBoolean(value)) {
      valid = value;
    } else if (_.isNumber(value)) {
      valid = value > 0;
    } else if (_.isString(value)) {
      valid = value === "true" || value === "1";
    }
    if (valid) {
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

  flatten_item_attributes = function(data, prefix, item) {
    var k, v;
    for (k in item) {
      v = item[k];
      if (_.isObject(v)) {
        flatten_item_attributes(data, prefix + k + "_", v);
      } else {
        data[prefix + k] = v;
      }
    }
  };

  flatten_item = function(item) {
    var data;
    data = {};
    flatten_item_attributes(data, "", item);
    return data;
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

  session_template = Handlebars.compile($("#session_template").html());

  session_error_template = Handlebars.compile($("#session_error_template").html());

  session_settings_template = Handlebars.compile($("#session_settings_template").html());

  print_error = function(text) {
    return display_error("session_error", session_error_template, text);
  };

  pop_settings = function(ui, data) {
    var it, mod, n, _i, _j, _len, _len1;
    ui.html(session_settings_template(data));
    n = ["#checkpoints_frequency", "#checkpoints_keep", "#time_step", "#time_factor", "#time_end_tick", "#rng_seed", "#pathfind_threads", "#recorder_frequency"];
    for (_i = 0, _len = n.length; _i < _len; _i++) {
      it = n[_i];
      force_input_regexp(/\d/, ui.find(it));
    }
    n = ["#rng_fire_mean", "#rng_fire_deviation", "#rng_wound_mean", "#rng_wound_deviation", "#rng_perception_mean", "#rng_perception_deviation", "#rng_breakdown_mean", "#rng_breakdown_deviation"];
    for (_j = 0, _len1 = n.length; _j < _len1; _j++) {
      it = n[_j];
      force_input_regexp(/[\d.]/, ui.find(it));
    }
    attach_checkbox_and_input($("#time_end_tick"), $("#time_end_tick_check"));
    attach_checkbox_and_input($("#rng_seed"), $("#rng_seed_check"));
    mod = ui.find(".modal");
    mod.modal("show");
    return [ui, mod];
  };

  check_value = function(root, ui, cond, tab, msg) {
    if (cond) {
      return true;
    }
    root.find('a[href="#' + tab + '"]').tab('show');
    toggle_input_error(ui, msg);
    return false;
  };

  validate_number = function(data, key, ui, id, min, max, tab, msg) {
    var val, widget;
    widget = ui.find(id);
    val = get_number(widget);
    if (!check_value(ui, widget, is_clipped(val, min, max), tab, msg)) {
      return false;
    }
    data[key] = val;
    return true;
  };

  validate_double = function(data, key, ui, id, min, max, tab, msg) {
    var val, widget;
    widget = ui.find(id);
    val = get_double(widget);
    if (!check_value(ui, widget, (val != null) && is_clipped(val, min, max), tab, msg)) {
      return false;
    }
    data[key] = val;
    return true;
  };

  validate_rng = function(data, ui, type) {
    var dist, tab;
    tab = ui.find('a[href="#pill_' + type + '"]');
    dist = ui.find("#rng_" + type + "_distribution option:selected");
    data.distribution = dist.val();
    if (dist.val() === "linear") {
      return true;
    }
    if (!validate_double(data, "deviation", ui, "#rng_" + type + "_deviation", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid")) {
      tab.tab('show');
      return false;
    }
    if (!validate_double(data, "mean", ui, "#rng_" + type + "_mean", 1e-32, Number.MAX_VALUE, "tab_rng", "Invalid")) {
      tab.tab('show');
      return false;
    }
    return true;
  };

  validate_settings = function(ui) {
    var child, data, it, name, next, _i, _len, _ref;
    data = {};
    name = ui.find("#name");
    if (!check_value(ui, name, name.val().length, "tab_general", "Missing")) {
      return;
    }
    data.name = name.val();
    next = data.checkpoints = {};
    if (!validate_number(next, "frequency", ui, "#checkpoints_frequency", 1, 100, "tab_checkpoints", "[1, 100] Only")) {
      return;
    }
    if (!validate_number(next, "keep", ui, "#checkpoints_keep", 1, 100, "tab_checkpoints", "[1, 100] Only")) {
      return;
    }
    next.enabled = ui.find("#checkpoints_enabled").is(":checked");
    next = data.time = {};
    if (!validate_number(next, "step", ui, "#time_step", 1, Number.MAX_VALUE, "tab_time", "Invalid")) {
      return;
    }
    if (!validate_number(next, "factor", ui, "#time_factor", 1, Number.MAX_VALUE, "tab_time", "Invalid")) {
      return;
    }
    if (!validate_number(next, "end_tick", ui, "#time_end_tick", 0, Number.MAX_VALUE, "tab_time", "Invalid")) {
      return;
    }
    next.paused = ui.find("#time_paused").is(":checked");
    next = data.rng = {};
    if (!validate_number(next, "seed", ui, "#rng_seed", 0, Number.MAX_VALUE, "tab_rng", "Invalid")) {
      return;
    }
    _ref = ["fire", "wound", "perception", "breakdown"];
    for (_i = 0, _len = _ref.length; _i < _len; _i++) {
      it = _ref[_i];
      child = next[it] = {};
      if (!validate_rng(child, ui, it)) {
        return;
      }
    }
    next = data.pathfind = {};
    if (!validate_number(next, "threads", ui, "#pathfind_threads", 0, 8, "tab_advanced", "[0, 8] Only")) {
      return;
    }
    next = data.recorder = {};
    if (!validate_number(next, "frequency", ui, "#recorder_frequency", 1, Number.MAX_VALUE, "Invalid")) {
      return;
    }
    return data;
  };

  SessionItem = (function(_super) {

    __extends(SessionItem, _super);

    function SessionItem() {
      this.sync = __bind(this.sync, this);
      return SessionItem.__super__.constructor.apply(this, arguments);
    }

    SessionItem.prototype.view = SessionItemView;

    SessionItem.prototype.sync = function(method, model, options) {
      var cfg_attributes, data;
      cfg_attributes = ["name", "time", "rng", "checkpoints", "pathfind", "recorder"];
      if (method === "create") {
        data = select_attributes(model.attributes, cfg_attributes);
        data = flatten_item(data);
        data.node = uuid;
        data.exercise = model.attributes.exercise;
        return ajax("/api/create_session", data, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_session", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "update") {
        data = select_attributes(model.attributes, cfg_attributes);
        data = flatten_item(data);
        data.id = model.id;
        return ajax("/api/update_session", data, options.success, options.error);
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
      return SessionList.__super__.constructor.apply(this, arguments);
    }

    SessionList.prototype.model = SessionItem;

    SessionList.prototype.order = "name";

    SessionList.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_sessions", {
          node: uuid
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    SessionList.prototype.comparator = function(lhs, rhs) {
      if (this.order === "status") {
        return this.status_compare(lhs, rhs);
      }
      return this.name_compare(lhs, rhs);
    };

    SessionList.prototype.name_compare = function(lhs, rhs) {
      return text_compare(lhs.get("name"), rhs.get("name"));
    };

    SessionList.prototype.status_compare = function(lhs, rhs) {
      var a, b;
      a = lhs.get("status");
      b = rhs.get("status");
      if (a === b) {
        return this.name_compare(lhs, rhs);
      }
      return status_order[a] - status_order[b];
    };

    SessionList.prototype.set_order = function(order) {
      this.order = order;
      return this.sort();
    };

    return SessionList;

  })(Backbone.Collection);

  contains = function(txt, value) {
    return txt.toLowerCase().indexOf(value) >= 0;
  };

  SessionItemView = (function(_super) {

    __extends(SessionItemView, _super);

    function SessionItemView() {
      this.clone = __bind(this.clone, this);

      this.edit = __bind(this.edit, this);

      this.set_search = __bind(this.set_search, this);

      this.toggle_load = __bind(this.toggle_load, this);

      this.set_filter = __bind(this.set_filter, this);

      this.download = __bind(this.download, this);

      this.restore = __bind(this.restore, this);

      this.archive = __bind(this.archive, this);

      this.pause = __bind(this.pause, this);

      this.play = __bind(this.play, this);

      this.stop = __bind(this.stop, this);

      this.modify = __bind(this.modify, this);

      this["delete"] = __bind(this["delete"], this);

      this.render = __bind(this.render, this);

      this.is_search = __bind(this.is_search, this);
      return SessionItemView.__super__.constructor.apply(this, arguments);
    }

    SessionItemView.prototype.tagName = "div";

    SessionItemView.prototype.className = "row";

    SessionItemView.prototype.filters = [];

    SessionItemView.prototype.search = null;

    SessionItemView.prototype.initialize = function(obj) {
      this.model.bind('change', this.render);
      this.filters = obj.filters;
      this.search = obj.search;
      return this.render();
    };

    SessionItemView.prototype.events = {
      "click .archive": "archive",
      "click .clone": "clone",
      "click .delete": "delete",
      "click .download": "download",
      "click .edit": "edit",
      "click .pause": "pause",
      "click .play": "play",
      "click .restore": "restore",
      "click .stop": "stop"
    };

    SessionItemView.prototype.is_search = function() {
      if (contains(this.model.get("name"), this.search)) {
        return true;
      }
      if (contains(this.model.get("exercise").name, this.search)) {
        return true;
      }
      return false;
    };

    SessionItemView.prototype.render = function() {
      var current, data, duration, filter, it, start, _i, _j, _len, _len1, _ref, _ref1, _ref2;
      $(this.el).empty();
      _ref = this.filters;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        filter = _ref[_i];
        if (filter === this.model.get("status")) {
          return;
        }
      }
      if (this.search && !this.is_search()) {
        return;
      }
      data = $.extend({}, this.model.attributes);
      if ((_ref1 = data.start_time) != null ? _ref1.length : void 0) {
        start = new Date(data.start_time);
        current = new Date(data.current_time);
        duration = current.getTime() - start.getTime();
        data.start_time = start.toUTCString();
        data.duration = ms_to_duration(duration);
      }
      $(this.el).html(session_template(data));
      set_spinner($(this.el).find(".session_top_right .spin_btn"));
      _ref2 = $(this.el).find(".link");
      for (_j = 0, _len1 = _ref2.length; _j < _len1; _j++) {
        it = _ref2[_j];
        $(it).attr("href", "sword://" + window.location.hostname + ":" + this.model.get("port") + "/");
      }
    };

    SessionItemView.prototype["delete"] = function() {
      var _this = this;
      this.toggle_load();
      return this.model.destroy({
        wait: true,
        error: function() {
          return print_error("Unable to delete session " + _this.model.get("name"));
        }
      });
    };

    SessionItemView.prototype.modify = function(cmd, data) {
      var _this = this;
      this.toggle_load();
      return ajax("/api/" + cmd + "_session", data, function(item) {
        _this.toggle_load();
        return _this.model.set(item);
      }, function() {
        print_error("Unable to " + cmd + " session " + _this.model.get("name"));
        return _this.toggle_load();
      });
    };

    SessionItemView.prototype.stop = function() {
      return this.modify("stop", {
        id: this.model.id
      });
    };

    SessionItemView.prototype.play = function(e) {
      var data, name;
      data = {
        id: this.model.id
      };
      name = $(e.currentTarget).attr("name");
      if (name != null ? name.length : void 0) {
        data.checkpoint = name;
      }
      return this.modify("start", data);
    };

    SessionItemView.prototype.pause = function() {
      return this.modify("pause", {
        id: this.model.id
      });
    };

    SessionItemView.prototype.archive = function() {
      return this.modify("archive", {
        id: this.model.id
      });
    };

    SessionItemView.prototype.restore = function() {
      return this.modify("restore", {
        id: this.model.id
      });
    };

    SessionItemView.prototype.download = function() {
      var uri;
      uri = get_url("/api/download_session?");
      uri += "node=" + uuid;
      uri += "&id=" + this.model.id;
      return window.location.href = uri;
    };

    SessionItemView.prototype.set_filter = function(list) {
      this.filters = list;
      return this.render();
    };

    SessionItemView.prototype.toggle_load = function() {
      var it, _i, _len, _ref, _results;
      _ref = $(this.el).find(".session_top_right .btn");
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        _results.push($(it).toggle());
      }
      return _results;
    };

    SessionItemView.prototype.set_search = function(item) {
      this.search = item;
      return this.render();
    };

    SessionItemView.prototype.edit = function(evt) {
      var data, mod, ui, _ref,
        _this = this;
      if (is_disabled(evt)) {
        return;
      }
      data = $.extend({}, this.model.attributes);
      data.checkpoints.frequency = Math.ceil(data.checkpoints.frequency / 60);
      _ref = pop_settings($("#settings"), data), ui = _ref[0], mod = _ref[1];
      return mod.find(".apply").click(function() {
        data = validate_settings(ui, _this.model);
        if (data == null) {
          return;
        }
        data.checkpoints.frequency *= 60;
        data.id = _this.model.id;
        mod.modal("hide");
        return _this.model.save(data, {
          wait: true,
          error: function() {
            return print_error("Unable to update session " + _this.model.get("name"));
          }
        });
      });
    };

    SessionItemView.prototype.clone = function() {
      var data;
      data = $.extend({}, this.model.attributes);
      data.exercise = data.exercise.name;
      delete data.id;
      return this.model.trigger('clone', data);
    };

    return SessionItemView;

  })(Backbone.View);

  get_filters = function() {
    return _.pluck($("#session_filters input:not(:checked)"), "name");
  };

  get_search = function() {
    return $(".session_search .search-query").val();
  };

  SessionListView = (function(_super) {

    __extends(SessionListView, _super);

    function SessionListView() {
      this.clone = __bind(this.clone, this);

      this.set_search = __bind(this.set_search, this);

      this.set_filter = __bind(this.set_filter, this);

      this.delta = __bind(this.delta, this);

      this.create = __bind(this.create, this);

      this.remove = __bind(this.remove, this);

      this.add = __bind(this.add, this);

      this.reset = __bind(this.reset, this);
      return SessionListView.__super__.constructor.apply(this, arguments);
    }

    SessionListView.prototype.el = $("#sessions");

    SessionListView.prototype.delta_period = 5000;

    SessionListView.prototype.initialize = function() {
      this.model = new SessionList;
      this.model.bind("add", this.add);
      this.model.bind("remove", this.remove);
      this.model.bind("reset", this.reset);
      this.model.bind("change", this.model.sort);
      this.model.bind("clone", this.clone);
      this.model.fetch({
        error: function() {
          return print_error("Unable to fetch sessions");
        }
      });
      return setTimeout(this.delta, this.delta_period);
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
        search: get_search(),
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
      return $("#" + item.id).parent().remove();
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
          update_model(_this.model, next);
          return setTimeout(_this.delta, _this.delta_period);
        },
        error: function() {
          print_error("Unable to fetch sessions");
          return setTimeout(_this.delta, _this.delta_period);
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

    SessionListView.prototype.set_search = function(value) {
      var it, _i, _len, _ref, _results;
      value = value.toLowerCase();
      _ref = this.model.models;
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        _results.push(it.view.set_search(value));
      }
      return _results;
    };

    SessionListView.prototype.clone = function(data) {
      var cur, it, max, regexp, tab, _i, _len, _ref;
      max = 1;
      regexp = new RegExp(data.name + " \\((\\d+)\\)");
      _ref = this.model.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        tab = regexp.exec(it.get("name"));
        if (!(tab != null ? tab.length : void 0)) {
          continue;
        }
        cur = parseInt(tab[1]);
        if (max <= cur) {
          max = cur + 1;
        }
      }
      data.name += " (" + max + ")";
      return this.create(data);
    };

    return SessionListView;

  })(Backbone.View);

  ExerciseListItem = (function(_super) {

    __extends(ExerciseListItem, _super);

    function ExerciseListItem() {
      this.sync = __bind(this.sync, this);
      return ExerciseListItem.__super__.constructor.apply(this, arguments);
    }

    ExerciseListItem.prototype.view = ExerciseListItemView;

    ExerciseListItem.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_exercises", {
          id: uuid
        }, function(resp, status, xhr) {
          return options.success({
            exercises: resp
          }, status, xhr);
        }, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return ExerciseListItem;

  })(Backbone.Model);

  ExerciseListItemView = (function(_super) {

    __extends(ExerciseListItemView, _super);

    function ExerciseListItemView() {
      this.delta = __bind(this.delta, this);

      this.render = __bind(this.render, this);
      return ExerciseListItemView.__super__.constructor.apply(this, arguments);
    }

    ExerciseListItemView.prototype.el = $("#exercises");

    ExerciseListItemView.prototype.delta_period = 5000;

    ExerciseListItemView.prototype.initialize = function() {
      this.model = new ExerciseListItem;
      this.model.bind("change", this.render);
      this.model.fetch({
        error: function() {
          return print_error("Unable to fetch exercises");
        }
      });
      return setTimeout(this.delta, this.delta_period);
    };

    ExerciseListItemView.prototype.render = function() {
      var it, _i, _len, _ref;
      $(this.el).children().remove().end();
      _ref = this.model.get("exercises");
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        $(this.el).append("<option>" + it + "</option>");
      }
    };

    ExerciseListItemView.prototype.delta = function() {
      var next,
        _this = this;
      next = new ExerciseListItem;
      return next.fetch({
        success: function() {
          _this.model.set(next.attributes);
          return setTimeout(_this.delta, _this.delta_period);
        },
        error: function() {
          print_error("Unable to fetch exercises");
          return setTimeout(_this.delta, _this.delta_period);
        }
      });
    };

    return ExerciseListItemView;

  })(Backbone.View);

  exercise_view = new ExerciseListItemView;

  session_view = new SessionListView;

  validate_input_session = function(control, result, error) {
    var group;
    if (!result) {
      group = control.parent().parent();
      group.addClass("error");
      setTimeout((function() {
        return group.removeClass("error");
      }), 3000);
      print_error(error);
      return false;
    }
    return true;
  };

  $("#session_create").click(function() {
    var exercise, name, _ref;
    name = $("#session_name");
    if (!validate_input_session(name, (_ref = name.val()) != null ? _ref.length : void 0, "Missing name")) {
      return;
    }
    exercise = $("#exercises");
    if (!validate_input_session(exercise, exercise.val() != null, "Missing exercise")) {
      return;
    }
    session_view.create({
      name: name.val(),
      exercise: exercise.val()
    });
    return name.val('');
  });

  $(".session_create_form").keypress(function(e) {
    if (e.which === 13) {
      return $("#session_create").click();
    }
  });

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

  $(".session_search input").keyup(function() {
    return session_view.set_search(get_search());
  });

  $(".session_search input").bind("input propertychange", function() {
    return session_view.set_search(get_search());
  });

}).call(this);
