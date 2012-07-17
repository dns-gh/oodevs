// Generated by CoffeeScript 1.3.3
(function() {
  var UserItem, UserItemView, UserList, UserListView, ajax, check_missing, diff_models, display_error, get_url, is_disabled, pajax, parse_parameters, pop_settings, print_error, raw_ajax, set_spinner, text_compare, toggle_input_error, transform_to_spinner, update_model, user_error_template, user_settings, user_template, user_view, validate_settings,
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

  user_settings = Handlebars.compile($("#user_settings_template").html());

  user_template = Handlebars.compile($("#user_template").html());

  user_error_template = Handlebars.compile($("#user_error_template").html());

  print_error = function(text) {
    return display_error("user_error", user_error_template, text);
  };

  pop_settings = function(data) {
    var mod, ui;
    ui = $("#user_settings");
    ui.html(user_settings(data));
    mod = ui.find(".modal");
    mod.addClass("hide fade");
    mod.modal("show");
    return [ui, mod];
  };

  check_missing = function(list) {
    var err, it, _i, _len;
    err = false;
    for (_i = 0, _len = list.length; _i < _len; _i++) {
      it = list[_i];
      if (!it.val().length) {
        toggle_input_error(it, "Missing");
        err = true;
      }
    }
    return err;
  };

  validate_settings = function(ui, add) {
    var bis, data, err, name, pwd, tmp, user, _ref;
    user = ui.find("#username");
    name = ui.find("#name");
    pwd = ui.find("#password");
    bis = ui.find("#password_bis");
    tmp = ui.find("#temporary");
    err = false;
    err |= check_missing([user]);
    err |= check_missing([name]);
    if (add != null) {
      err |= check_missing([pwd, bis]);
    }
    if ((add != null) && pwd.val() !== bis.val()) {
      toggle_input_error(pwd, "Invalid");
      toggle_input_error(bis, "Invalid");
      err = true;
    }
    if (err) {
      return;
    }
    data = {
      username: user.val(),
      name: name.val(),
      temporary: tmp.is(":checked"),
      type: "administrator"
    };
    if ((_ref = pwd.val()) != null ? _ref.length : void 0) {
      data.password = pwd.val();
    }
    return data;
  };

  UserItem = (function(_super) {

    __extends(UserItem, _super);

    function UserItem() {
      this.sync = __bind(this.sync, this);
      return UserItem.__super__.constructor.apply(this, arguments);
    }

    UserItem.prototype.view = UserItemView;

    UserItem.prototype.sync = function(method, model, options) {
      if (method === "create") {
        return pajax("/api/create_user", model.attributes, options.success, options.error);
      }
      if (method === "read") {
        return ajax("/api/get_user", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "delete") {
        return ajax("/api/delete_user", {
          id: model.id
        }, options.success, options.error);
      }
      if (method === "update") {
        return ajax("/api/update_user", model.attributes, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return UserItem;

  })(Backbone.Model);

  UserList = (function(_super) {

    __extends(UserList, _super);

    function UserList() {
      this.comparator = __bind(this.comparator, this);

      this.sync = __bind(this.sync, this);
      return UserList.__super__.constructor.apply(this, arguments);
    }

    UserList.prototype.model = UserItem;

    UserList.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/list_users", null, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    UserList.prototype.comparator = function(lhs, rhs) {
      if (lhs.id < rhs.id) {
        return -1;
      }
      if (lhs.id > rhs.id) {
        return 1;
      }
      return 0;
    };

    return UserList;

  })(Backbone.Collection);

  UserItemView = (function(_super) {

    __extends(UserItemView, _super);

    function UserItemView() {
      this.toggle_load = __bind(this.toggle_load, this);

      this.edit = __bind(this.edit, this);

      this["delete"] = __bind(this["delete"], this);

      this.render = __bind(this.render, this);
      return UserItemView.__super__.constructor.apply(this, arguments);
    }

    UserItemView.prototype.tagName = "tr";

    UserItemView.prototype.initialize = function(obj) {
      this.model.bind('change', this.render);
      return this.render();
    };

    UserItemView.prototype.events = {
      "click .delete": "delete",
      "click .edit": "edit"
    };

    UserItemView.prototype.render = function() {
      var spin, _ref;
      $(this.el).empty();
      $(this.el).attr("id", "id_" + this.model.id);
      $(this.el).html(user_template(this.model.attributes));
      spin = $("<a class='btn disabled spin_btn'></a>");
      set_spinner(spin);
      if ((_ref = $(this.el).find(".delete")) != null) {
        _ref.after(spin);
      }
      return spin.hide();
    };

    UserItemView.prototype["delete"] = function(evt) {
      var _this = this;
      if (is_disabled(evt)) {
        return;
      }
      this.toggle_load();
      return this.model.destroy({
        wait: true,
        error: function() {
          _this.toggle_load();
          return print_error("Unable to delete user " + _this.model.get("username"));
        }
      });
    };

    UserItemView.prototype.edit = function(evt) {
      var data, mod, other, ui, _ref,
        _this = this;
      if (is_disabled(evt)) {
        return;
      }
      other = $(this.el).find(".delete").length;
      data = $.extend({}, this.model.attributes);
      if (other) {
        data.other = true;
      }
      _ref = pop_settings(data), ui = _ref[0], mod = _ref[1];
      return ui.find(".add").click(function() {
        data = validate_settings(ui);
        if (data == null) {
          return;
        }
        _this.toggle_load();
        return _this.model.save(data, {
          wait: true,
          success: function() {
            if (other) {
              return mod.modal("hide");
            } else {
              mod.removeClass("fade");
              mod.modal("hide");
              return location.reload();
            }
          },
          error: function() {
            _this.toggle_load();
            mod.modal("hide");
            return print_error("Unable to update user " + _this.model.get("username"));
          }
        });
      });
    };

    UserItemView.prototype.toggle_load = function() {
      var it, _i, _len, _ref, _results;
      _ref = $(this.el).find(".btn");
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        _results.push($(it).toggle());
      }
      return _results;
    };

    return UserItemView;

  })(Backbone.View);

  UserListView = (function(_super) {

    __extends(UserListView, _super);

    function UserListView() {
      this.delta = __bind(this.delta, this);

      this.create = __bind(this.create, this);

      this.remove = __bind(this.remove, this);

      this.add = __bind(this.add, this);

      this.reset = __bind(this.reset, this);
      return UserListView.__super__.constructor.apply(this, arguments);
    }

    UserListView.prototype.el = $("#users");

    UserListView.prototype.initialize = function() {
      this.model = new UserList;
      this.model.bind('add', this.add);
      this.model.bind('remove', this.remove);
      this.model.bind('reset', this.reset);
      this.model.fetch({
        error: function() {
          return print_error("Unable to fetch users");
        }
      });
      return setTimeout(this.delta, 5000);
    };

    UserListView.prototype.reset = function(list, options) {
      var it, _i, _len, _ref;
      $(this.el).empty();
      _ref = list.models;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        this.add(it);
      }
    };

    UserListView.prototype.add = function(item) {
      var previous, view, _ref;
      view = new UserItemView({
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

    UserListView.prototype.remove = function(item, list, index) {
      return $("#id_" + item.id).remove();
    };

    UserListView.prototype.create = function(data) {
      var item,
        _this = this;
      item = new UserItem;
      item.set(data);
      return this.model.create(item, {
        wait: true,
        error: function() {
          return print_error("Unable to create user " + item.get("username"));
        }
      });
    };

    UserListView.prototype.delta = function() {
      var next,
        _this = this;
      next = new UserList;
      return next.fetch({
        success: function() {
          update_model(_this.model, next);
          return setTimeout(_this.delta, 5000);
        },
        error: function() {
          print_error("Unable to fetch users");
          return setTimeout(_this.delta, 5000);
        }
      });
    };

    return UserListView;

  })(Backbone.View);

  user_view = new UserListView;

  $("#user_create").click(function() {
    var mod, ui, _ref;
    _ref = pop_settings({
      add: true,
      other: true
    }), ui = _ref[0], mod = _ref[1];
    return ui.find(".add").click(function() {
      var data;
      data = validate_settings(ui, true);
      if (data == null) {
        return;
      }
      mod.modal("hide");
      return user_view.create(data);
    });
  });

}).call(this);
