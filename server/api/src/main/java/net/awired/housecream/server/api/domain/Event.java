package net.awired.housecream.server.api.domain;

import javax.xml.bind.annotation.XmlRootElement;
import com.google.common.base.Objects;

@XmlRootElement
public class Event {
    private long pointId;
    private float value;

    //    private Severity severity; // INFORM, (ERROR, WARNING), ALERT, DYNAMIC... 

    @Override
    public String toString() {
        return Objects.toStringHelper(this) //
                .add("pointId", pointId) //
                .add("value", value) //
                .toString();
    }

    public void setPointId(long pointId) {
        this.pointId = pointId;
    }

    public long getPointId() {
        return pointId;
    }

    public float getValue() {
        return value;
    }

    public void setValue(float value) {
        this.value = value;
    }
}