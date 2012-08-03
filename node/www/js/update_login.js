// Generated by CoffeeScript 1.3.3
(function() {
  var ajax, bytes_to_size, diff_models, display_error, duration_reduce, force_input_regexp, get_ms_duration_from, get_url, is_disabled, ms_to_duration, pad, pajax, parse_parameters, raw_ajax, set_spinner, text_compare, toggle_input_error, transform_to_spinner, update_model;

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

  $(".cancel").click(function() {
    return $(".sign_out").click();
  });

  $("a.update_login").click(function() {
    var bis, cur, err, pwd;
    cur = $("#current");
    pwd = $("#password");
    bis = $("#password_bis");
    err = false;
    if (!cur.val().length) {
      toggle_input_error(cur, "Missing");
      err = true;
    }
    if (!pwd.val().length) {
      toggle_input_error(pwd, "Missing");
      err = true;
    }
    if (!bis.val().length) {
      toggle_input_error(bis, "Missing");
      err = true;
    }
    if (err) {
      return;
    }
    if (pwd.val() !== bis.val()) {
      toggle_input_error(pwd, "Invalid");
      toggle_input_error(bis, "Invalid");
      return;
    }
    return pajax("/api/update_login", {
      username: user.username,
      current: cur.val(),
      password: pwd.val()
    }, function(obj) {
      var first, k, uri, uri_params, v;
      uri = get_url(window.location.pathname);
      first = true;
      uri_params = parse_parameters();
      delete uri_params.sid;
      $.cookie("sid", obj.sid, {
        expires: 7,
        path: '/'
      });
      for (k in uri_params) {
        v = uri_params[k];
        uri += first ? "?" : "&";
        first = false;
        uri += k;
        if (v != null) {
          uri += "=" + v;
        }
      }
      uri += window.location.hash;
      return window.location.href = uri;
    }, function() {
      return toggle_input_error(cur, "Invalid");
    });
  });

  $("form.update_login").keypress(function(e) {
    if (e.which === 13) {
      return $("a.update_login").click();
    }
  });

}).call(this);
