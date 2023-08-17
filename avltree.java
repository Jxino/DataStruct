import java.util.ArrayList;
import java.util.Comparator;

// AVL tree implementation in C
// https://www.programiz.com/dsa/avl-tree

class Node {
    int key;
    int value;
    Node left;
    Node right;
    int height;

    public Node (int key, int value) {
        this.key = key;
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }

    static int Max(int a, int b) {
        return (a > b) ? a : b;
    }

    // Calculate height
    static int get_height(Node node) {
        if (node == null)
            return 0;
        return node.height;
    }

    // Right rotate
    static Node rightRotate(Node node) {
        assert(node.left != null);
        Node x = node.left;
        Node T2 = x.right;

        x.right = node;
        node.left = T2;

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1;
        x.height = Node.Max(Node.get_height(x.left), Node.get_height(x.right)) + 1;

        return x; // 상위 node에서 새로운 subtree의 최상위 node를 연결하도록 리턴
    }

    // Left rotate
    static Node leftRotate(Node node) {
        Node y = node.right;
        Node T2 = y.left;

        y.left = node;
        node.right = T2;

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1;
        y.height = Node.Max(Node.get_height(y.left), Node.get_height(y.right)) + 1;

        return y;
    }

    // Get the balance factor
    static int getBalance(Node node) {
        if (node == null)
            return 0;
        return Node.get_height(node.left) - Node.get_height(node.right);
    }

    // 균형을 맞추고 결과적으로 최상위 node를 리턴, 균형이 맞으면 null을 리턴
    static Node rebalance(Node node, int key) {
        int balance = Node.getBalance(node); // balance factor가 -1, 0, 1이면 균형 잡힘, 절댓값이 2 이상이면 불균형  
        if (balance > 1) { // 왼쪽으로 기울어 있으면
            if (key < node.left.key) { // 왼쪽 사선으로 정렬되어 있으면, 오른쪽으로 회전
                return Node.rightRotate(node); 
            }
            if (key > node.left.key) { // 왼쪽으로 삐뚤게 되어 있으면
                node.left = Node.leftRotate(node.left); // 왼쪽 사선으로 정렬시킨 후 오른쪽으로 회전
                return Node.rightRotate(node);      
            }
        }
        if (balance < -1) { // 오른쪽으로 기울어 있으면
            if (key > node.right.key) { // 오른쪽 사선으로 정렬되어 있으면, 왼쪽으로 회전
                return Node.leftRotate(node);
            }
            if (key < node.right.key) { // 오른쪽으로 삐뚤게 되어 있으면
                node.right = Node.rightRotate(node.right); // 오른쪽 사선으로 정렬시킨 후 왼쪽으로 회전
                return Node.leftRotate(node);
            }
        }
        return null; // 좌우 균형이 맞으면
    }

    static Node minintNode(Node node) {
        Node current = node;

        while (current.left != null)
            current = current.left;

        return current;
    }

    // 특정한 node를 기준으로 하위 tree에서 key를 찾아서 node를 지우고 그 결과 최상위 node를 리턴한다
    public static Node deleteNode(Node node, int key) {
        // Find the node and delete it
        if (node == null) // This가 null이면 지우고 할 것 없이 리턴
            return node;

        if (key < node.key)
            node.left = Node.deleteNode(node.left, key); // 왼쪽에서 키를 지우고 결과로 생긴 최상위 node를 This.left에 연결
        else if (key > node.key)
            node.right = Node.deleteNode(node.right, key);
        else { // key가 같으면 This를 지운다
            if ((node.left == null) || (node.right == null)) { // 좌우 자식 node 둘 중에 하나라도 null이면
                Node temp = (node.left != null) ? node.left : node.right; 
                if (temp == null) { // 둘 다 null이면
                    return null;
                } else { // 좌우 sub tree가 하나(temp) 존재하면, 그것을 한 level 상승시킴
                    node = temp; // 살아있는 놈(temp)을 한 단계 끌어올린다
                }
            } else { // 좌우 자식 node 둘 다 있으면
                // 오른쪽에서 제일 작은 놈을 찾는다
                Node temp = Node.minintNode(node.right);
                // 그놈을 최상위로 끌어올린다
                node.key = temp.key; 
                node.value = temp.value;
                // 그놈을 오른쪽에서 지운 후 그 결과 최상위 node를 This 오른쪽에 붙인다
                node.right = Node.deleteNode(node.right, temp.key);
            }
        }

        // Update the balance factor of each node and
        // balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right));

        // Rebalance
        Node balanced = Node.rebalance(node, key);
        if (balanced != null) return balanced; // rebalance한 최상위 node를 리턴한다

        return node; // 지웠는데도 균형이 맞으면 This를 리턴한다
    }

        // Print the tree
    public static void printPreOrder(Node node) {
        if (node != null) {
            System.out.printf("%d:%d ", node.key, node.value);
            Node.printPreOrder(node.left);
            Node.printPreOrder(node.right);
        }
    }

