# AVL tree implementation in C
# https://www.programiz.com/dsa/avl-tree

class Node:
    def __init__(self, key: int, value: int):
        self.key = key
        self.value = value
        self.left = None
        self.right = None
        self.height = 1        

    def Max(a: int, b: int) -> int:
        if (a > b):
            return a
        return b
        

    # Calculate height
    def get_height(node) -> int:
        if (node == None):
            return 0
        return node.height

    # Right rotate
    def rightRotate(node):
        assert(node.left != None)
        x = node.left
        T2 = x.right

        x.right = node
        node.left = T2

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1
        x.height = Node.Max(Node.get_height(x.left), Node.get_height(x.right)) + 1

        return x # 상위 node에서 새로운 subtree의 최상위 node를 연결하도록 리턴
    

    # Left rotate
    def leftRotate(node):
        y = node.right
        T2 = y.left

        y.left = node
        node.right = T2

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1
        y.height = Node.Max(Node.get_height(y.left), Node.get_height(y.right)) + 1

        return y
    

    # Get the balance factor
    def getBalance(node):
        if (node == None):
            return 0
        return Node.get_height(node.left) - Node.get_height(node.right)
    

    # 균형을 맞추고 결과적으로 최상위 node를 리턴, 균형이 맞으면 None을 리턴
    def rebalance(node, key):
        balance = Node.getBalance(node) # balance factor가 -1, 0, 1이면 균형 잡힘, 절댓값이 2 이상이면 불균형  
        if (balance > 1): # 왼쪽으로 기울어 있으면
            if (key < node.left.key): # 왼쪽 사선으로 정렬되어 있으면, 오른쪽으로 회전
                return Node.rightRotate(node) 
            
            if (key > node.left.key): # 왼쪽으로 삐뚤게 되어 있으면
                node.left = Node.leftRotate(node.left) # 왼쪽 사선으로 정렬시킨 후 오른쪽으로 회전
                return Node.rightRotate(node)      
            
        
        if (balance < -1): # 오른쪽으로 기울어 있으면
            if (key > node.right.key): # 오른쪽 사선으로 정렬되어 있으면, 왼쪽으로 회전
                return Node.leftRotate(node)
            
            if (key < node.right.key): # 오른쪽으로 삐뚤게 되어 있으면
                node.right = Node.rightRotate(node.right) # 오른쪽 사선으로 정렬시킨 후 왼쪽으로 회전
                return Node.leftRotate(node)
            
        
        return None # 좌우 균형이 맞으면
    

    def minintNode(node):
        current = node

        while (current.left != None):
            current = current.left

        return current
    

    # 특정한 node를 기준으로 하위 tree에서 key를 찾아서 node를 지우고 그 결과 최상위 node를 리턴한다
    def deleteNode(node, key):
        # Find the node and delete it
        if (node == None): # This가 None이면 지우고 할 것 없이 리턴
            return node

        if (key < node.key):
            node.left = Node.deleteNode(node.left, key) # 왼쪽에서 키를 지우고 결과로 생긴 최상위 node를 This.left에 연결
        elif (key > node.key):
            node.right = Node.deleteNode(node.right, key)
        else: # key가 같으면 This를 지운다
            if ((node.left == None) or (node.right == None)): # 좌우 자식 node 둘 중에 하나라도 None이면
                if (node.left != None):
                    temp = node.left
                else:
                    temp = node.right 
                if (temp == None): # 둘 다 None이면
                    return None
                else: # 좌우 sub tree가 하나(temp) 존재하면, 그것을 한 level 상승시킴
                    node = temp # 살아있는 놈(temp)을 한 단계 끌어올린다
            else: # 좌우 자식 node 둘 다 있으면
                # 오른쪽에서 제일 작은 놈을 찾는다
                temp = Node.minintNode(node.right)
                # 그놈을 최상위로 끌어올린다
                node.key = temp.key 
                node.value = temp.value
                # 그놈을 오른쪽에서 지운 후 그 결과 최상위 node를 This 오른쪽에 붙인다
                node.right = Node.deleteNode(node.right, temp.key)
            
        # Update the balance factor of each node and
        # balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right))

        # Rebalance
        balanced = Node.rebalance(node, key)
        if (balanced != None):
            return balanced # rebalance한 최상위 node를 리턴한다

        return node # 지웠는데도 균형이 맞으면 This를 리턴한다
    

        # Print the tree
    def printPreOrder(node):
        if (node != None):
            print("%d:%d " % (node.key, node.value), end = "")
            Node.printPreOrder(node.left)
            Node.printPreOrder(node.right)
        
    

    # self node 하위에서 key를 찾아서 그 value를 리턴하는데, 없으면 None을 리턴한다 
    def searchNode(node, key):
        if (node == None): # leaf node면 None 리턴 
            return -999
        
        if (node.key == key):
            return node.value
        elif (node.key < key):
            # 오른쪽에서 찾은 value를 리턴한다
            return Node.searchNode(node.right, key)
        else:
            return Node.searchNode(node.left, key)
        
    

    def printNode(node, level, flag):
        for i in range (0, level): 
            if (i == (level - 1) and flag == 1): # 오른쪽일 때 +를 표시
                print("+ ", end = "")
            else:
                print("| ", end = "")
        print("(level = %d, key = %d, value = %d)" % (level, node.key, node.value))
        if (node.left != None):
            Node.printNode(node.left, level + 1, 0)
        
        if (node.right != None):
            Node.printNode(node.right, level + 1, 1)
        
    

    def fill_keys(node, keys):
        keys.append(node.key)
        if (node.left != None):
            Node.fill_keys(node.left, keys)
        
        if (node.right != None):
            Node.fill_keys(node.right, keys)
    
    def fill_values(node, values):
        values.append(node.value)
        if (node.left != None):
            Node.fill_values(node.left, values)
        
        if (node.right != None):
            Node.fill_values(node.right, values)
    
    def countNode(node):
        if (node == None):
            return 0 
        return Node.countNode(node.left) + Node.countNode(node.right) + 1
    
    # This node 하위 tree에 key와 value를 insert하고, 결과적인 최상위 node를 리턴한다
    def insertNode(node, key, value):
        # Find the correct position to insertthe node and insertit
        if (node == None): # leaf node에 도달하면 새로 만들어 붙인다
            return Node(key, value)

        if (key < node.key):
            node.left = Node.insertNode(node.left, key, value) # 좌측에 insert하고 최상위 node를 받아서 left에 붙인다
        elif (key > node.key):
            node.right = Node.insertNode(node.right, key, value) # 우측에 insert하고 최상위 node를 받아서 right에 붙인다
        else: # key가 같으면 value를 덮어쓴다
            node.value = value
            return node # 최상위 node 변경 없음
        
        # Update the balance factor of each node and
        # Balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right))

        # Rebalance
        balanced = Node.rebalance(node, key)
        if (balanced != None):
            return balanced # balance 결과 변경된 최상위 node를 리턴
        return node # inert는 했고, 최상위 node 변경은 없음

