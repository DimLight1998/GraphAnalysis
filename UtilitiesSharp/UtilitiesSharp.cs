using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace UtilitiesSharp
{
    public class UtilitiesSharp
    {
        public static void ExportGraphToJson(
            string fileName,
            int weightThreshold,
            double weightModificationFactor,
            bool removeIsolated,
            Dictionary<string, int> movieNameAndCategory,
            List<Tuple<string, string, int>> connections,
            double reserveProb,
            int reserveHigherThan
        )
        {
            var randomGenerator = new Random(DateTime.Now.Millisecond);

            // Filter by weight threshold.
            var filterdConnections = connections.Where(tuple => tuple.Item3 >= weightThreshold).ToList();

            // Remove some edges to reduce complexity by probability.
            for (var i = filterdConnections.Count - 1; i >= 0; i--)
                if (randomGenerator.NextDouble() > reserveProb && filterdConnections[i].Item3 <= reserveHigherThan)
                    filterdConnections.RemoveAt(i);

            // If need to remove isolated points, remove them.
            if (removeIsolated)
            {
                var nonisolatedPoints = new HashSet<string>();
                foreach (var filterdConnection in filterdConnections)
                {
                    nonisolatedPoints.Add(filterdConnection.Item1);
                    nonisolatedPoints.Add(filterdConnection.Item2);
                }

                var nonisolatedRecords = movieNameAndCategory.Where(pair => nonisolatedPoints.Contains(pair.Key));
                movieNameAndCategory = nonisolatedRecords.ToDictionary(pair => pair.Key, pair => pair.Value);
            }

            // Export to json.
            using (var file = new StreamWriter(fileName))
            {
                var jsonObject = new JObject(
                    new JProperty("nodes",
                        new JArray(
                            from item in movieNameAndCategory
                            select new JObject(
                                new JProperty("id", item.Key),
                                new JProperty("group", item.Value)
                            )
                        )),
                    new JProperty("links",
                        new JArray(
                            from item in filterdConnections
                            select new JObject(
                                new JProperty("source", item.Item1),
                                new JProperty("target", item.Item2),
                                new JProperty("value", (int) (item.Item3 * weightModificationFactor))
                            )
                        )
                    )
                );

                file.Write(jsonObject.ToString(Formatting.None));
            }
        }

        public static void BuildUserGraph(string movieFilePath, string userFilePath)
        {
            Console.WriteLine("Enter the threshold for the user-network:");
            var line = Console.ReadLine();

            if (int.TryParse(line, out var threshold) == false)
            {
                Console.WriteLine("Incorrect format, use 1 instead.");
                threshold = 1;
            }

            var allUsers = new Dictionary<string, Tuple<int, HashSet<int>>>();
            var allMovies = new Dictionary<string, int>();
            var movieUserRecords = new Dictionary<int, HashSet<string>>();

            Console.WriteLine("Start reading movie file.");
            // Read all movies.
            var counter = 0;
            using (var movieFile = new StreamReader(movieFilePath))
            {
                while ((line = movieFile.ReadLine()) != null)
                {
                    var splited = line.Split(new[] {','}, StringSplitOptions.RemoveEmptyEntries);
                    var movieName = splited[1];
                    if (allMovies.Keys.Contains(movieName)) continue;
                    allMovies.Add(movieName, counter);
                    counter++;
                }
            }

            Console.WriteLine("Start reading user file.");
            // Read all users.
            counter = 0;
            using (var userFile = new StreamReader(userFilePath))
            {
                while ((line = userFile.ReadLine()) != null)
                {
                    if (string.IsNullOrWhiteSpace(line)) continue;
                    var splited = line.Split(new[] {','}, StringSplitOptions.RemoveEmptyEntries);
                    var movieName = splited[0];
                    var userName = splited[1];

                    if (allUsers.Keys.Contains(userName)) allUsers[userName].Item2.Add(allMovies[movieName]);
                    else
                        allUsers.Add(userName,
                            new Tuple<int, HashSet<int>>(counter, new HashSet<int> {allMovies[movieName]}));

                    if (movieUserRecords.Keys.Contains(allMovies[movieName]))
                        movieUserRecords[allMovies[movieName]].Add(userName);
                    else
                        movieUserRecords.Add(allMovies[movieName], new HashSet<string> {userName});

                    counter++;
                }
            }

            // Export to json.
            var jsonObject = new JObject();

            var nodesObject = new JArray();
            foreach (var user in allUsers)
                nodesObject.Add(new JObject(new JProperty("name", user.Key), new JProperty("group", user.Value.Item1)));

            var edgesObject = new JArray();

            var jsonLock = new object();
            var count = 0;
            Parallel.ForEach(allUsers, user =>
            {
                var id = user.Value.Item1;
                var movieList = user.Value.Item2;
                var connections = new Dictionary<string, int>();

                foreach (var movie in movieList)
                foreach (var otherUser in movieUserRecords[movie])
                {
                    if (id > allUsers[otherUser].Item1) continue;
                    if (connections.Keys.Contains(otherUser)) connections[otherUser]++;
                    else connections.Add(otherUser, 1);
                }

                lock (jsonLock)
                {
                    Console.WriteLine($"[{count++:D8}] Building graph for user {user.Key}");

                    foreach (var connection in connections)
                        if (connection.Value >= threshold)
                            edgesObject.Add(new JObject(
                                new JProperty("start", user.Key),
                                new JProperty("end", connection.Key),
                                new JProperty("value", connection.Value)
                            ));
                }
            });

            jsonObject.Add(new JObject(new JProperty("nodes", nodesObject), new JProperty("edges", edgesObject)));

            using (var fileOut = new StreamWriter("out.json"))
            {
                fileOut.Write(jsonObject.ToString(Formatting.None));
            }
        }
    }
}