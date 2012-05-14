(function() {
  var get_url, spin_opts, spinner, toggle_load, tr, uid, _i, _len, _ref, _ref2;

  get_url = function(url) {
    return window.location.protocol + "//" + window.location.hostname + ":" + proxy + url;
  };

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
    return toggle_load();
  });

}).call(this);
