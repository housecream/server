"use strict";
///<amd-dependency path="test/specs/helpers" name="helpers" />
Object.defineProperty(exports, "__esModule", { value: true });
var common_1 = require("test/lib/common");
var moment_1 = require("moment");
var index_pattern_1 = require("../index_pattern");
common_1.describe('IndexPattern', function () {
    common_1.describe('when getting index for today', function () {
        common_1.it('should return correct index name', function () {
            var pattern = new index_pattern_1.default('[asd-]YYYY.MM.DD', 'Daily');
            var expected = 'asd-' + moment_1.default.utc().format('YYYY.MM.DD');
            common_1.expect(pattern.getIndexForToday()).to.be(expected);
        });
    });
    common_1.describe('when getting index list for time range', function () {
        common_1.describe('no interval', function () {
            common_1.it('should return correct index', function () {
                var pattern = new index_pattern_1.default('my-metrics');
                var from = new Date(2015, 4, 30, 1, 2, 3);
                var to = new Date(2015, 5, 1, 12, 5, 6);
                common_1.expect(pattern.getIndexList(from, to)).to.eql('my-metrics');
            });
        });
        common_1.describe('daily', function () {
            common_1.it('should return correct index list', function () {
                var pattern = new index_pattern_1.default('[asd-]YYYY.MM.DD', 'Daily');
                var from = new Date(1432940523000);
                var to = new Date(1433153106000);
                var expected = [
                    'asd-2015.05.29',
                    'asd-2015.05.30',
                    'asd-2015.05.31',
                    'asd-2015.06.01',
                ];
                common_1.expect(pattern.getIndexList(from, to)).to.eql(expected);
            });
        });
    });
});