    // this node 하위에서 key를 찾아서 그 value를 리턴하는데, 없으면 null을 리턴한다 
    public static int searchNode(Node node, int key) {
        if (node == null) { // leaf node면 null 리턴 
            return -999;
        }
        if (node.key == key) {
            return node.value;
        } else if (node.key < key) {
            // 오른쪽에서 찾은 value를 리턴한다
            return Node.searchNode(node.right, key);
        } else {
            return Node.searchNode(node.left, key);
        }
    }

    public static void printNode(Node node, int level, int flag) {
        for (int i = 0; i < level; i++) {
            if (i == (level - 1) && flag == 1) { // 오른쪽일 때 +를 표시
                System.out.printf("+ ");
            } else {
                System.out.printf("| ");
            }
        }
        System.out.printf("(level = %d, key = %d, value = %d)\n", level, node.key, node.value);
        if (node.left != null) {
            Node.printNode(node.left, level + 1, 0);
        }
        if (node.right != null) {
            Node.printNode(node.right, level + 1, 1);
        }
    }

    public static void fill_keys(Node node, ArrayList<Integer> keys) {
        keys.add(node.key);
        if (node.left != null) {
            Node.fill_keys(node.left, keys);
        }
        if (node.right != null) {
            Node.fill_keys(node.right, keys);
        }
    }

    public static void fill_values(Node node, ArrayList<Integer> values) {
        values.add(node.value);
        if (node.left != null) {
            Node.fill_values(node.left, values);
        }
        if (node.right != null) {
            Node.fill_values(node.right, values);
        }
    }

    static int countNode(Node node) {
        if (node == null) {
            return 0;
        }
        return Node.countNode(node.left) + Node.countNode(node.right) + 1;
    }

    // This node 하위 tree에 key와 value를 insert하고, 결과적인 최상위 node를 리턴한다
    public static Node insertNode(Node node, int key, int value) {
        // Find the correct position to insertNode the node and insertNode it
        if (node == null) // leaf node에 도달하면 새로 만들어 붙인다
            return (new Node(key, value));

        if (key < node.key)
            node.left = Node.insertNode(node.left, key, value); // 좌측에 insert하고 최상위 node를 받아서 left에 붙인다
        else if (key > node.key)
            node.right = Node.insertNode(node.right, key, value); // 우측에 insert하고 최상위 node를 받아서 right에 붙인다
        else { // key가 같으면 value를 덮어쓴다
            node.value = value;
            return node; // 최상위 node 변경 없음
        }

        // Update the balance factor of each node and
        // Balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right));

        // Rebalance
        Node balanced = Node.rebalance(node, key);
        if (balanced != null) return balanced; // balance 결과 변경된 최상위 node를 리턴

        return node; // inert는 했고, 최상위 node 변경은 없음
    }
}

class Stack {
    ArrayList<Node> array;

    public Stack() {
        this.array = new ArrayList<Node>();
    }

    public void push(Node node) {
        this.array.add(node);
    }

    public Node pop() {
        int size = this.array.size();
        Node popped = this.array.get(size - 1);
        this.array.remove(size - 1);
        return popped;
    }

    public int get_count() {
        return this.array.size();
    }
}

class Iterator {
    int key;
    int value;
    Stack stack;
    Node current;

    public Iterator(Node node) {
        this.key = 0;
        this.value = 0;
        this.stack = new Stack();
        this.current = node;
    }

    public Iterator next() {
        while (this.current != null || this.stack.get_count() != 0) {
            
            if (this.current != null) {
                this.stack.push(this.current);
                this.current = this.current.left;
                
            } else {
                Node node = this.stack.pop();
                this.value = node.value;
                this.key = node.key;
                this.current = node.right;
                
                return this;
            }
        }
        
        return null;
    }
}

class TreeMap {
    Node root;
    int count;

    public TreeMap() {
        this.root = null;
        this.count = 0;
    }

    public int get_count() {
        // return Node_count(this.root);
        return this.count;
    }
    
    public ArrayList<Integer> keys() {
        if (this.root == null) {
            return null;
        }
        ArrayList<Integer> keys = new ArrayList<Integer>();
        Node.fill_keys(this.root, keys);
        return keys;
    }
    
    public ArrayList<Integer> values() {
        if (this.root == null) {
            return null;
        }
        ArrayList<Integer> values = new ArrayList<Integer>();
        Node.fill_values(this.root, values);
        return values;
    }

    // this Tree에서 key를 찾아서 상응하는 value*을 리턴하고, 없으면 null을 리턴한다
    public int search(int key) {
        return Node.searchNode(this.root, key);
    }

    public boolean contains(int key) {
        if (this.search(key) == -999) {
            return false;
        } else {
            return true;
        }
    }

    public void insert(int key, int value) {
        if (! this.contains(key)) {
            this.count += 1;
        }
        // insert 하고 rebalancing 과정에서 root node의 변경이 일어날 수 있으므로 새로운 root를 기억
        this.root = Node.insertNode(this.root, key, value);
    }

    public void remove_item(int key) {
        if (this.contains(key)) {
            this.count -= 1;
        }   
        this.root = Node.deleteNode(this.root, key);
    }

