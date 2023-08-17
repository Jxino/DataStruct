class Data {    
    constructor(age, name) {
        this.age = age;
        this.name = name;
    }

    equals(data) { // this == data
        if (this.age != data.age) return false; 
        if (this.name != data.name) return false;
        return true;
    }

    print() {
        process.stdout.write("(" + this.age + "," + this.name + ")");
    }
}

class Node {
    constructor(data) { // data를 포함하는 Node 객체 하나를 초기화해서 리턴한다
        this.data = data;
    }

    print() {
        this.data.print();
    }
}

class Iterator {
    constructor() {
        this.list = null;
        this.cur_index = 0;
        this.data = null;
    
    }
    
    next() {
        if (this.cur_index >= this.list.get_count()) {
            return null;
        }
        this.data = this.list.array[this.cur_index].data;
        this.cur_index += 1;
        return this;
    }
}

class AList {
    iterator() {
        var iter = new Iterator();
        iter.list = this;
        iter.cur_index = 0;
        iter.data = null;
        this.iter = iter;
        return iter;
    }
    
    // List를 생성하는 함수 
    constructor() { 
        this.array = [];
        this.iter = null;
    }

    sort() {
        qsort(this.array, 0, this.array.length - 1, compare);
    }

    prepend_item(data) { // List에 data를 추가하는 함수
        // var node = new Node(data); // data를 이용해서 node를 하나 생성한다
        // this.array.push(node);
        this.insert_at(0, data);
    }

    print() { // 
        for(var i = 0; i < this.array.length; i++) {
            this.array[i].print();                                    
        }
        process.stdout.write("\n");
    }

    remove_item(data) { // list에서 data 삭제
        for (var i = 0; i < this.array.length; i++) { 
            if (this.array[i].data.equals(data)) { // this.array[i].data == data 
                this.array.splice(i, 1); // 해당 node를 지운다
            }             
        }
    }

    get_count() {
        return this.array.length;
    }

    append_item(data) { // list에 data를 덧붙이는 함수
        // var node = new Node(data); // 입력받는 data로 node 생성
        // this.array.push(node);
        this.insert_at(this.get_count(), data);
    }

    get_item(index) {
        if (index < 0 || index >= this.array.length) {
            return null;
        }
        return this.array[index].data;
    }

    remove_at(index) {
        if (index < 0 || index >= this.array.length) {
            return null;
        }
        // Node_delete(list->array[index]);
        var data = this.array[index].data; // 일단 data를 피신시키고 
        this.array.splice(index, 1); // index번째 node 한 개를 지운다 
        return data;
    }
    
    find(data) {
        for (var i = 0; i < this.array.length; i++) {
            if (this.array[i].data.equals(data)) {
                return i;
            }
        }
        return -1;
    }

    insert_at(index, data) {
        if (index < 0 || index > this.array.length) return false;
        var node = new Node(data);
        if (index < this.get_count()) {
            this.array.splice(index, 0, node); // insert(index, node)
        } else {
            this.array.push(node);
        }        
        return true;
    }
}

class AStack extends AList { // Stack이 List를 상속받는다(Stack inherits List, Stack extends List)
    constructor() {
        super();
    }

    push(data) {
        this.append_item(data);
    }

    pop() {
        var count = this.get_count();
        var data = this.remove_at(count - 1);
        return data;
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
}

function swap(array, i, k) {
    var temp = array[i];
    array[i] = array[k];
    array[k] = temp;
}

function compare(a, b) {
    if (a.data.age != b.data.age)
        return b.data.age - a.data.age;
    if (a.data.name < b.data.name) {
        return 1;
    } else if (a.data.name == b.data.name) {
        return 0;
    }
    return -1;
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
            process.stdout.write(array[i] + "");    
        } else {
            process.stdout.write(array[i] + ", ");
        }
    }
    process.stdout.write("]\n");
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