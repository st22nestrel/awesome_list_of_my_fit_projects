package UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements;

import dev.morphia.annotations.Entity;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class TrainActivity {
    String TrainActivityType;

    AssociatedAttachedTrainID AssociatedAttachedTrainID;

    String AssociatedAttachedOTN;
}
