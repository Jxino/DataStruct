import java.util.ArrayList;
import java.util.HashMap;

class Debug {
    public static void print(String str) {
        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
        if (stackTrace.length >= 3) {
            StackTraceElement caller = stackTrace[2];
            String fileName = caller.getFileName();
            int lineNumber = caller.getLineNumber();
            System.out.printf("(%s,%d)%s\n", fileName, lineNumber, str);
        } else {
            System.out.println("Unable to retrieve file info and line number.");
        }
    }    
}

class Pair {
    Character a;
    Character b;

    public Pair(Character a, Character b) {
        this.a = a;
        this.b = b;
    }
}

class LevelNode {
    public Integer level;
    public Character node;

    public LevelNode(Integer level, Character node) {
        this.level = level;
        this.node = node;
    }
}

class AGraph {
    HashMap<Character, HashMap<Character, Boolean>> adjacency_map;
    
    public AGraph() {
        this.adjacency_map = new HashMap<Character, HashMap<Character, Boolean>>();
    }
    
    public void add_node(Character node) {
        if (! this.adjacency_map.containsKey(node)) {
            HashMap<Character, Boolean> neighbors = new HashMap<Character, Boolean>();
            this.adjacency_map.put(node, neighbors);
        }
    }

    public void add_link(Character a_node, Character b_node) {
        Debug.print(String.format("add_link (%c, %c)", a_node, b_node));
        this.add_node(a_node);
        HashMap<Character, Boolean> neighbors = this.adjacency_map.get(a_node);
        neighbors.put(b_node, true);
    }

    public void add_links(Character a_node, ArrayList<Character> b_nodes) {
        for (int i = 0; i < b_nodes.size(); i++) { // b_node in b_nodes
            this.add_link(a_node, b_nodes.get(i));
        }
    }

    public void add_pairs(ArrayList<Pair> pairs) {
        for (Pair pair : pairs) {
            Debug.print(String.format("add_pairs: %c, %c", pair.a, pair.b));
            this.add_link(pair.a, pair.b);
        }
        // Set<Character> keys = pairs.keySet();
        // Iterator<Character> iter = keys.iterator();
        // while (iter.hasNext()) { // pair in pairs
        //     Character key = iter.next();
        //     this.add_link(key, pairs.get(key));
        // }
    }

    public ArrayList<ArrayList<LevelNode>> search_paths(Character a_node, Character b_node) {
        ArrayList<LevelNode> path = new ArrayList<LevelNode>();// [[0, a_node]]
        LevelNode init = new LevelNode(0, a_node);
        path.add(init);
        ArrayList<ArrayList<LevelNode>> paths = new ArrayList<ArrayList<LevelNode>>();
        // ArrayList<LevelNode> stack = new ArrayList<LevelNode>();
        ArrayList<LevelNode> q = new ArrayList<LevelNode>();
        ArrayList<Character> visited = new ArrayList<Character>();
        // stack.add(init)
        q.add(init);
        while (q.size() > 0) { // (stack.size() > 0):
            // LevelNode level_node = stack.get(stack.size() - 1); stack.remove(stack.size() - 1); // stack.pop()
            LevelNode level_node = q.get(0); q.remove(0); // q.dequeue()
            if (visited.contains(level_node.node)) continue;
            // ArrayList<LevelNode> new_path = new ArrayList<LevelNode>();
            // for (int i = 0; i < path.size(); i++) {
            //     if (path.get(i).level < level_node.level) {
            //         new_path.add(path.get(i));
            //     }
            // }
            // path = new_path;
            for (int i = path.size() - 1; i >= 0; i--) {
                if (path.get(i).level >= level_node.level) {
                    path.remove(i);
                }
            }
            path.add(level_node);
            visited.add(level_node.node);            
            if (this.adjacency_map.get(level_node.node) == null) continue; 
            for (Character neighbor : this.adjacency_map.get(level_node.node).keySet()) {
                if (neighbor == b_node) {
                    LevelNode element = new LevelNode(level_node.level + 1, neighbor);
                    path.add(element);
                    paths.add(path);
                    ArrayList<LevelNode> new_path = new ArrayList<LevelNode>();
                    LevelNode start = new LevelNode(0, a_node);
                    new_path.add(start);
                    path = new_path;
                    continue;
                }
                LevelNode element_2 = new LevelNode(level_node.level + 1, neighbor);
                q.add(element_2);
            }
        }
        for (int i = 0; i < visited.size(); i++) {
            System.out.printf("%c ", visited.get(i));
        }
        System.out.printf("\n");
        return paths;
    } // search_paths

