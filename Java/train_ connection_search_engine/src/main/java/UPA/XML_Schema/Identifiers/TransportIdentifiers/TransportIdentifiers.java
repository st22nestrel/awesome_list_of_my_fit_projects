package UPA.XML_Schema.Identifiers.TransportIdentifiers;

import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;

import java.util.Date;

@Entity("TransportIdentifiers")
@XmlAccessorType(XmlAccessType.FIELD)
public class TransportIdentifiers {
    @Id
    private int _id = this.hashCode();

    String ObjectType;

    String Company;

    String Core;

    String Variant;

    String TimetableYear;

    Date StartDate;
}
