// AVL tree implementation in C
// https://www.programiz.com/dsa/avl-tree

class Node {  
    constructor(key, value) {
        this.key = key;
        this.value = value;
        this.left = null;
        this.right = null;
        this.height = 1;
    }

    static Max(a, b) {
        return (a > b) ? a : b;
    }

    // Calculate height
    static get_height(node) {
        if (node == null)
            return 0;
        return node.height;
    }

    // Right rotate
    static rightRotate(node) {
        var x = node.left;
        var T2 = x.right;

        x.right = node;
        node.left = T2;

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1;
        x.height = Node.Max(Node.get_height(x.left), Node.get_height(x.right)) + 1;

        return x; // 상위 node에서 새로운 subtree의 최상위 node를 연결하도록 리턴
    }

    // Left rotate
    static leftRotate(node) {
        var y = node.right;
        var T2 = y.left;

        y.left = node;
        node.right = T2;

        node.height = Node.Max(Node.get_height(node.left), Node.get_height(node.right)) + 1;
        y.height = Node.Max(Node.get_height(y.left), Node.get_height(y.right)) + 1;

        return y;
    }

    // Get the balance factor
    static getBalance(node) {
        if (node == null)
            return 0;
        return Node.get_height(node.left) - Node.get_height(node.right);
    }

