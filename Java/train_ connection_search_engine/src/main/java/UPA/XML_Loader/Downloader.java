package UPA.XML_Loader;

import UPA.XML_Schema.CZCanceledPTTMessage;
import UPA.XML_Schema.CZPTTCISMessage;
import UPA.XML_Schema.CZPTTInformation.CZPTTInformation;
import UPA.XML_Schema.CZPTTInformation.CZPTTLocation.CZPTTLocation;
import UPA.XML_Schema.misc.Location;
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import dev.morphia.Datastore;
import dev.morphia.Morphia;
import dev.morphia.query.experimental.filters.Filters;
import jakarta.xml.bind.JAXBContext;
import jakarta.xml.bind.JAXBException;
import jakarta.xml.bind.Unmarshaller;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.*;
import java.net.URL;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Enumeration;
import java.util.stream.Stream;
import java.util.zip.GZIPInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class Downloader {
    private String userDirectory = System.getProperty("user.dir");

    public Downloader() throws JAXBException {
        datastore.getMapper().mapPackage(CZPTTCISMessage.class.getPackageName());
    }

    private void DownloadFile(String href) throws IOException {
        URL url = new URL(href);
        ReadableByteChannel readableByteChannel = Channels.newChannel(url.openStream());
        String filePathToDownload = url.getFile();
        String fileNameToDownload = filePathToDownload.substring(
                filePathToDownload.lastIndexOf("/")
        );

        File file = new File(userDirectory + "/zips" + fileNameToDownload);
        //download only if not already downloaded
        if(!file.exists()){
            file.getParentFile().mkdirs();
            file.createNewFile();
            FileOutputStream fileOutputStream = new FileOutputStream(file);

            fileOutputStream.getChannel()
                    .transferFrom(readableByteChannel, 0, Long.MAX_VALUE);
        }
    }
    public void Download(){
        try {
            Document tranSite = Jsoup.connect("https://portal.cisjr.cz/pub/draha/celostatni/szdc/").get();
            Elements trainLinks = tranSite.select("a[href]");

            Element folder2022 = trainLinks.stream().filter(element ->
                    element.attributes().get("href").endsWith("2022/")
            ).findFirst().get();

            Document site2022 = Jsoup.connect("https://portal.cisjr.cz" + folder2022.attributes().get("href")).get();

            Elements links2022 = site2022.select("a[href]");

            for (Element link : links2022) {
                String fileLink = link.attributes().get("href");
                if (fileLink.endsWith("zip")) {
                    DownloadFile(fileLink);
                } else {
                    Document innerFolder = Jsoup.connect("https://portal.cisjr.cz" + fileLink).get();
                    Elements innerLinks = innerFolder.select("a[href]");
                    for (Element innerLink : innerLinks) {
                        String innerFileLink = innerLink.attributes().get("href");
                        DownloadFile(innerFileLink);
                    }
                }
            }
        }
        catch (Exception e){
            e.printStackTrace(System.err);
        }
    }

    private JAXBContext jaxbMessageContext = JAXBContext.newInstance(CZPTTCISMessage.class);
    private JAXBContext jaxbCanceledMessageContext = JAXBContext.newInstance(CZCanceledPTTMessage.class);

    private Unmarshaller jaxbMessageUnmarshaller = jaxbMessageContext.createUnmarshaller();
    private Unmarshaller jaxbCanceledMessageUnmarshaller = jaxbCanceledMessageContext.createUnmarshaller();

    public MongoClient getMongoClient() {
        return mongoClient;
    }

    public Datastore getDatastore() {
        return datastore;
    }

    private final MongoClient mongoClient = MongoClients.create( "mongodb://localhost:27017" );

    private final Datastore datastore = Morphia.createDatastore(mongoClient, "morphia_quickstart");

    public CZPTTCISMessage UnMarshallCZPTTCISMessage(InputStream input) throws JAXBException {
        return (CZPTTCISMessage) jaxbMessageUnmarshaller.unmarshal(input);
    }

    public CZCanceledPTTMessage UnMarshallCZCanceledPTTMessage(InputStream input) throws JAXBException {
        return (CZCanceledPTTMessage) jaxbCanceledMessageUnmarshaller.unmarshal(input);
    }

    public void UnmarshallXML(Path path, InputStream stream) {
        try {
            // XML is CZPTTCISMessage
            CZPTTCISMessage message = UnMarshallCZPTTCISMessage(stream);
            MongodbUpsertCZPTTCISMessage(message);
        }
        catch (Exception e){
            // XML is CZCanceledPTTMessage
            try {
                CZCanceledPTTMessage message = UnMarshallCZCanceledPTTMessage(stream);
                MongodbUpsertCZCanceledPTTMessage(message);
            }
            // Unknown XML
            catch (Exception ex){
                System.err.println("Unable to parse file: " + path);
                ex.printStackTrace(System.err);
            }
        }
    }

    public void MongodbUpsertCZPTTCISMessage(CZPTTCISMessage message){
        //Store all entities separately, because Morphia can't save them at once
        //datastore.save(message.identifiers);
        message.identifiers.plannedTransportIdentifiers.forEach(datastore::save);
        message.identifiers.relatedPlannedTransportIdentifiers.forEach(datastore::save);
        datastore.save(message.czpttHeader);
        if(message.czpttHeader.ForeignOriginLocation != null){
            datastore.save(message.czpttHeader.ForeignOriginLocation);
            datastore.save(message.czpttHeader.ForeignOriginLocation.LocationIdent);
        }
        if(message.czpttHeader.ForeignDestinationLocation != null){
            datastore.save(message.czpttHeader.ForeignDestinationLocation);
            datastore.save(message.czpttHeader.ForeignDestinationLocation.LocationIdent);
        }
        message.czpttInformation.forEach(inf ->{
            ((CZPTTInformation) inf).czpttLocations.forEach(pttLoc -> {

                Location loc = ((CZPTTLocation) pttLoc).location;
                Location locInDB = datastore.find(Location.class).filter(Filters.eq("_id", loc.get_id())).first();
                if(loc.locationSubsidiaryIdentification != null){
                    datastore.save(loc.locationSubsidiaryIdentification);
                }
                if(locInDB != null && loc.locationSubsidiaryIdentification != null){
                    locInDB.locationSubsidiaryIdentifications.add(loc.locationSubsidiaryIdentification);
                    datastore.save(locInDB);
                }
                else{
                    datastore.save(loc);
                }
            });
        });
        datastore.save(message.czpttInformation);
        message.networkSpecificParameters.forEach(datastore::save);

        datastore.save(message);
    }

    public void MongodbUpsertCZCanceledPTTMessage(CZCanceledPTTMessage message){
        //Store all entities separately, because Morphia can't save them at once
        message.plannedTransportIdentifiers.forEach(datastore::save);
        datastore.save(message);
    }

    public void Unzip(){
        System.out.println("CWD is: " + userDirectory);
        System.out.println("Listing all files in directory:");

        try (Stream<Path> paths = Files.walk(Paths.get(userDirectory + "/zips"))) {
            paths.filter(Files::isRegularFile).forEach(path -> {
                if(path.endsWith("xml.zip")){
                    try {
                        GZIPInputStream stream = new GZIPInputStream(new FileInputStream(path.toString()));
                        UnmarshallXML(path, stream);
                    } catch (IOException e) {
                        System.err.println("Is this file a zipfile?: " + path);
                        e.printStackTrace(System.err);
                    }

                }
                else {
                    try {
                        ZipFile zipFile = new ZipFile(path.toString());
                        Enumeration<? extends ZipEntry> entries = zipFile.entries();

                        while(entries.hasMoreElements()){
                            ZipEntry entry = entries.nextElement();
                            InputStream stream = zipFile.getInputStream(entry);

                            UnmarshallXML(path, stream);
                        }
                    } catch (IOException e) {
                        System.err.println("Is this file a zipfile?: " + path);
                        e.printStackTrace(System.err);
                    }
                }
            });

        }
        catch (Exception e){
            e.printStackTrace(System.err);
        }
    }
}
