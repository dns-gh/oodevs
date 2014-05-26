/* =========================================================
 * bootstrap-datepicker.js
 * http://www.eyecon.ro/bootstrap-datepicker
 * =========================================================
 * Copyright 2012 Stefan Petre
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================= */ (function ($) {

    // Picker object

    var Datepicker = function (element, options) {
        this.element = $(element);
        this.lang = moment.langData();
        this.format = options.format || this.element.data('date-format') || 'MM/DD/YYYY';
        this.picker = $(DPGlobal.template)
            .appendTo(element)
            .on({
            click: $.proxy(this.click, this) //,
            //mousedown: $.proxy(this.mousedown, this)
        });
        this.isInput = this.element.is('input');
        this.component = this.element.is('.date') ? this.element.find('.add-on') : false;

        if (this.isInput) {
            this.element.on({
                focus: $.proxy(this.show, this),
                //blur: $.proxy(this.hide, this),
                keyup: $.proxy(this.update, this)
            });
        } else {
            if (this.component) {
                this.component.on('click', $.proxy(this.show, this));
            } else {
                this.element.on('click', $.proxy(this.show, this));
            }
        }

        this.minViewMode = options.minViewMode || this.element.data('date-minviewmode') || 0;
        if (typeof this.minViewMode === 'string') {
            switch (this.minViewMode) {
                case 'months':
                    this.minViewMode = 1;
                    break;
                case 'years':
                    this.minViewMode = 2;
                    break;
                default:
                    this.minViewMode = 0;
                    break;
            }
        }
        this.viewMode = options.viewMode || this.element.data('date-viewmode') || 0;
        if (typeof this.viewMode === 'string') {
            switch (this.viewMode) {
                case 'months':
                    this.viewMode = 1;
                    break;
                case 'years':
                    this.viewMode = 2;
                    break;
                default:
                    this.viewMode = 0;
                    break;
            }
        }
        this.startViewMode = this.viewMode;
        this.weekStart = options.weekStart || this.element.data('date-weekstart') || 0;
        this.weekEnd = this.weekStart === 0 ? 6 : this.weekStart - 1;
        this.fillDow();
        this.fillMonths();
        this.update();
        this.showMode();
    };

    Datepicker.prototype = {
        constructor: Datepicker,

        show: function (e) {
            this.picker.show();
            this.height = this.component ? this.component.outerHeight() : this.element.outerHeight();
            this.place();
            $(window).on('resize', $.proxy(this.place, this));
            if (e) {
                e.stopPropagation();
                e.preventDefault();
            }
            var that = this;
            $(document).on('mousedown', function (ev) {
                if ($(ev.target).closest('.datepicker').length === 0) {
                    that.hide();
                }
            });
            this.element.trigger({
                type: 'show',
                date: this.date
            });
        },

        hide: function () {
            this.picker.hide();
            $(window).off('resize', this.place);
            this.viewMode = this.startViewMode;
            this.showMode();
            if (!this.isInput) {
                $(document).off('mousedown', this.hide);
            }
            //this.set();
            this.element.trigger({
                type: 'hide',
                date: this.date
            });
        },

        set: function () {
            var formated = this.date.format(this.format);
            if (!this.isInput) {
                if (this.component) {
                    this.element.find('input').prop('value', formated);
                }
                this.element.data('date', formated);
            } else {
                this.element.prop('value', formated);
            }
        },

        setValue: function (newDate) {
            var next;
            if (typeof newDate === 'string') {
                next = moment(newDate, this.format);
            } else {
                next = moment(newDate);
            }
            if (next.isSame(this.date)) {
                return;
            }
            this.date = next;
            this.set();
            this.viewDate = this.date.clone().startOf('month');
            this.fill();
        },

        place: function () {
            var offset = this.component ? this.component.offset() : this.element.offset();
            this.picker.css({
                top: offset.top + this.height,
                left: offset.left,
                position: 'fixed'
            });
        },

        update: function (newDate) {
            var el = typeof newDate === 'string' ? newDate : (this.isInput ? this.element.prop('value') : this.element.data('date'));
            this.setValue(el);
        },

        fillDow: function () {
            var dow = moment().day(this.weekStart);
            var html = '<tr>';
            var i = 0;
            while (i++ < 7) {
                html += '<th class="dow">' + this.lang.weekdaysMin(dow) + '</th>';
                dow.add('days', 1);
            }
            html += '</tr>';
            this.picker.find('.datepicker-days thead').append(html);
        },

        fillMonths: function () {
            var dow = moment().month(0);
            var html = '';
            var i = 0;
            while (i++ < 12) {
                html += '<span class="month">' + this.lang.monthsShort(dow) + '</span>';
                dow.add('months', 1);
            }
            this.picker.find('.datepicker-months td').append(html);
        },

        fill: function () {
            var view = this.viewDate.clone();
            var year = view.year();
            this.picker.find('.datepicker-days th:eq(1)')
                .text(this.lang.months(view) + ' ' + year);
            var cursor = view.clone().startOf('month').subtract('day', 1);
            while( cursor.day() !== this.weekStart )
                cursor.subtract('day', 1);
            var end = view.clone().endOf('month').add('day', 1);
            while( end.day() !== this.weekEnd || end.diff(cursor, 'day') < 40 )
                end.add('day', 1);
            var html = '';
            while (cursor.isBefore(end)) {
                var classes = ['day'];
                if (cursor.day() === this.weekStart) {
                    html += '<tr>';
                }
                if (cursor.isBefore(view, 'month')) {
                    classes.push('old');
                } else if (cursor.isAfter(view, 'month')) {
                    classes.push('new');
                }
                if (cursor.isSame(this.date, 'day')) {
                    classes.push('active');
                }
                html += '<td class="' + classes.join(' ') + '">' + cursor.format('DD') + '</td>';
                if (cursor.day() === this.weekEnd) {
                    html += '</tr>';
                }
                cursor.add('days', 1);
            }
            this.picker.find('.datepicker-days tbody').empty().append(html);
            var currentYear = this.date.year();
            var months = this.picker.find('.datepicker-months')
                .find('th:eq(1)')
                .text(year)
                .end()
                .find('span').removeClass('active');
            if (currentYear === year) {
                months.eq(this.date.month()).addClass('active');
            }

            html = '';
            year = parseInt(year / 10, 10) * 10;
            var yearCont = this.picker.find('.datepicker-years')
                .find('th:eq(1)')
                .text(year + '-' + (year + 9))
                .end()
                .find('td');
            year -= 1;
            for (var i = -1; i < 11; i++) {
                html += '<span class="year' + (i === -1 || i === 10 ? ' old' : '') + (currentYear === year ? ' active' : '') + '">' + year + '</span>';
                year += 1;
            }
            yearCont.html(html);
        },

        click: function (e) {
            var year, month, arg, val;
            e.stopPropagation();
            e.preventDefault();
            var target = $(e.target).closest('span, td, th');
            if (target.length === 1) {
                switch (target[0].nodeName.toLowerCase()) {
                    case 'th':
                        switch (target[0].className) {
                            case 'switch':
                                this.showMode(1);
                                break;
                            case 'prev':
                            case 'next':
                                val = target[0].className === 'prev' ? -1 : +1;
                                val = val * DPGlobal.modes[this.viewMode].navStep;
                                arg = {};
                                arg[DPGlobal.modes[this.viewMode].clsStep] = val;
                                this.viewDate.add(arg);
                                this.fill();
                                this.set();
                                break;
                        }
                        break;
                    case 'span':
                        if (target.is('.month')) {
                            month = target.parent().find('span').index(target);
                            this.viewDate.month(month);
                        } else {
                            year = parseInt(target.text(), 10) || 0;
                            this.viewDate.year(year);
                        }
                        if (this.viewMode !== 0) {
                            this.date = this.viewDate.clone();
                            this.element.trigger({
                                type: 'changeDate',
                                date: this.date,
                                viewMode: DPGlobal.modes[this.viewMode].clsName
                            });
                        }
                        this.showMode(-1);
                        this.fill();
                        this.set();
                        break;
                    case 'td':
                        if (target.is('.day') && !target.is('.disabled')) {
                            var day = parseInt(target.text(), 10) || 1;
                            month = this.viewDate.month();
                            if (target.is('.old')) {
                                month -= 1;
                            } else if (target.is('.new')) {
                                month += 1;
                            }
                            year = this.viewDate.year();
                            this.date = moment([year, month, day]);
                            this.viewDate = this.date.clone().startOf('month');
                            this.fill();
                            this.set();
                            this.element.trigger({
                                type: 'changeDate',
                                date: this.date,
                                viewMode: DPGlobal.modes[this.viewMode].clsName
                            });
                        }
                        break;
                }
            }
        },

        mousedown: function (e) {
            e.stopPropagation();
            e.preventDefault();
        },

        showMode: function (dir) {
            if (dir) {
                this.viewMode = Math.max(this.minViewMode, Math.min(2, this.viewMode + dir));
            }
            this.picker.find('>div').hide().filter('.datepicker-' + DPGlobal.modes[this.viewMode].clsName).show();
        }
    };

    $.fn.datepicker = function (option, val) {
        return this.each(function () {
            var $this = $(this),
                data = $this.data('datepicker'),
                options = typeof option === 'object' && option;
            if (!data) {
                $this.data('datepicker', (data = new Datepicker(this, $.extend({}, $.fn.datepicker.defaults, options))));
            }
            if (typeof option === 'string') data[option](val);
        });
    };

    $.fn.datepicker.defaults = {};

    $.fn.datepicker.Constructor = Datepicker;

    var DPGlobal = {
        modes: [{
                clsName: 'days',
                clsStep: 'month',
                navStep: 1
            }, {
                clsName: 'months',
                clsStep: 'year',
                navStep: 1
            }, {
                clsName: 'years',
                clsStep: 'year',
                navStep: 10
            }
        ],
        headTemplate: '<thead>' + '<tr>' + '<th class="prev">&lsaquo;</th>' + '<th colspan="5" class="switch"></th>' + '<th class="next">&rsaquo;</th>' + '</tr>' + '</thead>',
        contTemplate: '<tbody><tr><td colspan="7"></td></tr></tbody>'
    };
    DPGlobal.template = '<div class="datepicker dropdown-menu">' +
        '<div class="datepicker-days">' +
        '<table class=" table-condensed">' +
        DPGlobal.headTemplate +
        '<tbody></tbody>' +
        '</table>' +
        '</div>' +
        '<div class="datepicker-months">' +
        '<table class="table-condensed">' +
        DPGlobal.headTemplate +
        DPGlobal.contTemplate +
        '</table>' +
        '</div>' +
        '<div class="datepicker-years">' +
        '<table class="table-condensed">' +
        DPGlobal.headTemplate +
        DPGlobal.contTemplate +
        '</table>' +
        '</div>' +
        '</div>';

}(window.jQuery));
