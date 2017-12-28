using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
            List<Tuple<string, string, int>> connections
        )
        {
            // Filter by weight threshold.
            var filterdConnections = connections.Where(tuple => tuple.Item3 >= weightThreshold).ToList();

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
    }
}