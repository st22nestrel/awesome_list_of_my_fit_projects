package UPA.XML_Schema.NetworkSpecificParameter;

import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;

@Entity("NetworkSpecificParameters")
@XmlAccessorType(XmlAccessType.FIELD)
public class NetworkSpecificParameter {
    @Id
    private int _id = this.hashCode();

    String Name;

    String Value;
}
