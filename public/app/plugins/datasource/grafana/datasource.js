/*! grafana - v4.2.0 - 2017-03-22
 * Copyright (c) 2017 Torkel Ödegaard; Licensed Apache-2.0 */

System.register(["lodash"],function(a,b){"use strict";var c,d;b&&b.id;return{setters:[function(a){c=a}],execute:function(){d=function(){function a(a){this.backendSrv=a}return a.$inject=["backendSrv"],a.prototype.query=function(a){return this.backendSrv.post("/api/tsdb/query",{from:a.range.from.valueOf().toString(),to:a.range.to.valueOf().toString(),queries:[{refId:"A",scenarioId:"random_walk",intervalMs:a.intervalMs,maxDataPoints:a.maxDataPoints}]}).then(function(a){var b=[];return a.results&&c.default.forEach(a.results,function(a){for(var c=0,d=a.series;c<d.length;c++){var e=d[c];b.push({target:e.name,datapoints:e.points})}}),{data:b}})},a.prototype.annotationQuery=function(a){return this.backendSrv.get("/api/annotations",{from:a.range.from.valueOf(),to:a.range.to.valueOf(),limit:a.limit,type:a.type})},a}(),a("GrafanaDatasource",d)}}});