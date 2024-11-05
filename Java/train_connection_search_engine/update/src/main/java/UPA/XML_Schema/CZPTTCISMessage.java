package UPA.XML_Schema;

import UPA.XML_Schema.CZPTTHeader.CZPTTHeader;
import UPA.XML_Schema.CZPTTInformation.CZPTTInformation;
import UPA.XML_Schema.Identifiers.Identifiers;
import UPA.XML_Schema.NetworkSpecificParameter.NetworkSpecificParameter;
import dev.morphia.annotations.*;
import jakarta.xml.bind.annotation.*;
import org.bson.types.ObjectId;

import java.util.ArrayList;
import java.util.List;


@Entity("CZPTTCISMessages")
@XmlRootElement(name = "CZPTTCISMessage")
//@XmlAccessorType(XmlAccessType.FIELD)
public class CZPTTCISMessage {
    @Id
    private int _id = this.hashCode();

    @XmlElement(name = "Identifiers")
    public Identifiers identifiers;

    @XmlElement(name = "CZPTTCreation")
    public String czpttCreation;

    @Reference(lazy = true)
    @XmlElement(name = "CZPTTHeader")
    public CZPTTHeader czpttHeader;

    @Reference(lazy = true)
    @XmlElement(name = "CZPTTInformation")
    public List<CZPTTInformation> czpttInformation = new ArrayList<>();

    @Reference(lazy = true)
    @XmlElement(name = "NetworkSpecificParameter")
    public List<NetworkSpecificParameter> networkSpecificParameters = new ArrayList<>();

}
