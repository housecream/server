"use strict";
///<reference path="../../../../headers/common.d.ts" />
Object.defineProperty(exports, "__esModule", { value: true });
var common_1 = require("../../../../../test/lib/common");
require("../module");
var angular_1 = require("angular");
var jquery_1 = require("jquery");
var helpers_1 = require("test/specs/helpers");
var time_series2_1 = require("app/core/time_series2");
var moment_1 = require("moment");
var core_1 = require("app/core/core");
common_1.describe('grafanaGraph', function () {
    common_1.beforeEach(common_1.angularMocks.module('grafana.core'));
    function graphScenario(desc, func, elementWidth) {
        if (elementWidth === void 0) { elementWidth = 500; }
        common_1.describe(desc, function () {
            var ctx = {};
            ctx.setup = function (setupFunc) {
                common_1.beforeEach(common_1.angularMocks.module(function ($provide) {
                    $provide.value("timeSrv", new helpers_1.default.TimeSrvStub());
                }));
                common_1.beforeEach(common_1.angularMocks.inject(function ($rootScope, $compile) {
                    var ctrl = {
                        events: new core_1.Emitter(),
                        height: 200,
                        panel: {
                            legend: {},
                            grid: {},
                            yaxes: [
                                {
                                    min: null,
                                    max: null,
                                    format: 'short',
                                    logBase: 1
                                },
                                {
                                    min: null,
                                    max: null,
                                    format: 'short',
                                    logBase: 1
                                }
                            ],
                            thresholds: [],
                            xaxis: {},
                            seriesOverrides: [],
                            tooltip: {
                                shared: true
                            }
                        },
                        renderingCompleted: common_1.sinon.spy(),
                        hiddenSeries: {},
                        dashboard: {
                            getTimezone: common_1.sinon.stub().returns('browser')
                        },
                        range: {
                            from: moment_1.default([2015, 1, 1, 10]),
                            to: moment_1.default([2015, 1, 1, 22]),
                        },
                    };
                    var scope = $rootScope.$new();
                    scope.ctrl = ctrl;
                    $rootScope.onAppEvent = common_1.sinon.spy();
                    ctx.data = [];
                    ctx.data.push(new time_series2_1.default({
                        datapoints: [[1, 1], [2, 2]],
                        alias: 'series1'
                    }));
                    ctx.data.push(new time_series2_1.default({
                        datapoints: [[1, 1], [2, 2]],
                        alias: 'series2'
                    }));
                    setupFunc(ctrl, ctx.data);
                    var element = angular_1.default.element("<div style='width:" + elementWidth + "px' grafana-graph><div>");
                    $compile(element)(scope);
                    scope.$digest();
                    jquery_1.default.plot = ctx.plotSpy = common_1.sinon.spy();
                    ctrl.events.emit('render', ctx.data);
                    ctx.plotData = ctx.plotSpy.getCall(0).args[1];
                    ctx.plotOptions = ctx.plotSpy.getCall(0).args[2];
                }));
            };
            func(ctx);
        });
    }
    graphScenario('simple lines options', function (ctx) {
        ctx.setup(function (ctrl) {
            ctrl.panel.lines = true;
            ctrl.panel.fill = 5;
            ctrl.panel.linewidth = 3;
            ctrl.panel.steppedLine = true;
        });
        common_1.it('should configure plot with correct options', function () {
            common_1.expect(ctx.plotOptions.series.lines.show).to.be(true);
            common_1.expect(ctx.plotOptions.series.lines.fill).to.be(0.5);
            common_1.expect(ctx.plotOptions.series.lines.lineWidth).to.be(3);
            common_1.expect(ctx.plotOptions.series.lines.steps).to.be(true);
        });
    });
    graphScenario('when logBase is log 10', function (ctx) {
        ctx.setup(function (ctrl) {
            ctrl.panel.yaxes[0].logBase = 10;
        });
        common_1.it('should apply axis transform and ticks', function () {
            var axis = ctx.plotOptions.yaxes[0];
            common_1.expect(axis.transform(100)).to.be(Math.log(100 + 0.1));
            common_1.expect(axis.ticks[0]).to.be(0);
            common_1.expect(axis.ticks[1]).to.be(1);
        });
    });
    graphScenario('should use timeStep for barWidth', function (ctx) {
        ctx.setup(function (ctrl, data) {
            ctrl.panel.bars = true;
            data[0] = new time_series2_1.default({
                datapoints: [[1, 10], [2, 20]],
                alias: 'series1',
            });
        });
        common_1.it('should set barWidth', function () {
            common_1.expect(ctx.plotOptions.series.bars.barWidth).to.be(1 / 1.5);
        });
    });
    graphScenario('series option overrides, fill & points', function (ctx) {
        ctx.setup(function (ctrl, data) {
            ctrl.panel.lines = true;
            ctrl.panel.fill = 5;
            data[0].zindex = 10;
            data[1].alias = 'test';
            data[1].lines = { fill: 0.001 };
            data[1].points = { show: true };
        });
        common_1.it('should match second series and fill zero, and enable points', function () {
            common_1.expect(ctx.plotOptions.series.lines.fill).to.be(0.5);
            common_1.expect(ctx.plotData[1].lines.fill).to.be(0.001);
            common_1.expect(ctx.plotData[1].points.show).to.be(true);
        });
    });
    graphScenario('should order series order according to zindex', function (ctx) {
        ctx.setup(function (ctrl, data) {
            data[1].zindex = 1;
            data[0].zindex = 10;
        });
        common_1.it('should move zindex 2 last', function () {
            common_1.expect(ctx.plotData[0].alias).to.be('series2');
            common_1.expect(ctx.plotData[1].alias).to.be('series1');
        });
    });
    graphScenario('when series is hidden', function (ctx) {
        ctx.setup(function (ctrl) {
            ctrl.hiddenSeries = { 'series2': true };
        });
        common_1.it('should remove datapoints and disable stack', function () {
            common_1.expect(ctx.plotData[0].alias).to.be('series1');
            common_1.expect(ctx.plotData[1].data.length).to.be(0);
            common_1.expect(ctx.plotData[1].stack).to.be(false);
        });
    });
    graphScenario('when stack and percent', function (ctx) {
        ctx.setup(function (ctrl) {
            ctrl.panel.percentage = true;
            ctrl.panel.stack = true;
        });
        common_1.it('should show percentage', function () {
            var axis = ctx.plotOptions.yaxes[0];
            common_1.expect(axis.tickFormatter(100, axis)).to.be("100%");
        });
    });
    graphScenario('when panel too narrow to show x-axis dates in same granularity as wide panels', function (ctx) {
        common_1.describe('and the range is less than 24 hours', function () {
            ctx.setup(function (ctrl) {
                ctrl.range.from = moment_1.default([2015, 1, 1, 10]);
                ctrl.range.to = moment_1.default([2015, 1, 1, 22]);
            });
            common_1.it('should format dates as hours minutes', function () {
                var axis = ctx.plotOptions.xaxis;
                common_1.expect(axis.timeformat).to.be('%H:%M');
            });
        });
        common_1.describe('and the range is less than one year', function () {
            ctx.setup(function (scope) {
                scope.range.from = moment_1.default([2015, 1, 1]);
                scope.range.to = moment_1.default([2015, 11, 20]);
            });
            common_1.it('should format dates as month days', function () {
                var axis = ctx.plotOptions.xaxis;
                common_1.expect(axis.timeformat).to.be('%m/%d');
            });
        });
    }, 10);
    // graphScenario('when using flexible Y-Min and Y-Max settings', function(ctx) {
    //   describe('and Y-Min is <100 and Y-Max is >200 and values within range', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '<100';
    //       ctrl.panel.yaxes[0].max = '>200';
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to 100 and max to 200', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(100);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(200);
    //     });
    //   });
    //   describe('and Y-Min is <100 and Y-Max is >200 and values outside range', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '<100';
    //       ctrl.panel.yaxes[0].max = '>200';
    //       data[0] = new TimeSeries({
    //         datapoints: [[99,10],[201,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to auto and max to auto', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(null);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(null);
    //     });
    //   });
    //   describe('and Y-Min is =10.5 and Y-Max is =10.5', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '=10.5';
    //       ctrl.panel.yaxes[0].max = '=10.5';
    //       data[0] = new TimeSeries({
    //         datapoints: [[100,10],[120,20], [110,30]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to last value + 10.5 and max to last value + 10.5', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(99.5);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(120.5);
    //     });
    //   });
    //   describe('and Y-Min is ~10.5 and Y-Max is ~10.5', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '~10.5';
    //       ctrl.panel.yaxes[0].max = '~10.5';
    //       data[0] = new TimeSeries({
    //         datapoints: [[102,10],[104,20], [110,30]], //Also checks precision
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to average value + 10.5 and max to average value + 10.5', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(94.8);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(115.8);
    //     });
    //   });
    // });
    // graphScenario('when using regular Y-Min and Y-Max settings', function(ctx) {
    //   describe('and Y-Min is 100 and Y-Max is 200', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '100';
    //       ctrl.panel.yaxes[0].max = '200';
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to 100 and max to 200', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(100);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(200);
    //     });
    //   });
    //   describe('and Y-Min is 0 and Y-Max is 0', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '0';
    //       ctrl.panel.yaxes[0].max = '0';
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to 0 and max to 0', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(0);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(0);
    //     });
    //   });
    //   describe('and negative values used', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = '-10';
    //       ctrl.panel.yaxes[0].max = '-13.14';
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min and max to negative', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(-10);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(-13.14);
    //     });
    //   });
    // });
    // graphScenario('when using Y-Min and Y-Max settings stored as number', function(ctx) {
    //   describe('and Y-Min is 0 and Y-Max is 100', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = 0;
    //       ctrl.panel.yaxes[0].max = 100;
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to 0 and max to 100', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(0);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(100);
    //     });
    //   });
    //   describe('and Y-Min is -100 and Y-Max is -10.5', function() {
    //     ctx.setup(function(ctrl, data) {
    //       ctrl.panel.yaxes[0].min = -100;
    //       ctrl.panel.yaxes[0].max = -10.5;
    //       data[0] = new TimeSeries({
    //         datapoints: [[120,10],[160,20]],
    //         alias: 'series1',
    //       });
    //     });
    //
    //     it('should set min to -100 and max to -10.5', function() {
    //        expect(ctx.plotOptions.yaxes[0].min).to.be(-100);
    //        expect(ctx.plotOptions.yaxes[0].max).to.be(-10.5);
    //     });
    //   });
    // });
});
