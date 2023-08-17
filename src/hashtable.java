import java.util.ArrayList;
import java.util.Comparator;

class Bucket {  
    static final int NOT_FOUND = -999;
    ArrayList<Integer> keys;
    ArrayList<Integer> values;

    public void init() {
        this.keys = new ArrayList<Integer>();
        this.values = new ArrayList<Integer>();
    }

    public boolean put(Integer key, Integer value) {
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) == key) {
                this.values.set(i, value);
                return false;
            }
        }
        this.keys.add(key);
        this.values.add(value);
        return true;
    }

    public Integer get(Integer key) {
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) == key) {
                return this.values.get(i);
            }
        }
        return NOT_FOUND;
    }

    public boolean del(Integer key) {
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) == key) {
                for (int j = i; j < this.keys.size() - 1; j++) {
                    this.keys.set(j, this.keys.get(j + 1));
                    this.values.set(j, this.values.get(j + 1));
                }
                return true;
            }
        }
        return false;
    }    
}

class AHashMap {  
    ArrayList<Bucket> buckets;
    int count; // AHashMap이 담고 있는 (key, value)의 개수
    int num_buckets; // bucket 개수 8, 16, ......
    Iterator iter;
    
    public class Iterator {  
        AHashMap table;
        int cur_bucket;
        int cur_key;
        Integer key;
        Integer value;

