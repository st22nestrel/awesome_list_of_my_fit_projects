package UPA.XML_Schema.CZPTTInformation.PlannedCalendar;

import UPA.XML_Schema.CZPTTInformation.PlannedCalendar.elements.ValidityPeriod;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Property;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlElement;

import java.util.BitSet;

@Entity
@XmlAccessorType(XmlAccessType.FIELD)
public class PlannedCalendar {
    //TODO use
    //Bitset
    //byte[]
    @Property("BitmapDays")
    public String BitmapDays;

    @XmlElement(name = "ValidityPeriod")
    public ValidityPeriod validityPeriod;
}