    public void print() {
        for (Character key : this.adjacency_map.keySet()) {
            System.out.printf("%c: ", key);
            for (Character value : this.adjacency_map.get(key).keySet()) {
                System.out.printf("%c ", value);
            }
            System.out.printf("\n");
        }
    }
} // Graph class

public class graph {
    public static void main(String[] args) {
        AGraph graph = new AGraph();
        ArrayList<Character> a_neighbors = new ArrayList<Character>();
        a_neighbors.add('B');
        a_neighbors.add('C');
        a_neighbors.add('X');
        graph.add_links('A', a_neighbors);
        ArrayList<Character> b_neighbors = new ArrayList<Character>();
        b_neighbors.add('D');
        b_neighbors.add('C');
        b_neighbors.add('Y');
        graph.add_links('B', b_neighbors);
        ArrayList<Character> c_neighbors = new ArrayList<Character>();
        c_neighbors.add('F');
        c_neighbors.add('Z');
        c_neighbors.add('W');
        c_neighbors.add('D');
        graph.add_links('C', c_neighbors);
        // ArrayList<Character> d_neighbors = new ArrayList<Character>();
        // d_neighbors.add('D');
        // d_neighbors.add('Z');
        // graph.add_links('D', d_neighbors);
        ArrayList<Pair> pairs = new ArrayList<Pair>();
        pairs.add(new Pair('A', 'B'));
        pairs.add(new Pair('A', 'C'));
        pairs.add(new Pair('A', 'X'));
        pairs.add(new Pair('B', 'D'));
        pairs.add(new Pair('B', 'C'));
        pairs.add(new Pair('B', 'Y'));
        pairs.add(new Pair('C', 'F'));
        pairs.add(new Pair('C', 'Z'));
        pairs.add(new Pair('C', 'W'));
        pairs.add(new Pair('C', 'D'));
        pairs.add(new Pair('D', 'D'));
        pairs.add(new Pair('D', 'Z'));
        graph.add_pairs(pairs);
        graph.print();
        ArrayList<ArrayList<LevelNode>> paths = graph.search_paths('A', 'D');
        for (ArrayList<LevelNode> path : paths) {
            for (LevelNode element : path) {
                System.out.printf("(%d,%c)", element.level, element.node);
            }
            System.out.printf("\n");
        }
    }
} 
// main():
//     graph = Graph()
//     graph.add_links('A', ['B', 'C', 'X'])
//     // graph.add_link('A', 'B')
//     // graph.add_link('A', 'C')
//     // graph.add_link('A', 'X')
//     graph.add_links('B', ['D', 'C', 'Y'])
//     // graph.add_link('B', 'D')
//     // graph.add_link('B', 'C')
//     // graph.add_link('B', 'Y')
//     graph.add_links('C', ['F', 'Z', 'W', 'D'])
//     // graph.add_link('C', 'F')
//     // graph.add_link('C', 'Z')
//     // graph.add_link('C', 'W')
//     // graph.add_link('C', 'D')
//     graph.add_links('D', ['D', 'Z'])
//     // graph.add_link('D', 'D')
//     // graph.add_link('D', 'Z')
//     pairs = [['A', 'B'], ['A', 'C'], ['A', 'X'],
//                  ['B', 'D'], ['B', 'C'], ['B', 'Y'],
//                  ['C', 'F'], ['C', 'Z'], ['C', 'W'], ['C', 'D'],
//                  ['D', 'D'], ['D', 'Z']]
//     graph.add_pairs(pairs)
//     graph.print()
//     paths = graph.search_paths('A', 'D')
//     for path in paths:
//         print(path)

// main()