(function() {
  var Package, PackageView, ajax, get_url, package_template, package_view, spin_opts, spinner, toggle_load, tr, uid, _i, _len, _ref, _ref2,
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

  package_template = Handlebars.compile($("#package_template").html());

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

  PackageView = (function(_super) {

    __extends(PackageView, _super);

    function PackageView() {
      this.delta = __bind(this.delta, this);
      this.update = __bind(this.update, this);
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
      $(this.el).empty();
      if (this.model.attributes.name != null) {
        $(this.el).html(package_template(this.model.attributes));
      }
    };

    PackageView.prototype.update = function(data) {
      return this.model.set(data);
    };

    PackageView.prototype.delta = function() {
      var item,
        _this = this;
      item = new Package;
      return item.fetch({
        success: function() {
          _this.update(item.attributes);
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

  $(".spin_btn").html(spinner.el);

  toggle_load = function() {
    $("#upload_form .upload").toggleClass("disabled");
    return $(".upload_alert").toggle();
  };

  $("#upload_form .upload").click(function() {
    if ($(this).hasClass("disabled")) return;
    toggle_load();
    return $("#upload_form").submit();
  });

  _ref = $(".exercises tr");
  for (_i = 0, _len = _ref.length; _i < _len; _i++) {
    tr = _ref[_i];
    if (!((_ref2 = tr.id) != null ? _ref2.length : void 0)) continue;
    uid = "#" + tr.id + "_briefing";
    $(tr).popover({
      placement: "bottom",
      title: $(uid + " h1:nth-child(2)").contents(),
      content: $(uid).contents()
    });
  }

  $("#upload_target").load(function(data) {
    toggle_load();
    return package_view.update(jQuery.parseJSON($(this).contents().find("body").html()));
  });

}).call(this);
