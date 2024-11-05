package UPA.XML_Schema.CZPTTInformation;

import UPA.XML_Schema.CZPTTCISMessage;
import UPA.XML_Schema.CZPTTInformation.CZPTTLocation.CZPTTLocation;
import UPA.XML_Schema.CZPTTInformation.PlannedCalendar.PlannedCalendar;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.Unmarshaller;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlElement;

import java.util.List;

//@XmlAccessorType(XmlAccessType.FIELD)
@Entity("Information")
public class CZPTTInformation {

    @Id
    private int _id;

    @Reference(lazy = true)
    public CZPTTCISMessage czpttcisMessage;

    @XmlElement(name = "CZPTTLocation")
    public List<CZPTTLocation> czpttLocations;

    @XmlElement(name = "PlannedCalendar")
    public PlannedCalendar plannedCalendar;

    void afterUnmarshal(Unmarshaller u, Object parent)
    {
        czpttcisMessage = ((CZPTTCISMessage) parent);
        this._id = this.hashCode();
    }
}
