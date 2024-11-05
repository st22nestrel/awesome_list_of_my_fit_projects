package UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements;

import dev.morphia.annotations.Entity;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlAttribute;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class Timing {
    String Time;
    String Offset;

    @XmlAttribute
    String TimmingQualifierCode;
}
