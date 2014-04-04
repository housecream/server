package org.housecream.server.it.server.it.floor;

import java.util.UUID;
import org.housecream.server.it.ItServer;
import org.junit.Rule;
import org.junit.Test;
import fr.norad.core.lang.exception.NotFoundException;

public class InPointIT {

    @Rule
    public ItServer hcs = new ItServer();

    @Test
    public void should_success_on_delete_not_existing_point() {
        UUID inPointId = UUID.randomUUID();
        hcs.session().inpoints().internalInpointResource(inPointId).deleteInPoint(inPointId);
    }

    @Test(expected = NotFoundException.class)
    public void should_not_found_not_exists_point() throws Exception {
        hcs.session().inpoints().getPoint(UUID.randomUUID());
    }
}
