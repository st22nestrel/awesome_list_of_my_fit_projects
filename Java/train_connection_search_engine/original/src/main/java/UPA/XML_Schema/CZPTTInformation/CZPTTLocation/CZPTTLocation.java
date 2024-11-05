package UPA.XML_Schema.CZPTTInformation.CZPTTLocation;

import UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements.TimingAtLocation;
import UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements.TrainActivity;
import UPA.XML_Schema.NetworkSpecificParameter.NetworkSpecificParameter;
import UPA.XML_Schema.misc.Location;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlElement;

import java.util.List;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class CZPTTLocation {
    @Reference
    @XmlElement(name = "Location")
    public Location location;

    @XmlElement(name = "TimingAtLocation")
    public TimingAtLocation timingAtLocation;

    public String ResponsibleRU;

    public String ResponsibleIM;

    public String TrainType;

    public String TrafficType;

    public String CommercialTrafficType;

    @XmlElement(name = "TrainActivity")
    public List<TrainActivity> trainActivities;

    public String OperationalTrainNumber;

    @Reference
    List<NetworkSpecificParameter> networkSpecificParameters;
}
