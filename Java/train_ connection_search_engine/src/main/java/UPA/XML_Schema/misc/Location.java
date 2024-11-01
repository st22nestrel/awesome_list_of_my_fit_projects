package UPA.XML_Schema.misc;

import dev.morphia.annotations.Entity;
import dev.morphia.annotations.Id;
import dev.morphia.annotations.Reference;
import dev.morphia.annotations.Transient;
import jakarta.xml.bind.Unmarshaller;
import jakarta.xml.bind.annotation.XmlAccessType;
import jakarta.xml.bind.annotation.XmlAccessorType;
import jakarta.xml.bind.annotation.XmlElement;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@Entity("Locations")
@XmlAccessorType(XmlAccessType.FIELD)
public class Location {
    public int get_id() {
        return _id;
    }

    @Id
    private int _id;

    public String CountryCodeISO;

    public String LocationPrimaryCode;

    public String PrimaryLocationName;

    @Transient
    @XmlElement(name = "LocationSubsidiaryIdentification")
    public LocationSubsidiaryIdentification locationSubsidiaryIdentification;

    @Reference(lazy = true)
    public List<LocationSubsidiaryIdentification> locationSubsidiaryIdentifications = new ArrayList<>();

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Location location = (Location) o;

        if (!CountryCodeISO.equals(location.CountryCodeISO)) return false;
        if (!LocationPrimaryCode.equals(location.LocationPrimaryCode)) return false;
        return PrimaryLocationName.equals(location.PrimaryLocationName);
    }

    @Override
    public int hashCode() {
        int result = CountryCodeISO.hashCode();
        result = 31 * result + LocationPrimaryCode.hashCode();
        result = 31 * result + PrimaryLocationName.hashCode();
        return result;
    }

    void afterUnmarshal(Unmarshaller u, Object parent)
    {
        this._id = this.hashCode();
        if(locationSubsidiaryIdentification != null){
            this.locationSubsidiaryIdentifications.add(locationSubsidiaryIdentification);
        }
    }
}
