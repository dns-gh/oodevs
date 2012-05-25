(function() {
  var Package, PackageView, ajax, get_url, package_template, package_view, print_error, setSpinner, toggle_load, upload_error_template,
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = Object.prototype.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor; child.__super__ = parent.prototype; return child; };

  get_url = function(url) {
    return window.location.protocol + "//" + window.location.hostname + ":" + proxy + url;
  };

  ajax = function(url, data, success, error) {
    return $.ajax({
      cache: false,
      data: data,
      dataType: "json",
      error: error,
      success: success,
      url: get_url(url)
    });
  };

  Handlebars.registerHelper("equal", function(lhs, rhs, options) {
    if (lhs === rhs) return options.fn(this);
    return options.inverse(this);
  });

  package_template = Handlebars.compile($("#package_template").html());

  upload_error_template = Handlebars.compile($("#upload_error_template").html());

  print_error = function(text) {
    var ctl;
    ctl = $("#upload_error");
    ctl.html(upload_error_template({
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

  Package = (function(_super) {

    __extends(Package, _super);

    function Package() {
      this.sync = __bind(this.sync, this);
      Package.__super__.constructor.apply(this, arguments);
    }

    Package.prototype.view = PackageView;

    Package.prototype.sync = function(method, model, options) {
      if (method === "read") {
        return ajax("/api/get_pack", {
          id: uuid
        }, options.success, options.error);
      }
      return Backbone.sync(method, model, options);
    };

    return Package;

  })(Backbone.Model);

  setSpinner = function(btn) {
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

  PackageView = (function(_super) {

    __extends(PackageView, _super);

    function PackageView() {
      this.delta = __bind(this.delta, this);
      this.update = __bind(this.update, this);
      this.render = __bind(this.render, this);
      this["switch"] = __bind(this["switch"], this);
      PackageView.__super__.constructor.apply(this, arguments);
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
      if ((reset != null) && !next) return $(this.el).empty();
    };

    PackageView.prototype.render = function() {
      var it, _i, _j, _len, _len2, _ref, _ref2,
        _this = this;
      if (!this.enabled) return;
      $(this.el).empty();
      if (this.model.attributes.name != null) {
        $(this.el).html(package_template(this.model.attributes));
        _ref = $(this.el).find(".action .more");
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          it = _ref[_i];
          $(it).click(function() {
            return $("#" + $(this).parent().parent().attr("rel")).toggle("fast");
          });
        }
        _ref2 = $(this.el).find(".name .error");
        for (_j = 0, _len2 = _ref2.length; _j < _len2; _j++) {
          it = _ref2[_j];
          $(it).tooltip({
            placement: "right"
          });
        }
        $(".form-actions .discard").click(function() {
          _this["switch"](false, true);
          return ajax("/api/delete_pack", {
            id: uuid
          }, function() {
            return _this["switch"](true);
          }, function() {
            return _this["switch"](true);
          });
        });
        $(".form-actions .save").click(function() {
          var it, list, rel, spin, _k, _len3, _ref3;
          list = [];
          _ref3 = $(_this.el).find(".action .add, .action .update");
          for (_k = 0, _len3 = _ref3.length; _k < _len3; _k++) {
            it = _ref3[_k];
            if (!$(it).hasClass("active")) continue;
            _this["switch"](false);
            spin = $("<a class='btn disabled spin_btn'></a>");
            setSpinner(spin);
            spin.appendTo($(it).parent());
            $(it).hide();
            rel = $(it).parent().parent().attr("rel");
            rel = rel.replace(/^briefing_/, '');
            if (rel != null) list.push(rel);
          }
          if (!list.length) {
            print_error("Please select package(s) to save");
            return;
          }
          $("html, body").animate({
            scrollTop: 0
          }, "fast");
          return ajax("/api/update_pack", {
            id: uuid,
            packs: list.join(',')
          }, function(item) {
            _this["switch"](true);
            return _this.update(item, true);
          }, function() {
            _this["switch"](true);
            return print_error("Unable to save package(s)");
          });
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
      var item,
        _this = this;
      item = new Package;
      return item.fetch({
        success: function() {
          if (item.attributes.name != null) {
            _this.update(item.attributes);
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

  $("#upload_form").attr("action", (get_url("/api/upload_pack")) + "?id=" + uuid);

  setSpinner($(".spin_btn"));

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
    if ($(this).hasClass("disabled")) return;
    toggle_load();
    package_view["switch"](false, true);
    return $("#upload_form").submit();
  });

  $("#upload_target").load(function() {
    toggle_load();
    package_view["switch"](true);
    return package_view.update(jQuery.parseJSON($(this).contents().text()));
  });

}).call(this);
