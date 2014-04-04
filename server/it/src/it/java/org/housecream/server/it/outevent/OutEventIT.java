package org.housecream.server.it.outevent;

import static org.fest.assertions.api.Assertions.assertThat;
import static org.housecream.restmcu.api.domain.line.RestMcuLineDirection.OUTPUT;
import static org.housecream.restmcu.it.builder.LineInfoBuilder.line;
import static org.housecream.server.api.domain.outPoint.OutPointType.LIGHT;
import org.housecream.restmcu.it.resource.LatchBoardResource;
import org.housecream.restmcu.it.resource.LatchLineResource;
import org.housecream.server.api.domain.outPoint.OutPoint;
import org.housecream.server.api.domain.zone.Land;
import org.housecream.server.it.ItServer;
import org.housecream.server.it.ItSession;
import org.junit.Rule;
import org.junit.Test;
import fr.norad.jaxrs.junit.RestServerRule;

public class OutEventIT {

    @Rule
    public ItServer hcs = new ItServer();

    private LatchBoardResource board = new LatchBoardResource("127.0.0.1:5879");
    private LatchLineResource line = new LatchLineResource() //
            .addLine(line(3).direction(OUTPUT).build());

    @Rule
    public RestServerRule restmcu = new RestServerRule("http://localhost:5879/", board, line);

    @Test
    public void should_send_output_value_synchronously() throws Exception {
        ItSession session = hcs.session();
        Land land = session.zones().createLand("landName");
        OutPoint light = session.outpoints().create("light1", land, LIGHT, "restmcu://127.0.0.1:5879/3");

        session.outpoints().setValue(light, 45f);

        assertThat(line.getLineValue(3)).isEqualTo(45f);
    }

}
