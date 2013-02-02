package net.awired.housecream.camel.restmcu;

import static net.awired.restmcu.api.domain.line.RestMcuLineNotifyCondition.SUP_OR_EQUAL;
import net.awired.ajsl.test.RestServerRule;
import net.awired.restmcu.api.domain.board.RestMcuBoardNotification;
import net.awired.restmcu.api.domain.board.RestMcuBoardNotificationType;
import net.awired.restmcu.api.domain.line.RestMcuLine;
import net.awired.restmcu.api.domain.line.RestMcuLineDirection;
import net.awired.restmcu.api.domain.line.RestMcuLineNotification;
import net.awired.restmcu.api.domain.line.RestMcuLineNotify;
import net.awired.restmcu.api.resource.server.RestMcuNotifyResource;
import net.awired.restmcu.it.resource.LatchBoardResource;
import net.awired.restmcu.it.resource.LatchLineResource;
import org.apache.camel.EndpointInject;
import org.apache.camel.builder.RouteBuilder;
import org.apache.camel.component.mock.MockEndpoint;
import org.apache.camel.test.junit4.CamelTestSupport;
import org.junit.Rule;
import org.junit.Test;

public class RestMcuConsumerTest extends CamelTestSupport {

    @EndpointInject(uri = "mock:result")
    protected MockEndpoint result;

    @Rule
    public RestServerRule boardRule = new RestServerRule("http://0.0.0.0:5879", new LatchLineResource() {
        {
            LineInfo lineInfo = new LineInfo();
            lineInfo.value = 42f;
            lineInfo.description = new RestMcuLine();
            lineInfo.description.setDirection(RestMcuLineDirection.INPUT);
            lines.put(2, lineInfo);
        }
    }, new LatchBoardResource());

    @Test
    public void should_update_notification_url_and_receive_notif() throws Exception {
        result.expectedMinimumMessageCount(1);
        RestMcuLineNotification notif = new RestMcuLineNotification(2, 41f, 42f, "0.0.0.0:5879",
                new RestMcuLineNotify(SUP_OR_EQUAL, 1f));
        result.expectedBodiesReceived(notif);
        RestMcuNotifyResource notifyResource = boardRule.getResource(LatchBoardResource.class)
                .buildNotifyProxyFromNotifyUrl();

        notifyResource.lineNotification(notif);

        assertMockEndpointsSatisfied();
    }

    @Test
    public void should_receive_board_notification() throws Exception {
        result.expectedMinimumMessageCount(1);
        RestMcuBoardNotification notif = new RestMcuBoardNotification(RestMcuBoardNotificationType.BOOT);
        result.expectedBodiesReceived(notif);

        RestMcuNotifyResource notifyResource = boardRule.getResource(LatchBoardResource.class)
                .buildNotifyProxyFromNotifyUrl();

        notifyResource.boardNotification(notif);

        assertMockEndpointsSatisfied();
    }

    @Override
    protected RouteBuilder createRouteBuilder() throws Exception {
        return new RouteBuilder() {
            @Override
            public void configure() {
                from("restmcu://localhost:5879/2").to("mock:result");
            }
        };
    }

}
