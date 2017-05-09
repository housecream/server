/*! grafana - v4.2.0 - 2017-03-22
 * Copyright (c) 2017 Torkel Ödegaard; Licensed Apache-2.0 */

System.register(["../../../../../test/lib/common","jquery","../graph_tooltip"],function(a,b){"use strict";function c(a,b){var c={};c.ctrl=g.ctrl,c.ctrl.panel={tooltip:{shared:!0},legend:{},stack:!1},c.setup=function(a){c.setupFn=a},d.describe(a,function(){d.beforeEach(function(){c.setupFn();var a=new f.default(h,i,g);c.results=a.getMultiSeriesPlotHoverInfo(c.data,c.pos)}),b(c)})}var d,e,f,g,h,i;b&&b.id;return{setters:[function(a){d=a},function(a){e=a},function(a){f=a}],execute:function(){g={appEvent:d.sinon.spy(),onAppEvent:d.sinon.spy(),ctrl:{}},h=e.default("<div></div>"),i={},d.describe("findHoverIndexFromData",function(){var a=new f.default(h,i,g),b={data:[[100,0],[101,0],[102,0],[103,0],[104,0],[105,0],[106,0],[107,0]]};d.it("should return 0 if posX out of lower bounds",function(){var c=99;d.expect(a.findHoverIndexFromData(c,b)).to.be(0)}),d.it("should return n - 1 if posX out of upper bounds",function(){var c=108;d.expect(a.findHoverIndexFromData(c,b)).to.be(b.data.length-1)}),d.it("should return i if posX in series",function(){var c=104;d.expect(a.findHoverIndexFromData(c,b)).to.be(4)}),d.it("should return i if posX not in series and i + 1 > posX",function(){var c=104.9;d.expect(a.findHoverIndexFromData(c,b)).to.be(4)})}),c("steppedLine false, stack false",function(a){a.setup(function(){a.data=[{data:[[10,15],[12,20]],lines:{}},{data:[[10,2],[12,3]],lines:{}}],a.pos={x:11}}),d.it("should return 2 series",function(){d.expect(a.results.length).to.be(2)}),d.it("should add time to results array",function(){d.expect(a.results.time).to.be(10)}),d.it("should set value and hoverIndex",function(){d.expect(a.results[0].value).to.be(15),d.expect(a.results[1].value).to.be(2),d.expect(a.results[0].hoverIndex).to.be(0)})}),c("one series is hidden",function(a){a.setup(function(){a.data=[{data:[[10,15],[12,20]]},{data:[]}],a.pos={x:11}})}),c("steppedLine false, stack true, individual false",function(a){a.setup(function(){a.data=[{data:[[10,15],[12,20]],lines:{},datapoints:{pointsize:2,points:[[10,15],[12,20]]},stack:!0},{data:[[10,2],[12,3]],lines:{},datapoints:{pointsize:2,points:[[10,2],[12,3]]},stack:!0}],a.ctrl.panel.stack=!0,a.pos={x:11}}),d.it("should show stacked value",function(){d.expect(a.results[1].value).to.be(17)})}),c("steppedLine false, stack true, individual false, series stack false",function(a){a.setup(function(){a.data=[{data:[[10,15],[12,20]],lines:{},datapoints:{pointsize:2,points:[[10,15],[12,20]]},stack:!0},{data:[[10,2],[12,3]],lines:{},datapoints:{pointsize:2,points:[[10,2],[12,3]]},stack:!1}],a.ctrl.panel.stack=!0,a.pos={x:11}}),d.it("should not show stacked value",function(){d.expect(a.results[1].value).to.be(2)})}),c("steppedLine false, stack true, individual true",function(a){a.setup(function(){a.data=[{data:[[10,15],[12,20]],lines:{},datapoints:{pointsize:2,points:[[10,15],[12,20]]},stack:!0},{data:[[10,2],[12,3]],lines:{},datapoints:{pointsize:2,points:[[10,2],[12,3]]},stack:!1}],a.ctrl.panel.stack=!0,a.ctrl.panel.tooltip.value_type="individual",a.pos={x:11}}),d.it("should not show stacked value",function(){d.expect(a.results[1].value).to.be(2)})})}}});