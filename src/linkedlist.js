class Data {
    constructor(age, name) {
        this._age = age;
        this._name = name;
    }

    equals(data) { // this == data
        if (this._age != data._age) return false; 
        if (this._name != data._name) return false;
        return true;
    }

    
    get age() { // data encapsulation
        return this._age;
    }
    
    get name() {
        return this._name;
    }

    set age(age) {
        this._age = age;
    }
    
    set name(name) {
        this._name = name;
    }

    print() {
        process.stdout.write("(" + this._age + "," + this._name + ")");
    }
}

class Node {
    constructor(data_) { // 생성자(constructor)
        this.data = data_;
        this.prev = null;
        this.next = null;
    }

    print() {
        this.data.print();
    }
}

class Iterator {    
    constructor() {
        this.list = null;
        this.next_node = null;
        this.node = null;
        this.data = null;        
    }

    next() {
        if (this.next_node == null) {
            return null;
        }
        this.node = this.next_node;
        this.data = this.node.data;
        this.next_node = this.node.next;
        return this;
    }    
}

class AList {
    constructor() {
        this.head = null;
        this.count_ = 0;
        this.iter = null;
    }

    iterator() {
        var iter = new Iterator();
        iter.list = this;
        iter.next_node = this.head;
        iter.data = null;
        iter.node = null;
        this.iter = iter;
        return iter;
    }

    prepend_item(data) {
        // var node = new Node(data); // data를 이용해서 node를 하나 생성한다
        
        // if (this.head == null) { // list가 비어 있으면 
        //     this.head = node;    // node를 list->head에 바로 할당
        // } else { 
        //     var prev_head = this.head; // 기존에 head에 있는 node를 temp에 피신시켜두고
        //     this.head = node;       // list->head에 새로 할당한 node를 할당
        //     node.next = prev_head;  // 피신시켜놓은 기존 head를 새로 할당한 node 꽁무니에 연결해준다
        //     prev_head.prev = node;  // 피신시켜놓은 prev_head->prev를 새로 들어온 node에 연결한다
        // }

        // this.count_ += 1;
        this.insert_at(0, data);
    }

    print() {
        var cur_node = this.head;
        
        process.stdout.write("count_ : " + this.count_);
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            cur_node.data.print();
            cur_node = cur_node.next;
        }
        process.stdout.write("\n");
    }

    get_count() {
        return this.count_;
    }

    append_item(data) {
        // var node = new Node(data);
        // var cur_node = this.head;

        // this.count_ += 1;
        
        // if (this.head == null) { // list가 비어있으면
        //     this.head = node;    // list->head에 node를 연결해주고 끝낸다
        //     return;
        // }

        // while (cur_node != null) { // 현재 node가 NULL이 아닌 동안에 
        //     if (cur_node.next == null) { // 마지막 node면
        //         cur_node.next = node;    // node를 연결해주고 끝낸다
        //         node.prev = cur_node;
        //         return;
        //     }
        //     cur_node = cur_node.next; // 다음 node로 넘어간다
        // }
        this.insert_at(this.get_count(), data);
    }

    get_item(index) {
        var i = 0;
        if (index < 0) {
            return null;
        }
        
        var cur_node = this.head;
        while (cur_node != null) {
            if (index == i) {
                return cur_node.data;
            }
            cur_node = cur_node.next;
            i += 1;
        }
        return null;
    }

    remove_item(data) {
        var cur_node = this.head;
        var prev_node = null; // 최초에는 이전 node가 없다
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            var next_node = cur_node.next;
            if (cur_node.data.equals(data)) { // 지워야 할 node가 식별됨
                this.count_ -= 1; 
                if (prev_node != null) {
                    if (next_node == null) {
                        prev_node.next = null;
                    } else {
                        prev_node.next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node.prev = prev_node;
                    }
                } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == null) {
                         this.head = next_node;
                    } else {
                        this.head = next_node; //list->head를 다음 node로 연결해 준다
                        next_node.prev = null;
                    }
                }
            } 
            prev_node = cur_node; // 이전 노드를 기억
            cur_node = next_node; // 다음 노드로 이동
        }
    }

    remove_at(index) {
        var cur_node = this.head;
        var prev_node = null;
        var i = 0;
        if (index < 0) return null;
        
        var cur_data = null;
        while (cur_node != null) {
            var next_node = cur_node.next;
            if (index == i) {
                cur_data = cur_node.data; // 현재 node의 data를 피신시켜두고
                cur_node.data = null; // 현재 node의 data link를 끊고
                this.count_ -= 1;
                if (prev_node != null) { 
                    if (next_node == null) {
                        prev_node.next = null;
                    } else {
                        prev_node.next = next_node; // 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node.prev = prev_node;
                    }
                    
                } else { // 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == null) {
                        this.head = next_node;
                    } else {
                        this.head = next_node; //list->head를 다음 node로 연결해 준다
                        next_node.prev = null;
                    }
                }
            }
            prev_node = cur_node; // 이전 노드를 기억
            cur_node = next_node; // 다음 노드로 이동
            i += 1;
        }
        return cur_data; // 피신시켜놓은 현재 node의 data를 리턴한다
    }

    find(data) {
        var cur_node = this.head;
        var i = 0;
        
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            if (cur_node.data.equals(data)) {
                return i;
            }
            cur_node = cur_node.next;
            i += 1;
        }
        return -1;
    }

    insert_at(index, data) {
        if (index < 0 || index > this.get_count()) // index 범위를 벗어나면 리턴 false 
            return false;
        var new_node = new Node(data); // 새로 추가할 node 생성
        var cur_node = this.head;
        var prev_node = null;
        var i = 0;
        while (cur_node != null) { // 현재 node가 null인지 여부 검사
            if (index == i) { // 추가할 index 위치를 찾으면 그곳에 끼워 넣는다
                if (prev_node == null) { // 이전 node가 없으면 뒤로만 연결하면 됨
                    new_node.next = cur_node; // 뒤로 연결
                    cur_node.prev = new_node; 
                    this.head = new_node; // 새로운 node가 list의 head가 됨
                } else { // 앞뒤로 다 연결
                    new_node.next = cur_node; // 뒤로 연결 
                    cur_node.prev = new_node; 
                    prev_node.next = new_node; // 앞으로 연결
                    new_node.prev = prev_node;
                }
                return true; // 리턴
            }
            prev_node = cur_node;
            cur_node = cur_node.next; // 뒤로 이동 
            i += 1;
        }
        if (prev_node == null) {
            this.head = new_node;
        } else {
            prev_node.next = new_node;
            new_node.prev = prev_node;
        }
        this.count_ += 1;
        return true;
    }

    sort() {
        var array = [];
        var iter = this.iterator();
        while (iter.next() != null) {
            array.push(iter.node);
        }
        qsort(array, 0, array.length - 1, compare);
    }
}

