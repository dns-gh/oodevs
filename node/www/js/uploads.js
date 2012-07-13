// Generated by CoffeeScript 1.3.3
(function() {
  var Package, PackageView, ajax, diff_models, display_error, get_url, package_template, package_view, pajax, parse_parameters, print_error, raw_ajax, set_spinner, text_compare, toggle_input_error, toggle_load, transform_to_spinner, upload_error_template,
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

  toggle_input_error = function(el, txt) {
    var msg, reset_error, root;
    el.val('');
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

  package_template = Handlebars.compile($("#package_template").html());

  upload_error_template = Handlebars.compile($("#upload_error_template").html());

  print_error = function(text) {
    return display_error("upload_error", upload_error_template, text);
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
        return ajax("/api/get_cache", {
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

      this.reset = __bind(this.reset, this);
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

    PackageView.prototype.reset = function() {
      this.model.clear();
      return $(this.el).empty();
    };

    PackageView.prototype.render = function() {
      var discard, it, save, _i, _j, _len, _len1, _ref, _ref1,
        _this = this;
      if (!this.enabled) {
        return;
      }
      $(this.el).empty();
      if (this.model.attributes.name == null) {
        return;
      }
      $(this.el).html(package_template(this.model.attributes));
      _ref = $(this.el).find(".action .more");
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        $(it).click(function() {
          return $("#briefing_" + $(this).parent().attr("data-rel")).toggle("fast");
        });
      }
      _ref1 = $(this.el).find(".name .error");
      for (_j = 0, _len1 = _ref1.length; _j < _len1; _j++) {
        it = _ref1[_j];
        $(it).tooltip({
          placement: "top"
        });
      }
      discard = $(".form-actions .discard");
      save = $(".form-actions .save");
      discard.click(function() {
        if (discard.hasClass("disabled")) {
          return;
        }
        _this.enabled = false;
        _this.reset();
        return ajax("/api/delete_cache", {
          id: uuid
        }, function() {
          return _this.enabled = true;
        }, function() {
          return _this.enabled = true;
        });
      });
      save.click(function() {
        var id, list, _k, _len2, _ref2;
        if (save.hasClass("disabled")) {
          return;
        }
        list = [];
        _ref2 = $(_this.el).find(".action .add, .action .update");
        for (_k = 0, _len2 = _ref2.length; _k < _len2; _k++) {
          it = _ref2[_k];
          if (!$(it).hasClass("active")) {
            continue;
          }
          _this.enabled = false;
          discard.addClass("disabled");
          save.addClass("disabled");
          id = transform_to_spinner(it);
          if (id != null) {
            list.push(id);
          }
        }
        if (!list.length) {
          print_error("Please select at least one package to install");
          return;
        }
        return ajax("/api/install_from_cache", {
          id: uuid,
          items: list.join(',')
        }, function(item) {
          _this.reset();
          _this.enabled = true;
          return _this.model.set(item);
        }, function() {
          _this.reset();
          _this.enabled = true;
          return print_error("Unable to save package(s)");
        });
      });
      $(".toggle a").click(function() {
        var _k, _len2, _ref2, _results;
        _ref2 = $(".action .add, .action .update");
        _results = [];
        for (_k = 0, _len2 = _ref2.length; _k < _len2; _k++) {
          it = _ref2[_k];
          _results.push($(it).button("toggle"));
        }
        return _results;
      });
    };

    PackageView.prototype.delta = function() {
      var item,
        _this = this;
      item = new Package;
      return item.fetch({
        success: function() {
          if (item.attributes.name != null) {
            _this.model.set(item.attributes);
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

  $("#upload_form").attr("action", (get_url("/api/upload_cache")) + "?id=" + uuid);

  set_spinner($(".spin_btn"));

  $("#upload_form input:file").change(function() {
    var ctl;
    ctl = $("#upload_form .upload");
    if (this.value != null) {
      ctl.removeClass("disabled");
    } else {
      ctl.addClass("disabled");
    }
  });

  toggle_load = function() {
    $("#upload_form .upload").toggleClass("disabled");
    return $(".upload_alert").toggle();
  };

  $("#upload_form .upload").click(function() {
    if ($(this).hasClass("disabled")) {
      return;
    }
    toggle_load();
    package_view.enabled = false;
    package_view.reset();
    return $("#upload_form").submit();
  });

  $("#upload_target").load(function() {
    toggle_load();
    package_view.enabled = true;
    return package_view.model.set(jQuery.parseJSON($(this).contents().text()));
  });

}).call(this);
