"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var Observable_1 = require("../../Observable");
var concatMapTo_1 = require("../../operator/concatMapTo");
Observable_1.Observable.prototype.concatMapTo = concatMapTo_1.concatMapTo;
