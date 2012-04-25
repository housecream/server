package net.awired.housecream.client.it;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import net.awired.housecream.client.HccTestRule;
import net.awired.housecream.client.common.domain.board.HccBoard;
import net.awired.housecream.client.common.resource.HccUpdateException;
import net.awired.housecream.client.common.test.DefaultITDomainHelper;
import org.apache.commons.lang3.builder.EqualsBuilder;
import org.junit.Rule;
import org.junit.Test;

public class HccResourceIT {

    @Rule
    public HccTestRule hcc = new HccTestRule();

    @Test
    public void should_reset_data() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setDescription("new Description");
        hcc.getHccResource().setBoard(deviceInfo);
        hcc.reset();

        HccBoard deviceInfo2 = hcc.getHccResource().getBoard();
        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo2, DefaultITDomainHelper.buildDefaultDevice()));
    }

    @Test
    public void should_get_root_resource() throws Exception {

        HccBoard deviceInfo = hcc.getHccResource().getBoard();

        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice()));
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_technical_description() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setTechnicalDescription("new Description");

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test
    public void should_update_description() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setDescription("new Description");

        HccBoard updateDevice = hcc.getHccResource().setBoard(deviceInfo);

        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(),
                "description"));
        assertEquals("new Description", updateDevice.getDescription());
        HccBoard device2 = hcc.getHccResource().getBoard();
        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(),
                "description"));
        assertEquals("new Description", device2.getDescription());
    }

    @Test
    public void should_update_name() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setName("new name");

        HccBoard updateDevice = hcc.getHccResource().setBoard(deviceInfo);

        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(), "name"));
        assertEquals("new name", updateDevice.getName());
        HccBoard device2 = hcc.getHccResource().getBoard();
        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(), "name"));
        assertEquals("new name", device2.getName());
    }

    @Test
    public void should_update_notifyurl() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setNotifyUrl("http://localhost:5353");

        HccBoard updateDevice = hcc.getHccResource().setBoard(deviceInfo);

        assertTrue(EqualsBuilder
                .reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(), "notifyUrl"));
        assertEquals("http://localhost:5353", updateDevice.getNotifyUrl());
        HccBoard device2 = hcc.getHccResource().getBoard();
        assertTrue(EqualsBuilder
                .reflectionEquals(deviceInfo, DefaultITDomainHelper.buildDefaultDevice(), "notifyUrl"));
        assertEquals("http://localhost:5353", device2.getNotifyUrl());
    }

    @Test
    public void should_be_able_to_update_all() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setNotifyUrl("http://localhost:5353");
        deviceInfo.setDescription("desc");
        deviceInfo.setName("GFDSQ");

        HccBoard updateDevice = hcc.getHccResource().setBoard(deviceInfo);

        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, updateDevice));
        HccBoard device2 = hcc.getHccResource().getBoard();
        assertTrue(EqualsBuilder.reflectionEquals(deviceInfo, device2));
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_ip() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setIp("23456789");

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_port() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setPort(4242);

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_mac() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setMac("ZERTYU");

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_number_of_pin() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setNumberOfPin(42);

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_software() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setSoftware("23456789");

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_update_version() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setVersion("23456789");

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_set_name_to_null() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setName(null);

        hcc.getHccResource().setBoard(deviceInfo);
    }

    @Test(expected = HccUpdateException.class)
    public void should_not_set_description_to_null() throws Exception {
        HccBoard deviceInfo = hcc.getHccResource().getBoard();
        deviceInfo.setDescription(null);

        hcc.getHccResource().setBoard(deviceInfo);
    }
}
