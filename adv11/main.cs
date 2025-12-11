Dictionary<string, Node> nodes = [];
var MakeNode = (string name) => nodes.TryGetValue(name, out var node) ? node : nodes[name] = new Node { ID = nodes.Count };

foreach (var line in File.ReadLines("input.txt")) {
    Node node = MakeNode(line.Split(":")[0].Trim());
    string[] children = line.Split(":")[1].Split(" ", StringSplitOptions.RemoveEmptyEntries);
    foreach (var child in children) {
        node.Children.Add(MakeNode(child));
    }
}

//Its not a general graph - its a tree, no cycles. We can define a depth, to catch siblings. 
Node root = nodes["svr"];
root.calcDepth(0);

Console.WriteLine("Part 1: " + numPaths("you", "out"));
Console.WriteLine("Part 2: " + numPaths("svr", "fft") * numPaths("fft", "dac") * numPaths("dac", "out"));

long numPaths(string from, string to) {
    int paths = 0;
    nodes[from].walk(nodes[to].ID, nodes[to].depthFromStart, ref paths);
    return paths;
}

class Node {
    public int ID;
    public int depthFromStart = int.MaxValue;
    public List<Node> Children = new List<Node>();

    public void calcDepth(int depth) {
        depthFromStart = Math.Min(depth, depthFromStart);
        int nextDepth = depth + 1;
        foreach (var child in Children) {
            if (child.depthFromStart > nextDepth) {
                child.calcDepth(nextDepth);
            }
        }
    }
    
    public void walk(int target, int maxdepth, ref int paths) {
        if (ID == target) {
            paths++;
            return;
        }
        if (depthFromStart >= maxdepth) {
            return;
        }
        foreach (var child in Children) {
            child.walk(target, maxdepth, ref paths);
        }
    }
}