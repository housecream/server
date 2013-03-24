package net.awired.housecream.server.api.resource;

import java.util.List;
import javax.validation.Valid;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.QueryParam;
import net.awired.ajsl.persistence.entity.Order;
import net.awired.client.bean.validation.js.domain.ClientValidatorInfo;
import net.awired.housecream.server.api.domain.inpoint.InPoint;
import net.awired.housecream.server.api.domain.inpoint.InPointType;
import net.awired.housecream.server.api.domain.inpoint.InPoints;
import org.springframework.security.access.annotation.Secured;

@Path("/inpoints")
public interface InPointsResource {

    @GET
    @Path("/validator")
    public ClientValidatorInfo getInPointValidator();

    @POST
    InPoint createInPoint(@Valid InPoint inPoint) throws PluginNotFoundException;

    @GET
    @Secured("ROLE_ADMIN")
    InPoints getInPoints(@QueryParam("length") Integer length, //
            @QueryParam("start") Integer start, //
            @QueryParam("search") String search, //
            @QueryParam("searchProperty") List<String> searchProperties, //
            @QueryParam("order") List<Order> orders);

    @DELETE
    void deleteAllInPoints();

    @GET
    @Path("/types")
    List<InPointType> getInPointTypes();

}
