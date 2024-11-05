package UPA.XML_Schema.CZPTTInformation.CZPTTLocation.elements;

import dev.morphia.annotations.Entity;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;

import java.util.Date;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class AssociatedAttachedTrainID {
    String ObjectType;

    String Company;

    String Core;

    String Variant;

    String TimetableYear;

    Date StartDate;
}