function compare(a, b) {
    var diff = b.data.age - a.data.age;
    if (diff != 0) return diff;
    if (b.data.name > a.data.name) return 1;
    if (b.data.name < a.data.name) return -1;
    return 0;
}

function qsort(arr, left, right, cmpf) {
    if (left >= right) return;
    const pivot = arr[right];
    let pivotIdx = left;
    for (let i = left; i < right; i++) {
        if (cmpf(arr[i], pivot) > 0) {
            [arr[pivotIdx].data, arr[i].data] = [arr[i].data, arr[pivotIdx].data]; // swap
            pivotIdx++;
        }
    }
    [arr[pivotIdx].data, arr[right].data] = [arr[right].data, arr[pivotIdx].data]; // swap

    qsort(arr, left, pivotIdx - 1, cmpf);
    qsort(arr, pivotIdx + 1, right, cmpf);
}

function print_arr(array) {
    process.stdout.write("[");
    for (var i = 0; i < array.length; i++) {
        if (i == array.length - 1) {
            process.stdout.write(array[i].data + "");    
        } else {
            process.stdout.write(array[i].data + ", ");
        }
    }
    process.stdout.write("]\n");
}

class AStack extends AList {    
    constructor() {
        super();
    }

    push(data) {
        this.append_item(data);
    }

    pop() {
        return this.remove_at(this.get_count() - 1);
    }

    print() {
        super.print();
    }
}

class AQue extends AList {  

    constructor() {
        super();
    }

    enqueue(data) {
        this.append_item(data);
    }

    dequeue() {
        return this.remove_at(0);
    }

    print() {
        super.print();
    }
}

function main() {
    var list = new AList();
    var data = new Data(20, "Jinho");
    list.prepend_item(data);
    var data_2 = new Data(52, "Hiongun");
    list.prepend_item(data_2);
    var data_3 = new Data(50, "Sujung");
    list.prepend_item(data_3);
    var data_4 = new Data(20, "Junho");
    list.prepend_item(data_4);
    var data_5 = new Data(20, "Junhyuk");
    list.prepend_item(data_5);
    list.print();
    list.insert_at(1, new Data(20, "Yongjun"));
    list.print();
    var idx = list.find(new Data(20, "Yongju"));
    process.stdout.write("idx = " + idx + "\n");

    var data_6 = new Data(20, "Min");
    list.append_item(data_6);
    process.stdout.write(list.get_count() + "\n");
    list.remove_item(data_4);
    process.stdout.write(list.get_count() + "\n");
    list.print();

    list.sort();
    process.stdout.write("-----\n");
    var iter = list.iterator();
    while (iter.next() != null) {
        var data_ = iter.data;
        data_.print();
    }
    process.stdout.write("\n-----\n");     
    
    var s1 = new AStack();
    var d1 = new Data(20, "김유찬");
    s1.push(d1);
    var d2 = new Data(20, "김준");
    s1.push(d2);
    s1.print();
    var dx = s1.pop();
    if (dx != null) {
        dx.print(); process.stdout.write("\n");
    }
    dx = s1.pop();
    if (dx != null) {
        dx.print(); process.stdout.write("\n");
    }
    dx = s1.pop();
    if (dx != null) {
        dx.print(); process.stdout.write("\n");
    }
    var d3 = new Data(20, "안재형");
    s1.push(d3);
    s1.print();
    
    var q = new AQue();
    var d11 = new Data(20, "김유찬");
    q.enqueue(d11);
    var d12 = new Data(20, "김태환");
    q.enqueue(d12);
    var d13 = new Data(20, "이호준");
    q.enqueue(d13);
    var d14 = q.dequeue();
    if (d14 != null) {
        d14.print();
    } 
    q.print(); process.stdout.write("\n");
}

main();