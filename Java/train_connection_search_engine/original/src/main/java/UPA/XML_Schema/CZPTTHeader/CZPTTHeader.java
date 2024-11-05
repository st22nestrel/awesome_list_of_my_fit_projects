package UPA.XML_Schema.CZPTTHeader;

import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.annotation.XmlElement;

@Entity("Headers")
public class CZPTTHeader {
    @Id
    private int _id = this.hashCode();

    @Reference
    @XmlElement(name = "ForeignOriginLocation")
    public ForeignLocation ForeignOriginLocation;

    @Reference
    @XmlElement(name = "ForeignDestinationLocation")
    public ForeignLocation ForeignDestinationLocation;
}
