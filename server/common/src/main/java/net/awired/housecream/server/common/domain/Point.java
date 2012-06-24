package net.awired.housecream.server.common.domain;

import javax.persistence.Column;
import javax.persistence.MappedSuperclass;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import net.awired.ajsl.persistence.entity.IdEntityImpl;

/**
 * A capture or command point (temperature, switch, ...)
 */
//@XmlRootElement
@XmlAccessorType(XmlAccessType.PROPERTY)
@MappedSuperclass
public abstract class Point extends IdEntityImpl<Long> {

    private static final long serialVersionUID = 1L;

    //    private Coordinate position;
    //    private List<Coordinate> coverage;

    @NotNull
    @Column(unique = true)
    private String name; // = "Interrupteur 42";
    //    private String description = "Eclairage principal de la piece";

    @NotNull
    @Column(unique = true)
    private String url;

    //    private Device device;

    ////////////////////////

    //    private Area area;

    ////////////////////

    public String getUrlPrefix() {
        if (url == null) {
            return null;
        }
        int indexOf = url.indexOf(':');
        if (indexOf == -1) {
            return null;
        }
        return url.substring(0, indexOf);
    }

    @Override
    @XmlElement
    public Long getId() {
        return super.getId();
    }

    @Override
    public void setId(Long id) {
        super.setId(id);
    }

    ///////////////////////////

    public void setName(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getUrl() {
        return url;
    }

}
