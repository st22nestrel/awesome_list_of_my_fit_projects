package UPA.XML_Schema.CZPTTHeader;

import UPA.XML_Schema.misc.Location;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.annotation.XmlElement;

@Entity
public class ForeignLocation {
    @Id
    private int _id = this.hashCode();

    @Reference
    @XmlElement(name = "LocationIdent")
    public Location LocationIdent;
}