class Stack:
    def __init__(self):
        self.array = []
    
    def push(self, node):
        self.array.append(node)
    
    def pop(self):
        size = len(self.array)
        popped = self.array[size - 1]
        del self.array[size - 1]
        return popped
    
    def get_count(self):
        return len(self.array)
    


class Iterator:
    def __init__(self, node):
        self.key = 0
        self.value = 0
        self.stack = Stack()
        self.current = node    

    def next(self):
        while (self.current != None or self.stack.get_count() != 0):
            if (self.current != None):
                self.stack.push(self.current)
                self.current = self.current.left
            else:
                node = self.stack.pop()
                self.value = node.value
                self.key = node.key
                self.current = node.right    
                return self
        return None

class TreeMap:
    def __init__(self):
        self.root = None
        self.count = 0
    

    def get_count(self):
        # return Node_count(self.root)
        return self.count
    
    def keys(self):
        if (self.root == None):
            return None
        
        keys = []
        Node.fill_keys(self.root, keys)
        return keys
    
    def values(self):
        if (self.root == None):
            return None
        
        values = []
        Node.fill_values(self.root, values)
        return values
    
    # self Tree에서 key를 찾아서 상응하는 value*을 리턴하고, 없으면 None을 리턴한다
    def search(self, key):
        return Node.searchNode(self.root, key)
    
    def contains(self, key):
        if (self.search(key) == -999):
            return False
        else:
            return True

    def insert(self, key, value):
        if (not self.contains(key)):
            self.count += 1
        # insert 하고 rebalancing 과정에서 root node의 변경이 일어날 수 있으므로 새로운 root를 기억
        self.root = Node.insertNode(self.root, key, value)
    
    def remove_item(self, key):
        if (self.contains(key)):
            self.count -= 1
        self.root = Node.deleteNode(self.root, key)
    
    def print(self):
        if (self.root != None):
            Node.printNode(self.root, 0, 0)
        
    def iterator(self):
        if (self.root == None):
            return None
        iter = Iterator(self.root)
        return iter

