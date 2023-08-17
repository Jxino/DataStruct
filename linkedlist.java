import java.util.ArrayList;
import java.util.Comparator;

class Data {
    private int age;
    private String name;
    
    public int getAge() { // data encapsulation
        return this.age;
    }
    
    public boolean equals(Data data) { // this == data
        if (this.age != data.age) return false; 
        if (this.name != data.name) return false;
        return true;
    }

    public String getName() {
        return this.name;
    }

    public void setAge(int age) {
        this.age = age;
    }
    
    public void setName(String name) {
        this.name = name;
    }


    public Data(int age, String name) {
        this.age = age;
        this.name = name;
    }

    public void print() {
        System.out.printf("(%d, %s)", this.age, this.name);
    }
}

class Node {
    public Data data;
    public Node prev;
    public Node next;
    
    public Node(Data data_) { // 생성자(constructor)
        this.data = data_;
        this.prev = null;
        this.next = null;
    }

    public void print() {
        this.data.print();
    }
}

class AList {    
    public int count_; // list에 들어 있는 node 개수를 기억한다
    public Node head;
    public Iterator iter;

    public class Iterator {    
        public AList list;
        public Node next;
        public Node node;
        public Data data;
    
        public Iterator next() {
            if (this.next == null) {
                return null;
            }
            this.data = this.next.data;
            this.node = this.next;
            this.next = this.node.next;
            return this;
        }    
    }

    Iterator iterator() {
        Iterator iter = new Iterator();
        iter.list = this;
        iter.next = this.head;
        iter.node = null;
        iter.data = null;
        this.iter = iter;
        return iter;
    }    

    public AList() {
        this.head = null;
        this.count_ = 0;
        this.iter = null; 
    }

    public void prepend_item(Data data) {
        // Node node = new Node(data); // data를 이용해서 node를 하나 생성한다
        
        // if (this.head == null) { // list가 비어 있으면 
        //     this.head = node;    // node를 list.head에 바로 할당
        // } else { 
        //     Node prev_head = this.head; // 기존에 head에 있는 node를 temp에 피신시켜두고
        //     this.head = node;       // list.head에 새로 할당한 node를 할당
        //     node.next = prev_head;  // 피신시켜놓은 기존 head를 새로 할당한 node 꽁무니에 연결해준다
        //     prev_head.prev = node;  // 피신시켜놓은 prev_head.prev를 새로 들어온 node에 연결한다
        // }

        // this.count_ += 1;
        this.insert_at(0, data);
    }

    public void print() {
        Node cur_node = this.head;
        
        System.out.printf("count_ : %d ", this.count_);
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            cur_node.data.print();
            cur_node = cur_node.next;
        }
        System.out.printf("\n");
    }

    public int get_count() {
        return this.count_;
    }

    public void append_item(Data data) {
        // Node node = new Node(data);
        // Node cur_node = this.head;

        // this.count_ += 1;
        
        // if (this.head == null) { // list가 비어있으면
        //     this.head = node;    // list.head에 node를 연결해주고 끝낸다
        //     return;
        // }

        // while (cur_node != null) { // 현재 node가 null이 아닌 동안에 
        //     if (cur_node.next == null) { // 마지막 node면
        //         cur_node.next = node;    // node를 연결해주고 끝낸다
        //         node.prev = cur_node;
        //         return;
        //     }
        //     cur_node = cur_node.next; // 다음 node로 넘어간다
        // }
        this.insert_at(this.get_count(), data);
    }

    public Data get_item(int index) {
        int i = 0;
        if (index < 0) {
            return null;
        }
        
        Node cur_node = this.head;
        while (cur_node != null) {
            if (index == i) {
                return cur_node.data;
            }
            cur_node = cur_node.next;
            i += 1;
        }
        return null;
    }

    public void remove_item(Data data) {
        Node cur_node = this.head;
        Node prev_node = null; // 최초에는 이전 node가 없다
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            Node next_node = cur_node.next;
            if (cur_node.data.getName() == data.getName() && cur_node.data.getAge() == data.getAge()) { // 지워야 할 node가 식별됨
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
                        this.head = next_node; //list.head를 다음 node로 연결해 준다
                        next_node.prev = null;
                    }
                }
            } 
            prev_node = cur_node; // 이전 노드를 기억
            cur_node = next_node; // 다음 노드로 이동
        }
    }

    public Data remove_at(int index) {
        Node cur_node = this.head;
        Node prev_node = null;
        int i = 0;
        if (index < 0) return null;
        
        Data cur_data = null;
        while (cur_node != null) {
            Node next_node = cur_node.next;
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
                        this.head = next_node; //list.head를 다음 node로 연결해 준다
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

    public int find(Data data) {
        Node cur_node = this.head;
        int i = 0;
        
        while (cur_node != null) { // 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            if (cur_node.data.equals(data)) {
                return i;
            }
            cur_node = cur_node.next;
            i += 1;
        }
        return -1;
    }

    public boolean insert_at(int index, Data data) {
        if (index < 0 || index > this.get_count()) // index 범위를 벗어나면 리턴 false 
            return false;
        Node new_node = new Node(data); // 새로 추가할 node 생성
        Node cur_node = this.head;
        Node prev_node = null;
        int i = 0;
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
        assert(index == this.get_count());
        if (prev_node == null) {
            assert(index == 0 && this.get_count() == 0);
            this.head = new_node;
        } else {
            prev_node.next = new_node;
            new_node.prev = prev_node;
        }
        this.count_ += 1;
        return true;
    }

    public void sort() {
        ArrayList<Node> array = new ArrayList<Node>();
        Iterator iter = this.iterator();
        while (iter.next() != null) {
            array.add(iter.node);
        }
        qsort(array, 0, array.size() - 1, new MyComp());
    }

    public static void qsort(ArrayList<Node> array, int left, int right, MyComp comp) {
        if (right <= left) return;
        
        Node pivot = array.get(right);
        int pivotIdx = left;
        for (int i = left; i < right; i++) {
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

class MyComp implements Comparator<Node> {
    public int compare(Node a, Node b) {
        int diff = b.data.getAge() - a.data.getAge();
        if (diff != 0) return diff;
        if (b.data.getName().compareTo(a.data.getName()) > 0) return 1;
        if (a.data.getName().compareTo(b.data.getName()) > 0) return -1;
        return 0;
    }
}

class AStack extends AList {    

    public AStack() {

    }

    public void push(Data data) {
        this.append_item(data);
    }

    public Data pop() {
        return this.remove_at(this.get_count() - 1);
    }

    public void print() {
        super.print();
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

    public void print() {
        super.print();
    }
}

public class linkedlist {
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