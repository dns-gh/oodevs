(function() {
  var tr, uid, _i, _len, _ref, _ref2;

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

}).call(this);
