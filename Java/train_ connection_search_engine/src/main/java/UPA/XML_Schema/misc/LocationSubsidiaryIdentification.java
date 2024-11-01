package UPA.XML_Schema.misc;

import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.Unmarshaller;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlAttribute;

@Entity("Location-track-identification")
@XmlAccessorType(XmlAccessType.FIELD)
public class LocationSubsidiaryIdentification {
    @Id
    int _id;
    @Reference(lazy = true)
    Location LocationRef;

    String LocationSubsidiaryCode;

    @XmlAttribute
    String LocationSubsidiaryTypeCode;

    String AllocationCompany;

    String LocationSubsidiaryName;

    void afterUnmarshal(Unmarshaller u, Object parent)
    {
        this.LocationRef = ((Location) parent);
        _id = this.hashCode();
    }
}