    public void print() {
        if (this.root != null) {
            Node.printNode(this.root, 0, 0);
        }
    }

    public Iterator iterator() {
        if (this.root == null) {
            
            return null;
        }
        
        Iterator iter = new Iterator(this.root);
        
        return iter;
    }

    public static void qsort(ArrayList<Integer> array, int left, int right, MyComp comp) {
        if (right <= left) return;

        int pivot = array.get(right);
        int pivotIdx = left;
        for (int i = left; i < right; i++) {
            if (comp.compare(pivot, array.get(i)) > 0) {
                int temp = array.get(i);
                array.set(i, array.get(pivotIdx));
                array.set(pivotIdx, temp);
                pivotIdx += 1;
            }
        }
        array.set(right, array.get(pivotIdx));
        array.set(pivotIdx, pivot);

        qsort(array, left, pivotIdx - 1, comp);
        qsort(array, pivotIdx + 1, right, comp);
    }    
}

class Set extends TreeMap {
    public Set() {}

    public void add(Integer element) {
        this.insert(element, 1);
    }

    public boolean has(Integer element) {
        return this.contains(element);
    }
    
    public Set intersection(Set set2) {
        Set set3 = new Set();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            if (set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    public Set union(Set set2) {
        Set set3 = new Set();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            set3.add(iter.key);
        }
        iter = set2.iterator();
        while (iter.next() != null) {
            set3.add(iter.key);
        }
        return set3;        
    }

    public Set difference(Set set2) {
        Set set3 = new Set();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            if (! set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    public Set symmetric_difference(Set set2) {
        Set set3 = new Set();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            if (! set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        iter = set2.iterator();
        while (iter.next() != null) {
            if (! this.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    public void print() {
        System.out.printf("{");
        Iterator iter = this.iterator();
        for (int i = 0; iter.next() != null; i++) {
            if (i == this.get_count() - 1) {
                System.out.printf("%d", iter.key);
            } else {
                System.out.printf("%d, ", iter.key);
            }
        }
        System.out.printf("}\n");
    }
}

// interface Comparator<Integer> {
//     public int compare(Integer o1, Integer o2);
// }
class MyComp implements Comparator<Integer> { // Interface
    @Override
    public int compare(Integer o1, Integer o2) {
        return o1 - o2;
    }
}

public class avltree {
    public static void main(String[] args) {
        Node root = null;
        root = Node.insertNode(root, 2, 200);
        root = Node.insertNode(root, 1, 100);
        root = Node.insertNode(root, 7, 700);
        root = Node.insertNode(root, 4, 400);
        root = Node.insertNode(root, 5, 500);
        root = Node.insertNode(root, 3, 300);
        root = Node.insertNode(root, 8, 800);

        Node.printPreOrder(root);

        root = Node.deleteNode(root, 3);

        System.out.printf("\nAfter deletion: ");
        Node.printPreOrder(root);
        int value = Node.searchNode(root, 5);
        System.out.printf("\nvalue : %d\n", value);

        TreeMap tree = new TreeMap();
        tree.insert(2, 200);
        tree.insert(1, 100);
        tree.insert(7, 700);
        tree.insert(4, 400);
        tree.insert(5, 500);
        tree.insert(3, 300);
        tree.insert(8, 800);
        tree.insert(9, 900);
        tree.insert(6, 600);
        tree.insert(10, 1000);

        tree.print();
        ArrayList<Integer> keys = tree.keys();
        System.out.printf("keys : ");
        for (int i = 0; i < tree.get_count(); i++) {
            System.out.printf("%3d", keys.get(i));
        }
        System.out.printf("\n");

        ArrayList<Integer> values = tree.values();
        System.out.printf("values : ");
        for (int i = 0; i < tree.get_count(); i++) {
            System.out.printf("%d ", values.get(i));
        }
        System.out.printf("\n");
        
        Iterator iter = tree.iterator();
        while (iter.next() != null) {
            System.out.printf("key = %d, value = %d\n", iter.key, iter.value);
        }

        System.out.printf("----------\n");
        keys = tree.keys();
        TreeMap.qsort(keys, 0, keys.size() - 1, new MyComp());
        for (int key : keys) {
            System.out.printf("key: %d, value: %d\n", key, tree.search(key));
        }
        System.out.printf("----------\n");

        Set setx = new Set();
        setx.add(23);
        setx.add(37);
        setx.add(41);
        setx.add(23);
        setx.add(59);
        setx.add(73);
        setx.add(89);
        setx.add(97);

        setx.print();
        Set sety = new Set();
        sety.add(37);
        sety.add(67);
        sety.add(83);
        sety.add(41);
        sety.print();
        Set setz = setx.intersection(sety);
        setz.print();
        Set setw = setx.union(sety);
        setw.print();
        Set setu = setx.difference(sety);
        setu.print();
        Set setk = sety.difference(setx);
        setk.print();
        Set setl = setx.symmetric_difference(sety);
        setl.print();        
    }   
}
