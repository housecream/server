/**
 *
 *     Housecream.org project
 *     Copyright (C) Awired.net
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Affero General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Affero General Public License for more details.
 *
 *     You should have received a copy of the GNU Affero General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
package net.awired.housecream.server.application.config;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import javax.inject.Inject;
import net.awired.housecream.server.api.resource.InPointResource;
import net.awired.housecream.server.api.resource.InPointsResource;
import net.awired.housecream.server.api.resource.OutPointResource;
import net.awired.housecream.server.api.resource.OutPointsResource;
import net.awired.housecream.server.api.resource.RuleResource;
import net.awired.housecream.server.api.resource.RulesResource;
import net.awired.housecream.server.api.resource.UserResource;
import net.awired.housecream.server.api.resource.UsersResource;
import net.awired.housecream.server.api.resource.ZonesResource;
import net.awired.housecream.server.service.ResourceService;
import net.awired.jaxrs.client.server.resource.mapper.NotFoundExceptionMapper;
import net.awired.jaxrs.client.server.resource.mapper.UpdateExceptionMapper;
import net.awired.jaxrs.client.server.resource.mapper.ValidationExceptionMapper;
import net.awired.jaxrs.client.server.resource.mapper.generic.GenericExceptionMapper;
import net.awired.jaxrs.client.server.resource.mapper.generic.RuntimeExceptionMapper;
import net.awired.jaxrs.client.server.resource.mapper.generic.WebApplicationExceptionMapper;
import org.apache.cxf.bus.spring.SpringBus;
import org.apache.cxf.endpoint.Server;
import org.apache.cxf.interceptor.LoggingInInterceptor;
import org.apache.cxf.interceptor.LoggingOutInterceptor;
import org.apache.cxf.jaxrs.JAXRSServerFactoryBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ServiceConfig {

    @Inject
    private ResourceService resourceService;
    @Inject
    private InPointResource inPointService;
    @Inject
    private InPointsResource inPointsService;
    @Inject
    private OutPointResource outPointService;
    @Inject
    private OutPointsResource outPointsService;
    @Inject
    private RuleResource ruleService;
    @Inject
    private RulesResource rulesService;
    //    @Inject
    //    private ZoneResource zoneService;
    @Inject
    private ZonesResource zonesService;
    @Inject
    private UserResource userService;
    @Inject
    private UsersResource usersService;

    @Autowired
    private EncodingConfig encodingContext;

    @Bean(name = "cxf", destroyMethod = "shutdown")
    public SpringBus cxf() {
        return new SpringBus();
    }

    @Bean(name = "restRouter")
    public Server server() {
        JAXRSServerFactoryBean sf = new JAXRSServerFactoryBean();
        sf.getInInterceptors().add(new LoggingInInterceptor());
        sf.getOutInterceptors().add(new LoggingOutInterceptor());
        sf.setServiceBeanObjects(resourceService, //
                inPointService, //
                inPointsService, //
                outPointService, //
                outPointsService, //
                ruleService, //
                rulesService, //
                //                zoneService, //
                zonesService, //
                userService, //
                usersService);
        sf.setAddress("/");
        sf.setStaticSubresourceResolution(true);

        ArrayList<Object> arrayList = new ArrayList<>();
        arrayList.add(encodingContext.jacksonJsonProvider());
        arrayList.add(encodingContext.jAXBElementProvider());
        arrayList.add(new GenericExceptionMapper());
        arrayList.add(new RuntimeExceptionMapper());
        arrayList.add(new WebApplicationExceptionMapper());
        arrayList.add(new ValidationExceptionMapper());
        arrayList.add(new NotFoundExceptionMapper());
        arrayList.add(new UpdateExceptionMapper());
        sf.setProviders(Arrays.asList(encodingContext.jacksonJsonProvider(), encodingContext.jAXBElementProvider()));
        Map<Object, Object> extensions = new HashMap<>();
        extensions.put("json", "application/json");
        extensions.put("xml", "application/xml");
        sf.setExtensionMappings(extensions);
        return sf.create();
    }

}
