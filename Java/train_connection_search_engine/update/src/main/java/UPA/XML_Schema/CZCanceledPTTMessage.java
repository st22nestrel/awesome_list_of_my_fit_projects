package UPA.XML_Schema;

import UPA.XML_Schema.CZDeactivatedSection.CZDeactivatedSection;
import UPA.XML_Schema.CZPTTInformation.PlannedCalendar.PlannedCalendar;
import UPA.XML_Schema.Identifiers.TransportIdentifiers.TransportIdentifiers;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlElement;
import jakarta.xml.bind.annotation.XmlRootElement;

import java.util.ArrayList;
import java.util.List;

@Entity
@XmlRootElement
@XmlAccessorType(XmlAccessType.FIELD)
public class CZCanceledPTTMessage {
    @Id
    private int _id = this.hashCode();

    @Reference(lazy = true)
    @XmlElement(name = "PlannedTransportIdentifiers")
    public List<TransportIdentifiers> plannedTransportIdentifiers = new ArrayList<>();

    public String CZPTTCancelation;

    @XmlElement(name = "CZDeactivatedSection")
    public CZDeactivatedSection czDeactivatedSection;

    public PlannedCalendar plannedCalendar;
}