        public Iterator next() {
            AHashMap table = this.table;
            assert(table != null);
            while (this.cur_bucket < table.num_buckets) {
                Bucket bucket = table.buckets.get(this.cur_bucket);
                assert(bucket != null);
                if (this.cur_key < bucket.keys.size()) {
                    this.key = bucket.keys.get(this.cur_key);
                    this.value = bucket.values.get(this.cur_key);
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

    public Iterator iterator() {
        Iterator iter = new Iterator();
        iter.table = this;
        iter.cur_bucket = 0;
        iter.cur_key = 0;
        this.iter = iter;
        return iter;
    }

    public AHashMap() {
        this.buckets = new ArrayList<Bucket>();
        this.count = 0;
        this.num_buckets = 8;
        for (int i = 0; i < this.num_buckets; i++) {
            this.buckets.add(new Bucket());
            this.buckets.get(i).init();
        }
        this.iter = null;
    }

    static int hash(Integer key) { // Simplest hash function
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

    private void resize() {
        if (this.count <= (int) ((float) this.num_buckets * 0.75)) {
            return;
        }
        // 새로운 buckets을 할당한다
        int new_count = 0;
        int new_num_buckets = this.num_buckets * 2;
        ArrayList<Bucket> new_buckets = new ArrayList<Bucket>();
        for (int i = 0; i < new_num_buckets; i++) {
            new_buckets.add(new Bucket());
            new_buckets.get(i).init();
        }
        // 새로운 buckets에 기존 값들을 모두 옮겨 담는다
        Iterator iter = this.iterator();
        while (true) {
            iter = iter.next();
            if (iter == null) {
                break;
            }
            int h = hash(iter.key);
            int which_bucket = h % new_num_buckets;
            boolean inserted = new_buckets.get(which_bucket).put(iter.key, iter.value);
            if (inserted == true) {
                new_count += 1;
            }
        }
 
        // 새로운 buckets를 This에 할당
        this.count = new_count;
        this.num_buckets = new_num_buckets;
        this.buckets = new_buckets;
    }

    public boolean put(Integer key, Integer value) {
        int h = hash(key);
        int which_bucket = h % this.num_buckets;
        boolean inserted = this.buckets.get(which_bucket).put(key, value); 
        if (inserted) {
            this.count += 1;
            this.resize(); // 해쉬테이블의 buckets 크기를 늘린다
            return true;
        }
        return false;
    }

    public Integer get(Integer key) {
        int h = hash(key);
        int which_bucket = h % this.num_buckets;
        Bucket bucket = this.buckets.get(which_bucket);
        return bucket.get(key);    
    }

    public void del(Integer key) {
        int h = hash(key);
        int which_bucket = h % this.num_buckets;
        Bucket bucket = this.buckets.get(which_bucket);
        if (bucket.del(key)) this.count -= 1;
        // if (this.contains(key)) this.count -= 1;
        // bucket.del(key);
    }

    public void print() {
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            // iter = Iterator_next(iter);
            // if (iter == NULL) break;
            System.out.printf("(%d, %d)\n", iter.key, iter.value);
        }
    }
    
    public ArrayList<Integer> keys() {
        ArrayList<Integer> key_s = new ArrayList<Integer>();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            // iter = Iterator_next(iter);
            // if (iter == NULL) break;
            // System.out.printf("(%d, %d)\n", iter.key, iter.value);
            key_s.add(iter.key);
        }
        return key_s;
    }

    public ArrayList<Integer> values() {
        ArrayList<Integer> value_s = new ArrayList<Integer>();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            // iter = Iterator_next(iter);
            // if (iter == NULL) break;
            // System.out.printf("(%d, %d)\n", iter.key, iter.value);
            value_s.add(iter.value);
        }
        return value_s;
    }

    public boolean contains(Integer key) {
        return (this.get(key) != Bucket.NOT_FOUND);
    }

    public int get_count() {
        return this.count;
    }
    
    public void qsort(ArrayList<Integer> array, int left, int right, MyComp comp) {
        if (right <= left) return;

        int pivot = array.get(right);
        int pivotIdx = left;
        for (int i = left; i < right; i++) {
            if (comp.compare(array.get(i), pivot) > 0) {
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

class MyComp implements Comparator<Integer> {
    public int compare(Integer a, Integer b) {
        return b - a;
    }
}

class ASet extends AHashMap {
    public ASet() {}

    public boolean has(Integer element) {
        return this.contains(element);
    }

    public void add(Integer element) {
        this.put(element, 1);
    }

    public void remove(Integer element) {
        this.del(element);
    }

    public int count() {
        return this.get_count();
    }

    public ASet intersection(ASet set2) {
        ASet set3 = new ASet();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            if (set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    public ASet union(ASet set2) {
        ASet set3 = new ASet();
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

    public ASet difference(ASet set2) {
        ASet set3 = new ASet();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            if (! set2.has(iter.key)) {
                set3.add(iter.key);
            }
        }
        return set3;
    }

    public ASet symmetric_difference(ASet set2) {
        ASet set3 = new ASet();
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
        System.out.printf("Set.count: %d {", this.count());
        Iterator iter = this.iterator();
        for (int i = 0; iter.next() != null; i++) {
            if (i == this.count() - 1) {
                System.out.printf("%d", iter.key);
            } else {
                System.out.printf("%d, ", iter.key);
            }
        }
        System.out.printf("}\n");
    }
} 

public class hashtable {
    public static void main(String[] args) {
        AHashMap table = new AHashMap();
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
        Integer val = table.get(3);
        System.out.printf("val : %d\n", val);
        Integer val2 = table.get(9);
        if (val2 == Bucket.NOT_FOUND) {
            System.out.printf("key = %d not found\n", 9);
        } else {
            System.out.printf("val : %d\n", val2);
        }

        table.del(17);
        table.print();

        System.out.printf("----------\n");
        ArrayList<Integer> keys = table.keys();
        table.qsort(keys, 0, keys.size() - 1, new MyComp());
        for (int key : keys) {
            System.out.printf("key: %d, value: %d\n", key, table.get(key));
        }
        System.out.printf("----------\n");

        ASet setx = new ASet();
        setx.add(23);
        setx.add(37);
        setx.add(41);
        setx.add(23);
        setx.add(59);
        setx.add(73);
        setx.add(89);
        setx.add(97);

        setx.print();
        ASet sety = new ASet();
        sety.add(37);
        sety.add(67);
        sety.add(83);
        sety.add(41);
        sety.print();
        ASet setz = setx.intersection(sety);
        setz.print();
        ASet setw = setx.union(sety);
        setw.print();
        ASet setu = setx.difference(sety);
        setu.print();
        ASet setk = sety.difference(setx);
        setk.print();
        ASet setl = setx.symmetric_difference(sety);
        setl.print();
    }
}