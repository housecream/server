package net.awired.housecream.camel.restmcu;

import static net.awired.restmcu.it.builder.LineInfoBuilder.line;
import net.awired.ajsl.test.RestServerRule;
import net.awired.restmcu.api.domain.line.RestMcuLineDirection;
import net.awired.restmcu.it.resource.LatchBoardResource;
import net.awired.restmcu.it.resource.LatchLineResource;
import org.apache.camel.EndpointInject;
import org.apache.camel.ProducerTemplate;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.component.mock.MockEndpoint;
import org.apache.camel.test.junit4.CamelTestSupport;
import org.fest.assertions.api.Assertions;
import org.junit.Rule;
import org.junit.Test;

public class RestMcuProducerTest extends CamelTestSupport {

    @EndpointInject(uri = "mock:result")
    protected MockEndpoint result;

    private LatchLineResource line = new LatchLineResource() //
            .addLine(line(3).value(43f).direction(RestMcuLineDirection.OUTPUT).build());
    private LatchBoardResource board = new LatchBoardResource("127.0.0.1:5879");

    @Rule
    public RestServerRule boardRule = new RestServerRule("http://0.0.0.0:5879", line, board);

    @Test
    public void should_send_new_line_value() throws Exception {
        result.expectedMinimumMessageCount(1);
        ProducerTemplate template = context.createProducerTemplate();
        template.sendBody("direct:start", 42f);

        Assertions.assertThat(line.awaitLineValue(3)).isEqualTo(42);
        assertMockEndpointsSatisfied();
    }

    @Override
    protected RouteBuilder createRouteBuilder() throws Exception {
        return new RouteBuilder() {
            @Override
            public void configure() {
                from("direct:start").to("restmcu://localhost:5879/3").to("mock:result");
            }
        };
    }
}
