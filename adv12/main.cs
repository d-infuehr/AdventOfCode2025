string[] lines = File.ReadAllLines("input.txt");
List<int> areas = new List<int>();

int works = 0;
for (int i = 0; i < lines.Length; i++) {
    if (lines[i].Contains('x')) {
        int area = lines[i].Split(':')[0].Split('x').Select(int.Parse).Aggregate((a, b) => a * b);
        int[] objCounts = lines[i].Split(':')[1].Trim().Split(' ').Select(int.Parse).ToArray();
        int objArea = 0;
        for (int n = 0; n < objCounts.Length; n++) {
            objArea += objCounts[n] * areas[n];
        }
        works += objArea <= area ? 1 : 0;
    } else {
        string shape = "";
        while (lines[i] != "") {
            shape += lines[i++];
        }
        areas.Add(shape.Count(x => x == '#'));
    }
}
Console.WriteLine(works);