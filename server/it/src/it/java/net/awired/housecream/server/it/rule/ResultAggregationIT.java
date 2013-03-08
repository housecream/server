package net.awired.housecream.server.it.rule;

import static net.awired.restmcu.api.domain.line.RestMcuLineDirection.INPUT;
import static net.awired.restmcu.api.domain.line.RestMcuLineDirection.OUTPUT;
import static net.awired.restmcu.api.domain.line.RestMcuLineNotifyCondition.SUP_OR_EQUAL;
import java.util.Date;
import net.awired.ajsl.test.RestServerRule;
import net.awired.housecream.server.api.domain.inpoint.InPoint;
import net.awired.housecream.server.api.domain.inpoint.InPointType;
import net.awired.housecream.server.api.domain.outPoint.OutPoint;
import net.awired.housecream.server.api.domain.outPoint.OutPointType;
import net.awired.housecream.server.api.domain.rule.Condition;
import net.awired.housecream.server.api.domain.rule.ConditionType;
import net.awired.housecream.server.api.domain.rule.Consequence;
import net.awired.housecream.server.api.domain.rule.EventRule;
import net.awired.housecream.server.api.domain.zone.Zone;
import net.awired.housecream.server.it.HcWsItServer;
import net.awired.housecream.server.it.builder.InPointBuilder;
import net.awired.housecream.server.it.builder.OutPointBuilder;
import net.awired.housecream.server.it.builder.zone.LandBuilder;
import net.awired.housecream.server.it.restmcu.NotifBuilder;
import net.awired.restmcu.api.domain.line.RestMcuLineNotification;
import net.awired.restmcu.it.builder.LineInfoBuilder;
import net.awired.restmcu.it.resource.LatchBoardResource;
import net.awired.restmcu.it.resource.LatchLineResource;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;

@Ignore
public class ResultAggregationIT {
    @Rule
    public HcWsItServer hcs = new HcWsItServer();

    @Rule
    public RestServerRule restmcu = new RestServerRule("http://localhost:5879/", new LatchBoardResource(),
            new LatchLineResource());

    @Test
    public void should_delay_the_consequence() throws Exception {
        LatchLineResource lineResource = restmcu.getResource(LatchLineResource.class);
        LatchBoardResource boardResource = restmcu.getResource(LatchBoardResource.class);

        lineResource.line(2, new LineInfoBuilder().direction(INPUT).value(1).build());
        lineResource.line(3, new LineInfoBuilder().direction(OUTPUT).value(1).build());
        lineResource.line(4, new LineInfoBuilder().direction(OUTPUT).value(1).build());

        Zone zone = hcs.zonesResource().createZone(new LandBuilder().name("land").build());

        // inpoint
        InPoint inPoint = new InPointBuilder().type(InPointType.PIR).name("my pir1").zoneId(zone.getId())
                .uri("restmcu://127.0.0.1:5879/2").build();
        inPoint = hcs.inPointsResource().createInPoint(inPoint);

        // outpoint
        OutPoint outPoint = new OutPointBuilder().name("my light1").type(OutPointType.LIGHT).zoneId(zone.getId())
                .uri("restmcu://127.0.0.1:5879/3").build();
        outPoint = hcs.outPointsResource().createOutPoint(outPoint);

        // outpoint
        OutPoint outPoint2 = new OutPointBuilder().name("my light2").type(OutPointType.LIGHT).zoneId(zone.getId())
                .uri("restmcu://127.0.0.1:5879/4").build();
        outPoint2 = hcs.outPointsResource().createOutPoint(outPoint2);

        // rule
        EventRule rule = new EventRule();
        rule.setName("my first rule");
        rule.getConditions().add(new Condition(inPoint.getId(), 1, ConditionType.event));
        rule.getConsequences().add(new Consequence(outPoint.getId(), 1));
        rule.getConsequences().add(new Consequence(outPoint2.getId(), 1));
        rule.getConsequences().add(new Consequence(outPoint2.getId(), 0, 5000));
        hcs.rulesResource().createRule(rule);

        RestMcuLineNotification pinNotif1 = new NotifBuilder().lineId(2).oldValue(0).value(1)
                .source("127.0.0.1:5879").notify(SUP_OR_EQUAL, 1).build();

        Date start = new Date();
        boardResource.buildNotifyProxyFromNotifyUrl().lineNotification(pinNotif1);

        Thread.sleep(10000);

        //        assertThat(lineResource.awaitLineValue(3)).isEqualTo(1);
        //        Date stop = new Date();
        //        assertThat(stop.getTime() - start.getTime()).isGreaterThan(5000).isLessThan(5200);
    }

}