package UPA.XML_Schema.CZPTTInformation.PlannedCalendar.elements;

import dev.morphia.annotations.Entity;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;

import java.util.Date;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class ValidityPeriod {
    public Date StartDateTime;

    public Date EndDateTime;
}
