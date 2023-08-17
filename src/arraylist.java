import java.util.ArrayList;
import java.util.Comparator;

class Data {
    public int age;
    public String name;
    
    public Data(int age, String name) {
        this.age = age;
        this.name = name;
    }

    public boolean equals(Data data) { // this == data
        if (this.age != data.age) return false; 
        if (this.name != data.name) return false;
        return true;
    }

    public void print() {
        System.out.printf("(%d, %s)", this.age, this.name);
    }

    public static void main(String[] args) {
        System.out.printf("Data class main\n");
    }
}

class Node {
    public Data data;
    
    public Node(Data data) { // data를 포함하는 Node 객체 하나를 초기화해서 리턴한다
        this.data = data;
    }

    public void print() {
        this.data.print();
    }
}

class MyComp implements Comparator<Node> {
    public int compare(Node a, Node b) {
        int diff = b.data.age - a.data.age;
        if (diff != 0) return diff;
        if (b.data.name.compareTo(a.data.name) > 0) return 1;
        if (a.data.name.compareTo(b.data.name) > 0) return -1;
        return 0;
    }
}

class AList {
    class Iterator {
        public AList list;
        public int cur_index;
        public Data data;

        public Iterator next() {
            if (this.cur_index >= this.list.get_count()) {
                return null;
            }
            this.data = this.list.array.get(this.cur_index).data;
            this.cur_index += 1;
            return this;
        }
    }

    public Iterator iterator() {
        Iterator iter = new Iterator();
        iter.list = this;
        iter.cur_index = 0;
        iter.data = null;
        this.iter = iter;
        return iter;
    }
    
    public ArrayList<Node> array; // node포인터들을 담고 있는 array
    public Iterator iter;
    
    // List를 생성하는 함수 
    public AList() { 
        this.array = new ArrayList<Node>();
        this.iter = null;
    }

    public void prepend_item(Data data) { // List에 data를 추가하는 함수
        // Node node = new Node(data); // data를 이용해서 node를 하나 생성한다
        // this.array.add(node);
        this.insert_at(0, data);
    }

    public void print() { // 
        for(int i = 0; i < this.array.size(); i++) {
            this.array.get(i).print();                                    
        }
        System.out.printf("\n");
    }

    public void remove_item(Data data) { // list에서 data 삭제
        for (int i = 0; i < this.array.size(); i++) { 
            if (this.array.get(i).data.equals(data)) { // 지울 data가 들어 있는 node를 찾으면 
                this.array.remove(i); // 해당 node를 지운다
            }             
        }
    }

    public int get_count() {
        return this.array.size();
    }

    public void append_item(Data data) { // list에 data를 덧붙이는 함수
        // Node node = new Node(data); // 입력받는 data로 node 생성
        // this.array.add(node);
        this.insert_at(this.get_count(), data);
    }

    public Data get_item(int index) {
        if (index < 0 || index >= this.array.size()) {
            return null;
        }
        return this.array.get(index).data;
    }

    public Data remove_at(int index) {
        if (index < 0 || index >= this.array.size()) {
            return null;
        }
        // Node_delete(list.array[index]);
        Data data = this.array.get(index).data; // 일단 data를 피신시키고 
        this.array.get(index).data = null; // node와 data와의 연결을 끊고
        this.array.remove(index); // node포인터도 지운다 
        return data;
    }

    public int find(Data data) {
        for (int i = 0; i < this.array.size(); i++) {
            if (this.array.get(i).data.equals(data)) {
                return i;
            }
        }
        return -1;
    }

    public boolean insert_at(int index, Data data) {
        if (index < 0 || index > this.array.size()) return false;
        Node node = new Node(data);
        
        if (index < this.get_count()) {
            this.array.add(index, node); // insert(index, node)
        } else {
            this.array.add(node);
        }
        return true;
    }

    public void sort() {
        qsort(this.array, 0, this.array.size() - 1, new MyComp());
    }

    public static void qsort(ArrayList<Node> array, int left, int right, MyComp comp) {
        if (right <= left) return;

        Node pivot = array.get(right);
        int pivotIdx = left;
        for (int i = left; i <right; i++) {
            if (comp.compare(array.get(i), pivot) > 0) {
                Data temp = array.get(i).data;
                array.get(i).data = array.get(pivotIdx).data;
                array.get(pivotIdx).data = temp;
                pivotIdx += 1;                
            }
        }
        Data temp = array.get(right).data;
        array.get(right).data = array.get(pivotIdx).data;
        array.get(pivotIdx).data = temp;

        qsort(array, left, pivotIdx - 1, comp);
        qsort(array, pivotIdx + 1, right, comp);
    }
}

class AStack extends AList { // Stack이 List를 상속받는다(Stack inherits List, Stack extends List)
    public AStack() {

    }

    public void push(Data data) {
        this.append_item(data);
    }

    public Data pop() {
        int count = this.get_count();
        Data data = this.remove_at(count - 1);
        return data;
    }     
}

class AQue extends AList {

    public AQue() {
        
    }

    public void enqueue(Data data) {
        this.append_item(data);
    }

    public Data dequeue() { 
        return this.remove_at(0);
    }
}

public class arraylist {
    public static void main(String[] args) {
        AList list = new AList();
        Data data = new Data(20, "Jinho");
        list.prepend_item(data);
        Data data_2 = new Data(52, "Hiongun");
        list.prepend_item(data_2);
        Data data_3 = new Data(50, "Sujung");
        list.prepend_item(data_3);
        Data data_4 = new Data(20, "Junho");
        list.prepend_item(data_4);
        Data data_5 = new Data(20, "Junhyuk");
        list.prepend_item(data_5);
        list.print();
        list.insert_at(1, new Data(20, "Yongjun"));
        list.print();
        var idx = list.find(new Data(20, "Yongju"));
        System.out.printf("idx = %d\n", idx);        
        Data data_6 = new Data(20, "Min");
        list.append_item(data_6);
        System.out.printf("%d\n", list.get_count());
        list.remove_item(data_4);
        System.out.printf("%d\n", list.get_count());
        list.print();

        list.sort();
        System.out.printf("-----\n");
        AList.Iterator iter = list.iterator();        
        while (iter.next() != null) {
            Data data_ = iter.data;
            data_.print();
        }
        System.out.printf("\n-----\n");
        
        AStack s1 = new AStack();
        Data d1 = new Data(20, "김유찬");
        s1.push(d1);
        Data d2 = new Data(20, "김준");
        s1.push(d2);
        s1.print();
        Data dx = s1.pop();
        if (dx != null) {
            dx.print(); System.out.printf("\n");
        }
        dx = s1.pop();
        if (dx != null) {
            dx.print(); System.out.printf("\n");
        }
        dx = s1.pop();
        if (dx != null) {
            dx.print(); System.out.printf("\n");
        }
        Data d3 = new Data(20, "안재형");
        s1.push(d3);
        s1.print();
        
        AQue q = new AQue();
        Data d11 = new Data(20, "김유찬");
        q.enqueue(d11);
        Data d12 = new Data(20, "김태환");
        q.enqueue(d12);
        Data d13 = new Data(20, "이호준");
        q.enqueue(d13);
        Data d14 = q.dequeue();
        if (d14 != null) {
            d14.print();
        } 
        q.print(); System.out.printf("\n");
    }
}