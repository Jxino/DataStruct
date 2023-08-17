class Graph:
    def __init__(self):
        self.adjacency_map = {}
    
    def add_node(self, node):
        if not node in self.adjacency_map:
            neighbors = {}
            self.adjacency_map[node] = neighbors

    def add_link(self, a_node, b_node):
        self.add_node(a_node)
        neighbors = self.adjacency_map[a_node]
        neighbors[b_node] = True

    def add_links(self, a_node, b_nodes):
        for b_node in b_nodes:
            self.add_link(a_node, b_node)

    def add_pairs(self, pairs):
        for pair in pairs:
            self.add_link(pair[0], pair[1])

    def search_paths(self, a_node, b_node):
        path = [[0, a_node]]
        paths = []
        # stack = []
        q = []
        visited = []
        # stack.append([0, a_node])
        q.append([0, a_node])
        while (len(q) > 0): # (len(stack) > 0):
            # level_node = stack.pop()
            level_node = q[0]; q.remove(q[0]) # q.dequeue()
            if (level_node[1] in visited): continue
            path = [p for p in path if p[0] < level_node[0]]
            path.append(level_node)
            visited.append(level_node[1])
            neighbors = self.adjacency_map.get(level_node[1], [])
            for neighbor in neighbors:
                if neighbor == b_node:
                    path.append([level_node[0] + 1, neighbor])
                    paths.append(path)
                    path = [[0, a_node]]
                    continue
                #stack.append([level_node[0] + 1, neighbor])
                q.append([level_node[0] + 1, neighbor])
        print(visited)
        return paths
    
    def print(self):
        for key in self.adjacency_map.keys():
            print("%c: " % (key), end = "")
            for value in self.adjacency_map[key].keys():
                print("%c " % (value), end = "")
            print("")

def main():
    graph = Graph()
    graph.add_links('A', ['B', 'C', 'X'])
    # graph.add_link('A', 'B')
    # graph.add_link('A', 'C')
    # graph.add_link('A', 'X')
    graph.add_links('B', ['D', 'C', 'Y'])
    # graph.add_link('B', 'D')
    # graph.add_link('B', 'C')
    # graph.add_link('B', 'Y')
    graph.add_links('C', ['F', 'Z', 'W', 'D'])
    # graph.add_link('C', 'F')
    # graph.add_link('C', 'Z')
    # graph.add_link('C', 'W')
    # graph.add_link('C', 'D')
    graph.add_links('D', ['D', 'Z'])
    # graph.add_link('D', 'D')
    # graph.add_link('D', 'Z')
    pairs = [['A', 'B'], ['A', 'C'], ['A', 'X'],
                 ['B', 'D'], ['B', 'C'], ['B', 'Y'],
                 ['C', 'F'], ['C', 'Z'], ['C', 'W'], ['C', 'D'],
                 ['D', 'D'], ['D', 'Z']]
    graph.add_pairs(pairs)
    graph.print()
    paths = graph.search_paths('A', 'D')
    for path in paths:
        print(path)

main()