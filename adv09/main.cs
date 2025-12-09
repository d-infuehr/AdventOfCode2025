using System.Collections.Generic;
using System.Diagnostics;
using System.Security.Cryptography;

var input = File.ReadLines("input.txt").Select(x => x.Split(',').Select(long.Parse).ToArray()).ToList();

//Part 1
Console.WriteLine("Part1: " + Enumerable.Range(0, input.Count).SelectMany(i => Enumerable.Range(i + 1, input.Count - i - 1), (i, j) => (input[i], input[j])).Select(i => (Math.Abs(i.Item1[0] - i.Item2[0]) + 1) * (Math.Abs(i.Item1[1] - i.Item2[1]) + 1)).Max());

//Part 2: Tricky mctrickface - only remember outer edges and ban them
input.Add(input[0]);
HashSet<(int, int)> banned = new HashSet<(int, int)>();
for (int i = 0; i < input.Count - 1; i++)
{
    (int, int) from = ((int)input[i][0], (int)input[i][1]);
    (int, int) to = ((int)input[i + 1][0], (int)input[i + 1][1]);

    long dx = to.Item1 - from.Item1;
    long dy = to.Item2 - from.Item2;
    if (dx != 0 && dy != 0)
        throw new Exception("Diagonal edge detected");

    if (dx == 0 && dy > 0)
    {
        for(int y = from.Item2; y <= to.Item2; y++)
            banned.Add((from.Item1 + 1, y));
    }
    else if (dx == 0 && dy < 0)
    {
        for (int y = to.Item2; y <= from.Item2; y++)
            banned.Add((from.Item1 - 1, y));
    }
    else if (dy == 0 && dx > 0)
    {
        for (int x = from.Item1; x <= to.Item1; x++)
            banned.Add((x, from.Item2 - 1));
    }
    else if (dy == 0 && dx < 0)
    {
        for (int x = to.Item1; x <= from.Item1; x++)
            banned.Add((x, from.Item2 + 1));
    }
    else throw new NotImplementedException("Diagonal edge detected");
}

// Unban the actual edges
for (int i = 0; i < input.Count - 1; i++)
{
    int x1 = (int)Math.Min(input[i][0], input[i + 1][0]);
    int x2 = (int)Math.Max(input[i][0], input[i + 1][0]);
    int y1 = (int)Math.Min(input[i][1], input[i + 1][1]);
    int y2 = (int)Math.Max(input[i][1], input[i + 1][1]);
    for(int x = x1; x <= x2; x++)
        for(int y = y1; y <= y2; y++)
            banned.Remove((x, y));
}

long AreaOf(long[] from, long[] to)
{
    int x1 = (int)Math.Min(from[0], to[0]);
    int x2 = (int)Math.Max(from[0], to[0]);
    int y1 = (int)Math.Min(from[1], to[1]);
    int y2 = (int)Math.Max(from[1], to[1]);

    foreach (var (bx, by) in banned)
    {
        if (bx >= x1 && bx <= x2 && by >= y1 && by <= y2)
            return 0;
    }
    return (Math.Abs(x2 - x1) + 1) * (Math.Abs(y2 - y1) + 1);
}

input.RemoveAt(input.Count - 1);

var m = Stopwatch.StartNew();
var tests = Enumerable.Range(0, input.Count).SelectMany(i => Enumerable.Range(i + 1, input.Count - i - 1), (i, j) => (input[i], input[j])).AsParallel().Select(i => AreaOf(i.Item1, i.Item2)).Max();
Console.WriteLine("Part2: " + tests);
Console.WriteLine("Time via naive approach: " + m.ElapsedMilliseconds + "ms");
m.Restart();


//Lookup test:
Dictionary<int, SortedSet<int>> banyx = new Dictionary<int, SortedSet<int>>();
Dictionary<int, SortedSet<int>> banxy = new Dictionary<int, SortedSet<int>>();
SortedSet<int> bannedY = new SortedSet<int>();
SortedSet<int> bannedX = new SortedSet<int>();
foreach (var (bx, by) in banned)
{
    bannedX.Add(bx);
    bannedY.Add(by);
    if (!banyx.TryGetValue(by, out var setx))
    {
        setx = new SortedSet<int>();
        banyx[by] = setx;
    }
    setx.Add(bx);

    if (!banxy.TryGetValue(bx, out var sety))
    {
        sety = new SortedSet<int>();
        banxy[bx] = sety;
    }
    sety.Add(by);
}

long AreaOf2(long[] from, long[] to)
{
    int x1 = (int)Math.Min(from[0], to[0]);
    int x2 = (int)Math.Max(from[0], to[0]);
    int y1 = (int)Math.Min(from[1], to[1]);
    int y2 = (int)Math.Max(from[1], to[1]);

    if ((x2 - x1) <= (y2 - y1))
    {
        foreach (int x in bannedX.GetViewBetween(x1, x2))
        {
            if (banxy[x].GetViewBetween(y1, y2).Count > 0)
                return 0;
        }
    }
    else
    {
        foreach (int y in bannedY.GetViewBetween(y1, y2))
        {
            if (banyx[y].GetViewBetween(x1, x2).Count > 0)
                return 0;
        }
    }
    return (Math.Abs(x2 - x1) + 1) * (Math.Abs(y2 - y1) + 1);
}


var tests2 = Enumerable.Range(0, input.Count).SelectMany(i => Enumerable.Range(i + 1, input.Count - i - 1), (i, j) => (input[i], input[j])).AsParallel().Select(i => AreaOf2(i.Item1, i.Item2)).Max();
Console.WriteLine("Part2: " + tests2);
Console.WriteLine("Time via dictionary and sortedset: " + m.ElapsedMilliseconds + "ms");
