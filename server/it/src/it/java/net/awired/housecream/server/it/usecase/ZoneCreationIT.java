package net.awired.housecream.server.it.usecase;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import javax.mail.internet.InternetHeaders;
import javax.ws.rs.core.MultivaluedMap;
import net.awired.housecream.server.api.resource.ZoneResource;
import net.awired.housecream.server.it.HcsItServer;
import org.apache.cxf.jaxrs.ext.multipart.Attachment;
import org.apache.cxf.jaxrs.ext.multipart.ContentDisposition;
import org.apache.cxf.jaxrs.ext.multipart.MultipartBody;
import org.apache.cxf.jaxrs.impl.MetadataMap;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;

public class ZoneCreationIT {
    @Rule
    public HcsItServer hcs = new HcsItServer();

    @Test
    public void should_create_zone() throws Exception {
        ZoneResource zoneResource = hcs.zoneResource();

        //        Land land = new Land();
        //        land.setName("landName");
        //        Long landId = zoneResource.createZone(land);

        //        File file = new File(DOC); 
        //        FileDataSource ds = new FileDataSource(file);

        //        List<Attachment> attachments = new ArrayList<Attachment>();
        InternetHeaders headers = new InternetHeaders();
        headers.addHeader("Content-Type", "application/pdf");
        headers.addHeader("Content-ID", "doc.pdf");
        headers.addHeader("Content-Transfer-Encoding", "base64");
        headers.addHeader("Content-Disposition", "filename=" + "123456789034567834567");

        //        Attachment attach = AttachmentUtil.createAttachment(ds.getInputStream(), headers);
        //        attachments.add(attach);

        //        BindingProvider bp = (BindingProvider) client;
        //        java.util.Map<String, Object> reqContext = bp.getRequestContext();
        //        reqContext.put(Message.ATTACHMENTS, attachments);

        //        InputStream resourceAsStream = getClass().getResourceAsStream("test.txt");
        //        Attachment attach = AttachmentUtil.createAttachment(resourceAsStream, headers);
        //        
        //        
        //        Collection<Attachment> attachments = message.getAttachments();
        //
        //        DataSource dataSource = new MyDataSource(myInputStream));
        //        attachments.add(new AttachmentImpl(attachmentId, new
        //        DataHandler(dataSource));

        List<Attachment> asList = new ArrayList<Attachment>();
        //        asList.add(attach);
        //        zoneResource.upload(asList);

        //        assertNotNull(landId);
    }

    @Test
    @Ignore("need the upload part of form first")
    public void should_create_zone2() throws Exception {
        ZoneResource zoneResource = hcs.zoneResource();
        // TODO Auto-generated method stub
        InputStream resourceAsStream = getClass().getResourceAsStream("test.txt");
        ContentDisposition cd = new ContentDisposition("attachment;filename=java.jpg");
        MultivaluedMap<String, String> headers = new MetadataMap<String, String>();
        headers.putSingle("Content-ID", "image");
        headers.putSingle("Content-Disposition", cd.toString());
        headers.putSingle("Content-Location", "http://host/bar");
        headers.putSingle("custom-header", "custom");
        Attachment att = new Attachment(resourceAsStream, headers);

        MultipartBody body = new MultipartBody(att);
        zoneResource.upload(body);
        //        MultipartBody body2 = client.post(body, MultipartBody.class);
    }
}
