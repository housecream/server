package net.awired.housecream.client.common.resource.client;

import javax.ws.rs.GET;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import net.awired.housecream.client.common.domain.pin.HccPinDescription;
import net.awired.housecream.client.common.resource.HccUpdateException;
import net.awired.housecream.client.common.resource.PinNotFoundException;

@Path("pin/{pinId}")
public interface HccPinResource {

    @GET
    HccPinDescription getPinDescription(@PathParam("pinId") int pinId) throws PinNotFoundException;

    @GET
    @Path("/value")
    Float getValue(@PathParam("pinId") int pinId) throws PinNotFoundException;

    @PUT
    @Path("/value")
    void setValue(@PathParam("pinId") int pinId, Float value) throws PinNotFoundException, HccUpdateException;
}