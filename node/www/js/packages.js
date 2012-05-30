(function() {
  var Package, PackageView, ajax, get_url, package_error_template, package_template, package_view,
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

  Handlebars.registerHelper("is_header", function(items, type, options) {
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
      if (it.type === type) return options.fn(this);
    }
    return options.inverse(this);
  });

  Handlebars.registerHelper("is_odd_row", function(items, type, options) {
    var idx, it, types, _i, _len;
    types = {};
    idx = 0;
    for (_i = 0, _len = items.length; _i < _len; _i++) {
      it = items[_i];
      if (it.type === type) break;
      if (types[it.type] == null) ++idx;
      types[it.type] = true;
      ++idx;
    }
    if (idx & 1) return options.fn(this);
    return options.inverse(this);
  });

  Handlebars.registerHelper("forall", function(items, type, options) {
    var buffer, it, _i, _len;
    buffer = '';
    for (_i = 0, _len = items.length; _i < _len; _i++) {
      it = items[_i];
      if (it.type === type) buffer += options.fn(it);
    }
    return buffer;
  });

  package_template = Handlebars.compile($("#package_template").html());

  package_error_template = Handlebars.compile($("#package_error_template").html());

  Package = (function(_super) {

    __extends(Package, _super);

    function Package() {
      this.sync = __bind(this.sync, this);
      Package.__super__.constructor.apply(this, arguments);
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
      PackageView.__super__.constructor.apply(this, arguments);
    }

    PackageView.prototype.el = $("#packages");

    PackageView.prototype.initialize = function(obj) {
      this.model = new Package;
      this.model.bind('change', this.render);
      this.model.fetch();
      return setTimeout(this.delta, 5000);
    };

    PackageView.prototype.render = function() {
      var it, _i, _len, _ref;
      $(this.el).empty();
      if (this.model.attributes.items == null) return;
      $(this.el).html(package_template(this.model.attributes));
      _ref = $(this.el).find(".action .more");
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        it = _ref[_i];
        $(it).click(function() {
          return $("#" + $(this).parent().parent().attr("data-rel")).toggle("fast");
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
