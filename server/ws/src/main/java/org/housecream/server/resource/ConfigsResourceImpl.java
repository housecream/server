/**
 *
 *     Copyright (C) Housecream.org
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
package org.housecream.server.resource;


import org.housecream.server.api.domain.config.Config;
import org.housecream.server.api.resource.ConfigsResource;
import org.housecream.server.application.JaxRsResource;
import org.housecream.server.storage.dao.ConfigDao;
import org.springframework.beans.factory.annotation.Autowired;

@JaxRsResource
public class ConfigsResourceImpl implements ConfigsResource {
    @Autowired
    private ConfigResourceImpl configResource;

    @Autowired
    private Config config;

    @Override
    public Config getConfigs() {
        return config;
    }

    @Override
    public ConfigResource config(String name) {
        return configResource;
    }

    @JaxRsResource
    public static class ConfigResourceImpl implements ConfigResource {
        @Autowired
        private ConfigDao configDao;

        @Autowired
        private Config config;

        @Override
        public void setConfig(String name, String value) {
            configDao.saveConfig(name, value);
            config.setValue(name, value);
        }
    }
}