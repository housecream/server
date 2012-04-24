package net.awired.housecream.client.it.reserved;

import static org.junit.Assert.assertTrue;
import net.awired.housecream.client.HccTestRule;
import net.awired.housecream.client.common.domain.pin.HccPinDescription;
import net.awired.housecream.client.common.test.DefaultITDomainHelper;
import org.apache.commons.lang3.builder.EqualsBuilder;
import org.junit.Rule;
import org.junit.Test;

public class PinResourceReservedIT {

    private static final int PIN_ID = 0;

    @Rule
    public HccTestRule hcc = new HccTestRule();

    @Test
    public void should_get_valid_reserved_pin() throws Exception {

        HccPinDescription pin = hcc.getPinResource().getPinDescription(PIN_ID);

        assertTrue(EqualsBuilder.reflectionEquals(pin, DefaultITDomainHelper.buildDefaultPin(PIN_ID)
                .getDescription()));
    }
}
