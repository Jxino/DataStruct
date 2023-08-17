class Bucket {  
    NOT_FOUND = -999;

    constructor() {
        this.keys = [];
        this.values = [];
    }

    put(key, value) {
        for (var i = 0; i < this.keys.length; i++) {
            if (this.keys[i] == key) {
                this.values[i] = value;
                return false;
            }
        }
        this.keys.push(key);
        this.values.push(value);
        return true;
    }

    get(key) {
        for (var i = 0; i < this.keys.length; i++) {
            if (this.keys[i] == key) {
                return this.values[i];
            }
        }
        return Bucket.NOT_FOUND;
    }

    del(key) {
        for (var i = 0; i < this.keys.length; i++) {
            if (this.keys[i] == key) {
                for (var j = i; j < this.keys.length - 1; j++) {
                    this.keys[j] = this.keys[j + 1];
                    this.values[j] = this.values[j + 1];
                }
                return true;
            }
        }
        return false;
    }    
}

class Iterator {  
    constructor() {
        this.table = null;
        this.cur_bucket = 0;
        this.cur_key = 0;
        this.key = null;
        this.value = null;        
    }

    next() {
        var table = this.table;
        while (this.cur_bucket < table.num_buckets) {
            var bucket = table.buckets[this.cur_bucket];
            if (this.cur_key < bucket.keys.length) {
                this.key = bucket.keys[this.cur_key];
                this.value = bucket.values[this.cur_key];
                this.cur_key += 1;
                return this;
            } else {
                this.cur_key = 0;
            }
            this.cur_bucket += 1;
        }
        return null;
    }    
}

class HashMap {  
    iterator() {
        var iter = new Iterator();
        iter.table = this;
        iter.cur_bucket = 0;
        iter.cur_key = 0;
        this.iter = iter;
        return iter;
    }      

    constructor() {
        this.buckets = [];
        this.count = 0;
        this.num_buckets = 8;
        for (var i = 0; i < this.num_buckets; i++) {
            this.buckets.push(new Bucket());
        }
        this.iter = null;
    }

    static hash(key) { // Simplest hash function
        // int h = 0;
        // int size = sizeof(Integer);
        // char* p = (char*) &key;
        // char prime[8] = {2, 3, 7, 11, 13, 17, 19, 23};
        // for (int i = 0; i < size; i++) {
        //     h += p[i] ^ prime[i]; // bitwise XOR
        //     h <<= 8;
        // }
        // return h;
        return key + 42;
    }

    resize() {
        if (this.count <= parseInt(this.num_buckets * 0.75)) {
            return;
        }
        // 새로운 buckets을 할당한다
        var new_count = 0;
        var new_num_buckets = this.num_buckets * 2;
        var new_buckets = [];
        for (var i = 0; i < new_num_buckets; i++) {
            new_buckets.push(new Bucket());
        }
        // 새로운 buckets에 기존 값들을 모두 옮겨 담는다
        var iter = this.iterator();
        while (true) {
            iter = iter.next();
            if (iter == null) {
                break;
            }
            var h = HashMap.hash(iter.key);
            var which_bucket = h % new_num_buckets;
            var inserted = new_buckets[which_bucket].put(iter.key, iter.value);
            if (inserted == true) {
                new_count += 1;
            }
        }
 
        // 새로운 buckets를 This에 할당
        this.count = new_count;
        this.num_buckets = new_num_buckets;
        this.buckets = new_buckets;
    }

    put(key, value) {
        var h = HashMap.hash(key);
        var which_bucket = h % this.num_buckets;
        var inserted = this.buckets[which_bucket].put(key, value); 
        if (inserted == true) {
            this.count += 1;
            this.resize();
            return true;
        }
        return false;
    }

    get(key) {
        var h = HashMap.hash(key);
        var which_bucket = h % this.num_buckets;
        var bucket = this.buckets[which_bucket];
        return bucket.get(key);
    }

    del(key) {
        var h = HashMap.hash(key);
        var which_bucket = h % this.num_buckets;
        var bucket = this.buckets[which_bucket];
        if (bucket.del(key)) this.count -= 1;
    }

    print() {
        var iter = this.iterator();
        while (iter.next() != null) {
            // iter = Iterator_next(iter);
            // if (iter == NULL) break;
            process.stdout.write("(" + iter.key + "," + iter.value + ")\n");
        }
    }
    
    keys() {
        var key_s = [];
        var iter = this.iterator();
        while (iter.next() != null) {
            key_s.push(iter.key);
        }
        return key_s;
    }
    
    values() {
        var value_s = [];
        var iter = this.iterator();
        while (iter.next() != null) {
            value_s.push(iter.value);
        }
        return value_s;
    }

    get_count() {
        return this.count;
    }

    contains(key) {
        return (this.get(key) != Bucket.NOT_FOUND);
    }
}

class Set extends HashMap {
    constructor() {
        super();
    }

    has(element) {
        return this.contains(element);
    }

    add(element) {
        this.put(element, 1);
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
}

// function swap(array, i, k) {
//     var temp = array[i];
//     array[i] = array[k];
//     array[k] = temp;
// }

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
    var table = new HashMap();
    table.put(5, 500);
    table.put(7, 700);
    table.put(1, 100);
    table.put(3, 300);
    table.put(2, 200);
    table.put(15, 1500);
    table.put(17, 1700);
    table.put(11, 1100);
    table.put(13, 1300);
    table.put(12, 1200);
    table.print();
    var val = table.get(3);
    process.stdout.write("val : " + val + "\n");
    var val2 = table.get(9);
    if (val2 == Bucket.NOT_FOUND) {
        process.stdout.write("key = " + 9 + " not found\n");
    } else {
        process.stdout.write("val : " + val2 + "\n");
    }
    table.del(17);
    table.print();

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

    var array1 = setw.keys();
    let comp = function (a, b) {
        return b - a;
    };
    print_arr(array1);
    qsort(array1, 0, array1.length - 1, comp);
    print_arr(array1);    
}

main();