    // 균형을 맞추고 결과적으로 최상위 node를 리턴, 균형이 맞으면 null을 리턴
    static rebalance(node, key) {
        var balance = Node.getBalance(node); // balance factor가 -1, 0, 1이면 균형 잡힘, 절댓값이 2 이상이면 불균형  
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

    static minintNode(node) {
        var current = node;

        while (current.left != null)
            current = current.left;

        return current;
    }

    // 특정한 node를 기준으로 하위 tree에서 key를 찾아서 node를 지우고 그 결과 최상위 node를 리턴한다
    static deleteNode(node, key) {
        // Find the and delete it
        if (node == null) // This가 null이면 지우고 할 것 없이 리턴
            return node;

        if (key < node.key)
            node.left = Node.deleteNode(node.left, key); // 왼쪽에서 키를 지우고 결과로 생긴 최상위 node를 This.left에 연결
        else if (key > node.key)
            node.right = Node.deleteNode(node.right, key);
        else { // key가 같으면 This를 지운다
            if ((node.left == null) || (node.right == null)) { // 좌우 자식 둘 중에 하나라도 null이면
                var temp = (node.left != null) ? node.left : node.right; 
                if (temp == null) { // 둘 다 null이면
                    return null;
                } else { // 좌우 sub tree가 하나(temp) 존재하면, 그것을 한 level 상승시킴
                    node = temp; // 살아있는 놈(temp)을 한 단계 끌어올린다
                }
            } else { // 좌우 자식 둘 다 있으면
                // 오른쪽에서 제일 작은 놈을 찾는다
                var temp = Node.minintNode(node.right);
                // 그놈을 최상위로 끌어올린다
                node.key = temp.key; 
                node.value = temp.value;
                // 그놈을 오른쪽에서 지운 후 그 결과 최상위 node를 This 오른쪽에 붙인다
                node.right = Node.deleteNode(node.right, temp.key);
            }
        }

        // Update the balance factor of each and
        // balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right));

        // Rebalance
        var balanced = Node.rebalance(node, key);
        if (balanced != null) return balanced; // rebalance한 최상위 node를 리턴한다

        return node; // 지웠는데도 균형이 맞으면 This를 리턴한다
    }

        // Print the tree
    static printPreOrder(node) {
        if (node != null) {
            process.stdout.write(node.key + ":" + node.value + " ");
            Node.printPreOrder(node.left);
            Node.printPreOrder(node.right);
        }
    }

    // this 하위에서 key를 찾아서 그 value를 리턴하는데, 없으면 null을 리턴한다 
    static searchNode(node, key) {
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

    static printNode(node, level, flag) {
        for (var i = 0; i < level; i++) {
            if (i == (level - 1) && flag == 1) { // 오른쪽일 때 +를 표시
                process.stdout.write("+ ");
            } else {
                process.stdout.write("| ");
            }
        }
        process.stdout.write("(level = " + level + ", key = " + node.key + ", value = " + node.value + ")\n");
        if (node.left != null) {
            Node.printNode(node.left, level + 1, 0);
        }
        if (node.right != null) {
            Node.printNode(node.right, level + 1, 1);
        }
    }

    static fill_keys(node, keys) {
        keys.push(node.key);
    
        if (node.left != null) {
            Node.fill_keys(node.left, keys);
        }
        if (node.right != null) {
            Node.fill_keys(node.right, keys);
        }
    }

    static fill_values(node, values) {
        values.push(node.value);
        if (node.left != null) {
            Node.fill_values(node.left, values);
        }
        if (node.right != null) {
            Node.fill_values(node.right, values);
        }
    }

    countNode(node) {
        if (node == null) {
            return 0;
        }
        return Node.countNode(node.left) + Node.countNode(node.right) + 1;
    }

    // This 하위 tree에 key와 value를 insert하고, 결과적인 최상위 node를 리턴한다
    static insertNode(node, key, value) {
        // Find the correct position to insertNode the and insertNode it
        if (node == null) // leaf node에 도달하면 새로 만들어 붙인다
            return (new Node(key, value));

        if (key < node.key)
            node.left = Node.insertNode(node.left, key, value); // 좌측에 insert하고 최상위 node를 받아서 left에 붙인다
        else if (key > node.key)
            node.right = Node.insertNode(node.right, key, value); // 우측에 insert하고 최상위 node를 받아서 right에 붙인다
        else { // key가 같으면 value를 덮어쓴다
            node.value = value;
            return node; // 최상위 변경 없음
        }

        // Update the balance factor of each and
        // Balance the tree
        node.height = 1 + Node.Max(Node.get_height(node.left), Node.get_height(node.right));

        // Rebalance
        var balanced = Node.rebalance(node, key);
        if (balanced != null) return balanced; // balance 결과 변경된 최상위 node를 리턴

        return node; // inert는 했고, 최상위 변경은 없음
    }
}

class Stack {
    constructor() {
        this.array = [];
        this.count = 0;
    }

    push(node) {
        this.array.push(node);
        this.count += 1;
    }

    pop() {
        this.count -= 1;
        var popped = this.array[this.count]; 
        this.array.splice(this.count, 1); // count 위치의 한 개 삭제
        return popped; 
    }

    get_count() {
        return this.count;
    }
}

class Iterator {
    constructor(node) {
        this.key = 0;
        this.value = 0;
        this.stack = new Stack();
        this.current = node;
        // console.log("1");
    }

    next() {
        while (this.current != null || this.stack.get_count() != 0) {          
            if (this.current != null) {
                this.stack.push(this.current);
                // console.log(this.current.value + "," + this.current.key);
                this.current = this.current.left;
                // console.log("3");                
            } else {
                var node = this.stack.pop();
                this.value = node.value;
                this.key = node.key;
                this.current = node.right;          
                // console.log(node.value + "," + node.key);
                // console.log("4");
                return this;
            }
        }
        // console.log("5");
        return null;
    }
}

class TreeMap {
    constructor() {
        this.root = null;
        this.count = 0;
    }

    get_count() {
        // return Node_count(this.root);
        return this.count;
    }
    
    keys() {
        if (this.root == null) {
            return null;
        }
        var keys = [];
        Node.fill_keys(this.root, keys);
        return keys;
    }
    
    values() {
        if (this.root == null) {
            return null;
        }
        var values = [];
        Node.fill_values(this.root, values);
        return values;
    }

    // this Tree에서 key를 찾아서 상응하는 value*을 리턴하고, 없으면 null을 리턴한다
    search(key) {
        return Node.searchNode(this.root, key);
    }

    contains(key) {
        if (this.search(key) == -999) {
            return false;
        } else {
            return true;
        }
    }

    insert(key, value) {
        if (! this.contains(key)) {
            this.count += 1;
        }
        // insert 하고 rebalancing 과정에서 root node의 변경이 일어날 수 있으므로 새로운 root를 기억
        this.root = Node.insertNode(this.root, key, value);
    }

    remove_item(key) {
        if (this.contains(key)) {
            this.count -= 1;
        }   
        this.root = Node.deleteNode(this.root, key);
    }

    print() {
        if (this.root != null) {
            Node.printNode(this.root, 0, 0);
        }
    }

    iterator() {
        if (this.root == null) {
            return null;
        }        
        var iter = new Iterator(this.root);        
        return iter;
    }
}

class Set extends TreeMap {
    constructor() {
        super();
    }

    has(element) {
        return this.contains(element);
    }

    add(element) {
        this.insert(element, 1);
    }

    remove(element) {
        this.del(element);
    }

    size() {
        return this.get_count();
    }

    intersection(set2) {
        var set3 = new Set();
        var iter = this.iterator();
        while (iter.next() != null) {
            if (set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    union(set2) {
        var set3 = new Set();
        var iter = this.iterator();
        while (iter.next() != null) {
            set3.add(iter.key);
        }
        iter = set2.iterator();
        while (iter.next() != null) {
            set3.add(iter.key);
        }
        return set3;
    }

    difference(set2) {
        var set3 = new Set();
        var iter = this.iterator();
        while (iter.next() != null) {
            if (! set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    symmetric_difference(set2) {
        var set3 = new Set();
        var iter = this.iterator();
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

    print() {
        process.stdout.write("Set.size: " + this.size() + " {");
        var iter = this.iterator();
        for (var i = 0; iter.next() != null; i++) {
            if (i == this.size() - 1) {
                process.stdout.write(iter.key + "");
            } else {
                process.stdout.write(iter.key + ", ");
            }
        }
        process.stdout.write("}\n");
    }

    elements() {
        return this.keys();
    }
}

function swap(array, i, k) {
    var temp = array[i];
    array[i] = array[k];
    array[k] = temp;
}

function compare(a, b) {
    return b - a;
}

function qsort(arr, left, right, cmpf) {
    if (left >= right) return;
    const pivot = arr[right];
    let pivotIdx = left;
    for (let i = left; i < right; i++) {
        if (cmpf(arr[i], pivot) > 0) {
            [arr[pivotIdx], arr[i]] = [arr[i], arr[pivotIdx]]; // swap
            pivotIdx++;
        }
    }
    [arr[pivotIdx], arr[right]] = [arr[right], arr[pivotIdx]]; // swap

    qsort(arr, left, pivotIdx - 1, cmpf);
    qsort(arr, pivotIdx + 1, right, cmpf);
}

function print_arr(array) {
    process.stdout.write("[");
    for (var i = 0; i < array.length; i++) {
        if (i == array.length - 1) {
            process.stdout.write(array[i] + "");    
        } else {
            process.stdout.write(array[i] + ", ");
        }
    }
    process.stdout.write("]\n");
} 

function main() {
    var root = null;
    root = Node.insertNode(root, 2, 200);
    root = Node.insertNode(root, 1, 100);
    root = Node.insertNode(root, 7, 700);
    root = Node.insertNode(root, 4, 400);
    root = Node.insertNode(root, 5, 500);
    root = Node.insertNode(root, 3, 300);
    root = Node.insertNode(root, 8, 800);

    Node.printPreOrder(root);

    root = Node.deleteNode(root, 3);

    process.stdout.write("\nAfter deletion: ");
    Node.printPreOrder(root);
    var value = Node.searchNode(root, 5);
    process.stdout.write("\nvalue : " + value + "\n");

    var tree = new TreeMap();
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
    var keys = tree.keys();
    process.stdout.write("keys : ");
    for (var i = 0; i < tree.get_count(); i++) {
        process.stdout.write(keys[i] + " ");
    }
    process.stdout.write("\n");

    var values = tree.values();
    process.stdout.write("values : ");
    for (var i = 0; i < tree.get_count(); i++) {
        process.stdout.write(values[i] + " ");
    }
    process.stdout.write("\n");
    
    var iter = tree.iterator();
    while (iter.next() != null) {
        process.stdout.write("key = " + iter.key + ", value = " + iter.value + "\n");
    }

    var setx = new Set();
    setx.add(23);
    setx.add(37);
    setx.add(41);
    setx.add(23);
    setx.add(59);
    setx.add(73);
    setx.add(89);
    setx.add(97);

    setx.print();
    var sety = new Set();
    sety.add(37);
    sety.add(67);
    sety.add(83);
    sety.add(41);
    sety.print();
    var setz = setx.intersection(sety);
    setz.print();
    var setw = setx.union(sety);
    setw.print();
    var setu = setx.difference(sety);
    setu.print();
    var setk = sety.difference(setx);
    setk.print();
    var setl = setx.symmetric_difference(sety);
    setl.print();

    var array1 = setw.elements();
    let comp = function (a, b) {
        return a - b;
    };
    print_arr(array1);
    qsort(array1, 0, array1.length - 1, comp);
    print_arr(array1);
}

main();