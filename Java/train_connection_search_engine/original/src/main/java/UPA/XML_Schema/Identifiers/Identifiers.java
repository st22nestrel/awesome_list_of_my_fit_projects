package UPA.XML_Schema.Identifiers;

import UPA.XML_Schema.Identifiers.TransportIdentifiers.TransportIdentifiers;
import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import jakarta.xml.bind.annotation.XmlElement;

import java.util.ArrayList;
import java.util.List;

@Entity
public class Identifiers {

    @Reference(lazy = true)
    @XmlElement(name = "PlannedTransportIdentifiers")
    public List<TransportIdentifiers> plannedTransportIdentifiers = new ArrayList<>();

    @Reference(lazy = true)
    @XmlElement(name = "RelatedPlannedTransportIdentifiers")
    public List<TransportIdentifiers> relatedPlannedTransportIdentifiers = new ArrayList<>();
}
