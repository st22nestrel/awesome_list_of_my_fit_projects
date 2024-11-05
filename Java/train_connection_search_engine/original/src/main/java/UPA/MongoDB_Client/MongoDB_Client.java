package UPA.MongoDB_Client;

import com.mongodb.client.AggregateIterable;
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoDatabase;
import dev.morphia.Datastore;
import org.bson.Document;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

public class MongoDB_Client {
    private MongoClient mongoClient;

    private MongoDatabase mongoDB;

    private Datastore datastore;
    public MongoDB_Client(MongoClient client, Datastore datastore){
        this.mongoClient = client;
        this.datastore = datastore;
        this.mongoDB = datastore.getDatabase();
    }

    private static final DateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");

    public void FindTrain(String odkud, String kam, String datum_a_cas){
        Date date_and_time;
        try {
            date_and_time = DATE_FORMAT.parse(datum_a_cas);
        }
        catch (ParseException e) {
            throw new RuntimeException(e);
        }

        List<Document> queryPipeline = Arrays.asList(new Document("$match",
                        new Document("$and", Arrays.asList(new Document("plannedCalendar.validityPeriod.StartDateTime",
                                        new Document("$lte",
                                                date_and_time)),
                                new Document("plannedCalendar.validityPeriod.EndDateTime",
                                        new Document("$gte",
                                                date_and_time)),
                                new Document("czpttLocations.trainActivities.TrainActivityType", "0001")))),
                new Document("$set",
                        new Document("IsDayValid",
                                new Document("$substrBytes", Arrays.asList("$plannedCalendar.BitmapDays",
                                        new Document("$dateDiff",
                                                new Document("startDate", "$plannedCalendar.validityPeriod.StartDateTime")
                                                        .append("endDate",
                                                                date_and_time)
                                                        .append("unit", "day")), 1L)))),
                new Document("$lookup",
                        new Document("from", "Locations")
                                .append("localField", "czpttLocations.location.$id")
                                .append("foreignField", "_id")
                                .append("pipeline", Arrays.asList(new Document("$project",
                                        new Document("_id", 1L)
                                                .append("_t", 1L)
                                                .append("CountryCodeISO", 1L)
                                                .append("PrimaryLocationName", 1L))))
                                .append("as", "location")),
                new Document("$match",
                        new Document("$and", Arrays.asList(new Document("IsDayValid",
                                        new Document("$eq", "1")),
                                new Document("location.PrimaryLocationName", odkud),
                                new Document("location.PrimaryLocationName", kam)))),
                new Document("$set",
                        new Document("locationsArray",
                                new Document("$map",
                                        new Document("input", "$czpttLocations")
                                                .append("as", "one")
                                                .append("in",
                                                        new Document("$mergeObjects", Arrays.asList("$$one",
                                                                new Document("$arrayElemAt", Arrays.asList(new Document("$filter",
                                                                        new Document("input", "$location")
                                                                                .append("as", "two")
                                                                                .append("cond",
                                                                                        new Document("$eq", Arrays.asList("$$two._id", "$$one.location.$id")))), 0L)))))))),
                new Document("$project",
                        new Document("locationsArray.timingAtLocation", 1L)
                                .append("locationsArray.CountryCodeISO", 1L)
                                .append("locationsArray.PrimaryLocationName", 1L)));

        AggregateIterable<Document> queryResult = mongoDB.getCollection("Information").aggregate(queryPipeline);

        StringBuilder sb = new StringBuilder();

        for (Document item: queryResult) {

            boolean startFound = false;
            boolean endFound = false;
            System.out.println(item);
            for ( Document location:
                    ((ArrayList<Document>) item.get("locationsArray"))) {
                if (location.get("PrimaryLocationName").equals(odkud)){
                    startFound = true;
                    sb.append("Spojení nalezeno: \n");
                }
                else if (location.get("PrimaryLocationName").equals(kam)) {
                    endFound = true;
                }

                if (startFound){
                    sb.append("Zastávka: " + location.get("PrimaryLocationName") + "\n");
                    sb.append("\t\t " +
                            ((Document) ((Document) location.get("timingAtLocation"))
                                    .get("timing")).get("Time") + "\n");
                }
                if (endFound){
                    System.out.println(sb);
                    sb.setLength(0);
                    break;
                }
            }
        }
    }
}
