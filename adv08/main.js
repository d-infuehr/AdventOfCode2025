const fs = require('fs');

class Node {
    constructor(coord, networkId) {
        this.coord = coord;
        this.networkId = networkId;
        this.id = networkId;
        this.connections = [];
    }

    dist(other) {
        const dx = (this.coord[0] - other.coord[0]);
        const dy = (this.coord[1] - other.coord[1]);
        const dz = (this.coord[2] - other.coord[2]);
        return dx*dx + dy*dy + dz*dz;
    }

    isConnected(other) {
        return other.connections.indexOf(this.id) >= 0;
    }

    join(other, full) {
        this.connections.push(other.id);
        other.connections.push(this.id);
        
        const others = full.filter(x => x.networkId == other.networkId);
        for(const node of others) {
            node.networkId = this.networkId;
        }
    }
}

let networks = fs.readFileSync('input.txt', 'utf8')
    .split('\n')
    .map((line, index) => new Node(line.split(',').map(Number), index));

function frequency() {
    const counts = {};
    for (const node of networks) {
        const id = node.networkId;
        if (counts[id] == undefined) counts[id] = 0;
        counts[id]++;
    }
    return Object.entries(counts).map(([networkId, count]) => ({
        networkId,
        count
    }));
}

function smallestDist() {
    let minDist = Number.MAX_SAFE_INTEGER;
    let sa = -1;
    let sb = -1;
    for (let ia = 0; ia < networks.length; ia++) {
        for (let ib = ia + 1; ib < networks.length; ib++) {  
            const a = networks[ia];
            const b = networks[ib];
            const ab = a.dist(b);
            if (ab < minDist && !a.isConnected(b)) {
                minDist = ab;
                sa = ia;
                sb = ib;
            }
        }
    }
    return { a : networks[sa], b : networks[sb], dist: minDist };
}

for(i = 0; i<1000; i++) {
    let k = smallestDist();
    k.a.join(k.b, networks);
}

//Part 1:
let f = frequency();
console.log("num circuits: " + f.length);
let fc = [...new Set(f.map(item => item.count))].sort((a, b) => b - a);

let total = 1;
for(i = 0; i< 3; i++){
    total *= fc[i];
}
console.log("Part 1: " + total);

//Part 2:
let lastA, lastB;
console.log("Solving...");
while(frequency().length != 1){
    let k = smallestDist();
    lastA = k.a;
    lastB = k.b;
    k.a.join(k.b, networks);
}

console.log("Part 2: " + lastA.coord[0] * lastB.coord[0]);