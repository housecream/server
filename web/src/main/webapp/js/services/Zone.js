'use strict';

housecream.factory('Zone', function($resource, hcWsUrl) {
	var Zone = $resource(hcWsUrl + '/zone/:id', {id : '@id'});	
	return Zone;
});

housecream.factory('Zones', function($resource, hcWsUrl) {
	var Zones = $resource(hcWsUrl + '/zones');
	return Zones;
});
