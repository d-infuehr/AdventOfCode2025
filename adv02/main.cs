string input = File.ReadAllText("input.txt");

var transform = (string input) => input.Split(',').Select(x => new { id1 = long.Parse(x.Split('-')[0]), id2 = long.Parse(x.Split('-')[1]) });
var range = (long a, long b) => Enumerable.Range(0, (int)(b - a + 1)).Select(i => a + i);

var score1 = (long m) =>
{
    string full = m.ToString();
    string half = m.ToString().Substring(0, m.ToString().Length / 2);

    if (full == half + half) return m;
    else return 0;
};

var score2 = (long m) =>
{
    string full = m.ToString();

    for(int i = 1; i <= full.Length / 2; i++)
    {
        string repeated = String.Join("", Enumerable.Repeat(full.Substring(0, i), full.Length / i));
        if (full == repeated) return m;
    }
    return 0;
};

var totalScore1 = transform(input).Select(x => range(x.id1, x.id2)).SelectMany(x => x).Select(score1).Sum();
var totalScore2 = transform(input).Select(x => range(x.id1, x.id2)).SelectMany(x => x).Select(score2).Sum();

Console.WriteLine($"Total Score 1: {totalScore1}");
Console.WriteLine($"Total Score 2: {totalScore2}");