class Set(TreeMap):
    def count(self):
        return self.get_count()

    def has(self, element):
        return self.contains(element)

    def add(self, element):
        self.insert(element, True)

    def delete(self, element):
        return self.remove(element)
    
    def intersection(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            if (set2.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def union(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            set3.add(iter.key)
        iter = set2.iterator()
        while (iter.next() != None):
            set3.add(iter.key)
        # set4 = set2.keys()
        # for i in range(0, len(set4)):
        #     if (not self.has(set4[i])):
        #         set3.append(set4[i])
        return set3
    
    def difference(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            if (not set2.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def symmetric_difference(self, set2):
        # set3 = self.difference(set2)
        # set4 = set2.difference(self)
        # for i in range(0, len(set4)):
        #     set3.append(set4[i])
        set3 = Set()
        iter = self.iterator()
        while(iter.next() != None):
            if (not set2.has(iter.key)):
                set3.add(iter.key)
        iter = set2.iterator()
        while(iter.next() != None):
            if (not self.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def print(self):
        set3 = self.keys()
        print("{", end = "")
        for i in range(0, len(set3)):
            if (i <= len(set3) - 2):
                print("%d, " % (set3[i]), end = "")
            else:
                print("%d" % (set3[i]), end = "")
        print("}")

    def elements(self):
        return self.keys()

def qsort(arr, left, right, cmpf):
    if (left >= right): return
    pivot = arr[right]
    pivotIdx = left
    for i in range(left, right):
        if (cmpf(arr[i], pivot) > 0):
            [arr[pivotIdx], arr[i]] = [arr[i], arr[pivotIdx]] # swap
            pivotIdx += 1
        
    [arr[pivotIdx], arr[right]] = [arr[right], arr[pivotIdx]] # swap

    qsort(arr, left, pivotIdx - 1, cmpf)
    qsort(arr, pivotIdx + 1, right, cmpf)

def comp(a, b):
    return b - a

def main():
    root = None
    root = Node.insertNode(root, 2, 200)
    root = Node.insertNode(root, 1, 100)
    root = Node.insertNode(root, 7, 700)
    root = Node.insertNode(root, 4, 400)
    root = Node.insertNode(root, 5, 500)
    root = Node.insertNode(root, 3, 300)
    root = Node.insertNode(root, 8, 800)

    Node.printPreOrder(root)

    root = Node.deleteNode(root, 3)

    print("\nAfter deletion: ", end = "")
    Node.printPreOrder(root)
    value = Node.searchNode(root, 5)
    print("\nvalue : %d" % (value))

    tree = TreeMap()
    tree.insert(2, 200)
    tree.insert(1, 100)
    tree.insert(7, 700)
    tree.insert(4, 400)
    tree.insert(5, 500)
    tree.insert(3, 300)
    tree.insert(8, 800)
    tree.insert(9, 900)
    tree.insert(6, 600)
    tree.insert(10, 1000)

    tree.print()
    keys = tree.keys()
    print("keys : ", end = "")
    for i in range (0, tree.get_count()):
        print("%3d" % (keys[i]), end = "")
    
    print("\n")

    values = tree.values()
    print("values : ", end = "")
    
    for i in range (0, tree.get_count()):
        print("%d " % (values[i]), end = "")
    
    print("\n")
    
    iter = tree.iterator()
    while (iter.next() != None):
        print("key = %d, value = %d" % (iter.key, iter.value))
    
    print("----------")
    keys = tree.keys()
    qsort(keys, 0, len(keys) - 1, comp)
    for key in keys:
        print("key: %d, value: %d" % (key, tree.search(key)))
    # for i in range(0, len(keys)):
    #     print("key: %d, value: %d" % (keys[i], tree.search(keys[i])))
    print("----------")

    setx = Set()
    setx.add(23)
    setx.add(37)
    setx.add(41)
    setx.add(23)
    setx.print()
    sety = Set()
    sety.add(37)
    sety.add(67)
    sety.add(83)
    sety.add(41)
    sety.print()
    setz = setx.intersection(sety)
    setz.print()
    setw = setx.union(sety)
    setw.print()
    setu = setx.difference(sety)
    setu.print()
    setk = sety.difference(setx)
    setk.print()
    setl = setx.symmetric_difference(sety)
    setl.print()

    array1 = setw.elements()
    print(array1)
    qsort(array1, 0, len(array1) - 1, comp)
    print(array1)
        
main()