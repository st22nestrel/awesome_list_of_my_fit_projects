package UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements;


import dev.morphia.annotations.Entity;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlAttribute;
import jakarta.xml.bind.annotation.XmlElement;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class TimingAtLocation {
    @XmlElement(name = "Timing")
    Timing timing;

    String DwellTime;

    String ResponsibleRU;

    String ResponsibleIM;

    String TrainType;
}

