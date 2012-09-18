// Generated by CoffeeScript 1.3.3
(function() {
  var Package, PackageView, ajax, attach_checkbox_and_input, bytes_to_size, convert_to_boolean, diff_models, display_error, duration_reduce, force_input_regexp, get_double, get_ms_duration_from, get_number, get_url, is_clipped, is_disabled, link_checkbox_to_input, load_url, modal_confirm, ms_to_duration, on_input_event, package_error_template, package_template, package_view, pad, pajax, parse_parameters, parse_scalar, print_error, raw_ajax, select_attributes, set_checkbox, set_spinner, text_compare, toggle_input_error, toggle_spinner, update_model,
    __slice = [].slice,
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
    var regexp;
    regexp = new RegExp("^" + rhs + "$");
    if (regexp.test(lhs)) {
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
      return load_url(get_url(window.location.pathname));
    }, function() {
      return load_url(get_url(window.location.pathname));
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

  modal_confirm = function() {
    var args, fn, msg, opts;
    opts = arguments[0], fn = arguments[1], args = 3 <= arguments.length ? __slice.call(arguments, 2) : [];
    msg = $("<div class=\"modal fade in modal_confirm\">\n    <div class=\"alert alert-error alert-block\">\n        <h4 class=\"alert-heading\">Warning</h4>\n        <p> " + opts.message + " </p>\n        <div class=\"buttons\">\n            <a class=\"btn btn-danger\">" + opts.accept + "</a>\n            <a class=\"btn\" data-dismiss=\"modal\">" + opts.reject + "</a>\n        </div>\n    </div>\n</div>");
    msg.modal("show");
    return msg.find(".btn-danger").click(function(e) {
      msg.modal("hide");
      return fn.apply(null, args);
    });
  };

  package_template = Handlebars.compile($("#package_template").html());

  package_error_template = Handlebars.compile($("#package_error_template").html());

  print_error = function(text) {
    return display_error("upload_error", package_error_template, text);
  };

  Package = (function(_super) {

    __extends(Package, _super);

    function Package() {
      this.sync = __bind(this.sync, this);
      return Package.__super__.constructor.apply(this, arguments);
    }

    Package.prototype.view = PackageView;

    Package.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/get_install", {
          id: uuid
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return Package;

  })(Backbone.Model);

  PackageView = (function(_super) {

    __extends(PackageView, _super);

    function PackageView() {
      this.delta = __bind(this.delta, this);

      this.render = __bind(this.render, this);

      this.delete_items = __bind(this.delete_items, this);

      this.toggle_load = __bind(this.toggle_load, this);
      return PackageView.__super__.constructor.apply(this, arguments);
    }

    PackageView.prototype.el = $("#packages");

    PackageView.prototype.enabled = true;

    PackageView.prototype.initialize = function(obj) {
      this.model = new Package;
      this.model.bind('change', this.render);
      this.model.fetch();
      return setTimeout(this.delta, 5000);
    };

    PackageView.prototype.toggle_load = function(enabled, group) {
      if (!enabled) {
        this.enabled = enabled;
      }
      if (group != null) {
        toggle_spinner($(group));
      }
      return this.enabled = enabled;
    };

    PackageView.prototype.delete_items = function(list) {
      var it, next, _i, _len,
        _this = this;
      next = [];
      for (_i = 0, _len = list.length; _i < _len; _i++) {
        it = list[_i];
        next.push($(it).parent().attr("data-rel"));
      }
      return ajax("/api/delete_install", {
        id: uuid,
        items: next.join(',')
      }, function(item) {
        _this.toggle_load(true, $(list));
        return _this.model.set(item);
      }, function() {
        _this.toggle_load(true, $(list));
        return print_error("Unable to delete package(s)");
      });
    };

    PackageView.prototype.render = function() {
      var it, _i, _j, _k, _l, _len, _len1, _len2, _len3, _ref, _ref1, _ref2, _ref3,
        _this = this;
      if (!this.enabled) {
        return;
      }
      $(this.el).empty();
      if (this.model.attributes.items == null) {
        return;
      }
      $(this.el).html(package_template(this.model.attributes));
      _ref = $(this.el).find(".package_header .remove_all a");
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        $(it).click(function() {
          return modal_confirm({
            message: "Are you sure you want to delete all packages?",
            accept: "Delete",
            reject: "Cancel"
          }, function() {
            var btn, items, _j, _len1, _ref1;
            items = [];
            _ref1 = $(_this.el).find(".action .delete");
            for (_j = 0, _len1 = _ref1.length; _j < _len1; _j++) {
              btn = _ref1[_j];
              _this.toggle_load(false, btn);
              items.push(btn);
            }
            if (items.length) {
              return _this.delete_items(items);
            }
          });
        });
      }
      _ref1 = $(this.el).find(".action .more");
      for (_j = 0, _len1 = _ref1.length; _j < _len1; _j++) {
        it = _ref1[_j];
        $(it).click(function() {
          return $("#briefing_" + $(this).parent().attr("data-rel")).toggle("fast");
        });
      }
      _ref2 = $(this.el).find(".name .error");
      for (_k = 0, _len2 = _ref2.length; _k < _len2; _k++) {
        it = _ref2[_k];
        $(it).tooltip({
          placement: "top"
        });
      }
      _ref3 = $(this.el).find(".action .delete");
      for (_l = 0, _len3 = _ref3.length; _l < _len3; _l++) {
        it = _ref3[_l];
        $(it).click(it, function(e) {
          return modal_confirm({
            message: "Are you sure you want to delete this package?",
            accept: "Delete",
            reject: "Cancel"
          }, function() {
            _this.toggle_load(false, e.data);
            return _this.delete_items([e.data]);
          });
        });
      }
    };

    PackageView.prototype.delta = function() {
      var list,
        _this = this;
      list = new Package;
      return list.fetch({
        success: function() {
          if (list.attributes.items != null) {
            _this.model.set(list.attributes);
          } else {
            _this.model.clear();
          }
          return setTimeout(_this.delta, 5000);
        },
        error: function() {
          return setTimeout(_this.delta, 5000);
        }
      });
    };

    return PackageView;

  })(Backbone.View);

  package_view = new PackageView;

}).call(this);
