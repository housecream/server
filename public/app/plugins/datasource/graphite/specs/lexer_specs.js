/*! grafana - v4.2.0 - 2017-03-22
 * Copyright (c) 2017 Torkel Ödegaard; Licensed Apache-2.0 */

System.register(["test/lib/common","../lexer"],function(a,b){"use strict";var c,d;b&&b.id;return{setters:[function(a){c=a},function(a){d=a}],execute:function(){c.describe("when lexing graphite expression",function(){c.it("should tokenize metric expression",function(){var a=new d.Lexer("metric.test.*.asd.count"),b=a.tokenize();c.expect(b[0].value).to.be("metric"),c.expect(b[1].value).to.be("."),c.expect(b[2].type).to.be("identifier"),c.expect(b[4].type).to.be("identifier"),c.expect(b[4].pos).to.be(13)}),c.it("should tokenize metric expression with dash",function(){var a=new d.Lexer("metric.test.se1-server-*.asd.count"),b=a.tokenize();c.expect(b[4].type).to.be("identifier"),c.expect(b[4].value).to.be("se1-server-*")}),c.it("should tokenize metric expression with dash2",function(){var a=new d.Lexer("net.192-168-1-1.192-168-1-9.ping_value.*"),b=a.tokenize();c.expect(b[0].value).to.be("net"),c.expect(b[2].value).to.be("192-168-1-1")}),c.it("should tokenize metric expression with equal sign",function(){var a=new d.Lexer("apps=test"),b=a.tokenize();c.expect(b[0].value).to.be("apps=test")}),c.it("simple function2",function(){var a=new d.Lexer("offset(test.metric, -100)"),b=a.tokenize();c.expect(b[2].type).to.be("identifier"),c.expect(b[4].type).to.be("identifier"),c.expect(b[6].type).to.be("number")}),c.it("should tokenize metric expression with curly braces",function(){var a=new d.Lexer("metric.se1-{first, second}.count"),b=a.tokenize();c.expect(b.length).to.be(10),c.expect(b[3].type).to.be("{"),c.expect(b[4].value).to.be("first"),c.expect(b[5].value).to.be(","),c.expect(b[6].value).to.be("second")}),c.it("should tokenize metric expression with number segments",function(){var a=new d.Lexer("metric.10.12_10.test"),b=a.tokenize();c.expect(b[0].type).to.be("identifier"),c.expect(b[2].type).to.be("identifier"),c.expect(b[2].value).to.be("10"),c.expect(b[4].value).to.be("12_10"),c.expect(b[4].type).to.be("identifier")}),c.it("should tokenize metric expression with segment that start with number",function(){var a=new d.Lexer("metric.001-server"),b=a.tokenize();c.expect(b[0].type).to.be("identifier"),c.expect(b[2].type).to.be("identifier"),c.expect(b.length).to.be(3)}),c.it("should tokenize func call with numbered metric and number arg",function(){var a=new d.Lexer("scale(metric.10, 15)"),b=a.tokenize();c.expect(b[0].type).to.be("identifier"),c.expect(b[2].type).to.be("identifier"),c.expect(b[2].value).to.be("metric"),c.expect(b[4].value).to.be("10"),c.expect(b[4].type).to.be("number"),c.expect(b[6].type).to.be("number")}),c.it("should tokenize metric with template parameter",function(){var a=new d.Lexer("metric.[[server]].test"),b=a.tokenize();c.expect(b[2].type).to.be("identifier"),c.expect(b[2].value).to.be("[[server]]"),c.expect(b[4].type).to.be("identifier")}),c.it("should tokenize metric with question mark",function(){var a=new d.Lexer("metric.server_??.test"),b=a.tokenize();c.expect(b[2].type).to.be("identifier"),c.expect(b[2].value).to.be("server_??"),c.expect(b[4].type).to.be("identifier")}),c.it("should handle error with unterminated string",function(){var a=new d.Lexer("alias(metric, 'asd)"),b=a.tokenize();c.expect(b[0].value).to.be("alias"),c.expect(b[1].value).to.be("("),c.expect(b[2].value).to.be("metric"),c.expect(b[3].value).to.be(","),c.expect(b[4].type).to.be("string"),c.expect(b[4].isUnclosed).to.be(!0),c.expect(b[4].pos).to.be(20)}),c.it("should handle float parameters",function(){var a=new d.Lexer("alias(metric, 0.002)"),b=a.tokenize();c.expect(b[4].type).to.be("number"),c.expect(b[4].value).to.be("0.002")}),c.it("should handle bool parameters",function(){var a=new d.Lexer("alias(metric, true, false)"),b=a.tokenize();c.expect(b[4].type).to.be("bool"),c.expect(b[4].value).to.be("true"),c.expect(b[6].type).to.be("bool")})})}}});