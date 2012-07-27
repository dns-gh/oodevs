// Generated by CoffeeScript 1.3.3
(function() {
  var Package, PackageView, ajax, bytes_to_size, diff_models, display_error, duration_reduce, get_ms_duration_from, get_url, is_disabled, ms_to_duration, package_error_template, package_template, package_view, pajax, parse_parameters, print_error, raw_ajax, set_spinner, text_compare, toggle_input_error, transform_to_spinner, update_model,
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

  duration_reduce = function(ms, step, txt, suffix) {
    var n;
    if (ms < step) {
      return [ms, txt];
    }
    n = Math.floor(ms / step);
    txt += n + suffix + ' ';
    ms -= n * step;
    return [ms, txt];
  };

  ms_to_duration = function(ms) {
    var rpy, _ref, _ref1, _ref2, _ref3;
    _ref = duration_reduce(ms, 1000 * 60 * 60 * 24, "", 'd'), ms = _ref[0], rpy = _ref[1];
    _ref1 = duration_reduce(ms, 1000 * 60 * 60, rpy, 'h'), ms = _ref1[0], rpy = _ref1[1];
    _ref2 = duration_reduce(ms, 1000 * 60, rpy, 'm'), ms = _ref2[0], rpy = _ref2[1];
    _ref3 = duration_reduce(ms, 1000, rpy, 's'), ms = _ref3[0], rpy = _ref3[1];
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

      this.update = __bind(this.update, this);

      this.render = __bind(this.render, this);

      this.delete_items = __bind(this.delete_items, this);

      this["switch"] = __bind(this["switch"], this);
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

    PackageView.prototype["switch"] = function(next, reset) {
      this.enabled = next;
      if ((reset != null) && !next) {
        return $(this.el).empty();
      }
    };

    PackageView.prototype.delete_items = function(list) {
      var _this = this;
      return ajax("/api/delete_install", {
        id: uuid,
        items: list.join(',')
      }, function(item) {
        _this["switch"](true);
        return _this.update(item, true);
      }, function() {
        _this["switch"](true);
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
          var btn, items, _j, _len1, _ref1;
          items = [];
          _ref1 = $(_this.el).find(".action .delete");
          for (_j = 0, _len1 = _ref1.length; _j < _len1; _j++) {
            btn = _ref1[_j];
            _this["switch"](false);
            items.push($(btn).parent().attr("data-rel"));
          }
          if (items.length) {
            return _this.delete_items(items);
          }
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
          var id;
          _this["switch"](false);
          id = transform_to_spinner(e.data, true);
          return _this.delete_items([id]);
        });
      }
    };

    PackageView.prototype.update = function(data, buttons) {
      var it, _i, _len, _ref;
      if (buttons != null) {
        _ref = $(this.el).find(".btn");
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          it = _ref[_i];
          if ($(it).hasClass("spin_btn")) {
            $(it).remove();
          } else {
            $(it).show();
            $(it).removeClass("active");
          }
        }
      }
      return this.model.set(data);
    };

    PackageView.prototype.delta = function() {
      var list,
        _this = this;
      list = new Package;
      return list.fetch({
        success: function() {
          if (list.attributes.items != null) {
            _this.update(list.attributes);
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
