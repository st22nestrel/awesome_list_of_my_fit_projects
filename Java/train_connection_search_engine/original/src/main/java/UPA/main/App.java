package UPA.main;

import UPA.MongoDB_Client.MongoDB_Client;
import UPA.XML_Loader.Downloader;
import jakarta.xml.bind.JAXBException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import ch.qos.logback.classic.Level;
import ch.qos.logback.classic.LoggerContext;
import org.slf4j.LoggerFactory;

public class App
{
    public static void main( String[] args )
    {
        // Turning logging off
        LoggerContext loggerContext = (LoggerContext)LoggerFactory.getILoggerFactory();
        loggerContext.getLogger("org.mongodb.driver").setLevel(Level.ERROR);

        Downloader downloader = null;
        try {
            downloader = new Downloader();
        } catch (JAXBException e) {
            System.err.println("Unable to create class Downloader");
            throw new RuntimeException(e);
        }


        MongoDB_Client client = new MongoDB_Client(downloader.getMongoClient(), downloader.getDatastore());

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        while(true){
            System.out.println(
                    "Pre stiahnutie jízdních řádů a na ich nahranie do databázy zadajte '1'\n" +
                    "Pre vyhľadávanie spojov zadajte '2'\n" +
                    "Pre ukončenie aplikácie zadajte '3'\n" +
                    "Svoj výber potvrďte stlačením tlačidla 'Enter': ");
            String mode = null;
            try {
                mode = br.readLine();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            if(mode.equals("1")){
                downloader.Download();
                downloader.Unzip();;
            }
            if(mode.equals("2")){
                String odkud = null;
                String kam = null;
                String datum_a_cas = null;
                try{
                    System.out.println("Zadejte stanici 'odkud': " );
                    odkud = br.readLine();
                    System.out.println("Zadejte stanici 'kam': " );
                    kam = br.readLine();
                    System.out.println("Zadejte 'datum_a_cas' vo formáte 'yyyy-mm-ddThh:mm:ss': " );
                    datum_a_cas = br.readLine();
                    client.FindTrain(odkud, kam, datum_a_cas);

                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            else {
                System.out.println("Ukončovanie aplikácie");
                break;
            }
        }
    }
}
