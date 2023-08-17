class Data: 
    def __init__(self, age: int, name: str):
        self.age: int = age
        self.name: str = name
    
    def getAge(self)->int: # encapsulation
        return self.age
    
    def getName(self)->str:
        return self.name

    def setAge(self, age: int):
        self.age = age
    
    def setName(self, name: str):
        self.name = name

    def print(self):
        print("(%d, %s)" % (self.age, self.name), end = "")

    def equals(self, data) -> bool:
        if (self.age != data.age):
            return False
        if (self.name != data.name):
            return False
        return True     

class Node:
    def __init__(self, data_: Data): # 생성자(constructor)
        self.data: Data = data_
        self.prev: Node = None
        self.next: Node = None

    def print(self):
        self.data.print()

class AList:
    class Iterator:    
        def __init__(self):
            self.list: AList = None
            self.next_node: Node = None
            self.node: Node = None
            self.data: Data = None
            
        def next(self):
            if (self.next_node == None):
                return None
            self.node = self.next_node
            self.data = self.node.data
            self.next_node = self.node.next
            return self

    def iterator(self) -> Iterator:
        iter: self.Iterator = self.Iterator()
        iter.list = self
        iter.next_node = self.head
        iter.data = None
        iter.node = None
        self.iter = iter
        return iter

    def __init__(self):
        self.head: Data = None
        self.count_: int = 0
        self.iter: self.Iterator = None

    def prepend_item(self, data: Data):
        # node: Node = Node(data) # data를 이용해서 node를 하나 생성한다
        
        # if (self.head == None): # list가 비어 있으면 
        #     self.head = node    # node를 list->head에 바로 할당
        # else: 
        #     prev_head: Node = self.head # 기존에 head에 있는 node를 temp에 피신시켜두고
        #     self.head = node       # list->head에 새로 할당한 node를 할당
        #     node.next = prev_head  # 피신시켜놓은 기존 head를 새로 할당한 node 꽁무니에 연결해준다
        #     prev_head.prev = node  # 피신시켜놓은 prev_head->prev를 새로 들어온 node에 연결한다

        # self.count_ += 1
        self.insert_at(0, data)

    def print(self):
        cur_node: None = self.head
        
        print("count_ : %d " % (self.count_), end = "")
        while (cur_node != None): # 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            cur_node.data.print()
            cur_node = cur_node.next
        print("\n", end = "")

    def get_count(self)->int:
        return self.count_

    def append_item(self, data: Data):
        # node: Node = Node(data)
        # cur_node: Node = self.head
        # self.count_ += 1
        # if (self.head == None): # list가 비어있으면
        #     self.head = node    # list->head에 node를 연결해주고 끝낸다
        #     return
        # while (cur_node != None): # 현재 node가 None이 아닌 동안에 
        #     if (cur_node.next == None): # 마지막 node면
        #         cur_node.next = node    # node를 연결해주고 끝낸다
        #         node.prev = cur_node
        #         return
        #     cur_node = cur_node.next # 다음 node로 넘어간다
        self.insert_at(self.get_count(), data)
        
    def get_item(self, index: int)->Data:
        i: int = 0
        if (index < 0):
            return None
        
        cur_node: Node = self.head
        while (cur_node != None):
            if (index == i):
                return cur_node.data
            cur_node = cur_node.next
            i += 1
        return None
    
    def remove_item(self, data: Data):
        cur_node: Node = self.head
        prev_node: Node = None # 최초에는 이전 node가 없다
        while (cur_node != None): # 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            next_node: Node = cur_node.next
            if (cur_node.data.equals(data)): # 지워야 할 node가 식별됨
                self.count_ -= 1 
                if (prev_node != None):
                    if (next_node == None):
                        prev_node.next = None
                    else:
                        prev_node.next = next_node # 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node.prev = prev_node
                    
                else: # 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == None):
                         self.head = next_node
                    else:
                        self.head = next_node #list->head를 다음 node로 연결해 준다
                        next_node.prev = None
            prev_node = cur_node # 이전 노드를 기억
            cur_node = next_node # 다음 노드로 이동
        
    def remove_at(self, index: int)->Data:
        cur_node: Node = self.head
        prev_node: Node = None
        i: int = 0
        if (index < 0): return None
        
        cur_data: Data = None
        while (cur_node != None):
            next_node: Node = cur_node.next
            if (index == i):
                cur_data = cur_node.data # 현재 node의 data를 피신시켜두고
                cur_node.data = None # 현재 node의 data link를 끊고
                self.count_ -= 1
                if (prev_node != None): 
                    if (next_node == None):
                        prev_node.next = None
                    else:
                        prev_node.next = next_node # 이전 node의 next를 다음 node로 연결하면 현재 node는 배제된다
                        next_node.prev = prev_node
                else: # 이전 node가 없다는 것은 현재 node가 맨 앞의 node라는 뜻이므로
                    if (next_node == None):
                        self.head = next_node
                    else:
                        self.head = next_node #list->head를 다음 node로 연결해 준다
                        next_node.prev = None
            prev_node = cur_node # 이전 노드를 기억
            cur_node = next_node # 다음 노드로 이동
            i += 1
        return cur_data # 피신시켜놓은 현재 node의 data를 리턴한다
    
    def find(self, data: Data) -> int:
        cur_node: Node = self.head
        i: int = 0
        
        while (cur_node != None): # 연쇄적으로 맨 앞에 있는 node부터 node 각각을 print한다
            if (cur_node.data.equals(data)):
                return i
            cur_node = cur_node.next
            i += 1
        return -1

    def insert_at(self, index: int, data: Data) -> bool:
        if (index < 0 or index > self.get_count()): # index 범위를 벗어나면 리턴 false 
            return False
        new_node: Node = Node(data) # 새로 추가할 node 생성
        cur_node: Node = self.head
        prev_node: Node = None
        i: int = 0
        while (cur_node != None): # 현재 node가 null인지 여부 검사
            if (index == i): # 추가할 index 위치를 찾으면 그곳에 끼워 넣는다
                if (prev_node == None): # 이전 node가 없으면 뒤로만 연결하면 됨
                    new_node.next = cur_node # 뒤로 연결
                    cur_node.prev = new_node 
                    self.head = new_node # 새로운 node가 list의 head가 됨
                else: # 앞뒤로 다 연결
                    new_node.next = cur_node # 뒤로 연결 
                    cur_node.prev = new_node
                    prev_node.next = new_node # 앞으로 연결
                    new_node.prev = prev_node
                return True # 리턴
            prev_node = cur_node
            cur_node = cur_node.next # 뒤로 이동 
            i += 1
        if (prev_node == None):
            self.head = new_node
        else:
            prev_node.next = new_node
            new_node.prev = prev_node
        self.count_ += 1
        return True
    
    def sort(self):
        array = []
        iter = self.iterator()
        while (iter.next() != None):
            array.append(iter.node)
        
        qsort(array, 0, len(array) - 1, compare)
        for i in range(0, len(array)):
            print("[%d, %s]" % (array[i].data.age, array[i].data.name), end = "")
        print(" ")
        
def compare(a, b):
    diff = b.data.age - a.data.age
    if (diff != 0): return diff
    if (b.data.name > a.data.name): return 1
    if (b.data.name < a.data.name): return -1
    return 0

def qsort(array, left, right, compf):
    if (right <= left): return

    pivot = array[right]
    pivotIdx = left 
    for i in range(left, right):
        if (compf(array[i], pivot) > 0):
            array[i].data, array[pivotIdx].data = array[pivotIdx].data, array[i].data
            pivotIdx += 1
    array[right].data, array[pivotIdx].data = array[pivotIdx].data, array[right].data
    # array[right].data = array[pivotIdx].data
    # array[pivotIdx].data = pivot.data

    qsort(array, left, pivotIdx - 1, compf)
    qsort(array, pivotIdx + 1, right, compf)    
    
class AStack (AList):    
    def __init__(self):
        super().__init__() # 상속받은 super class를 초기화해준다

    def push(self, data: Data):
        self.append_item(data)
    
    def pop(self)->Data:
        return self.remove_at(self.get_count() - 1)

    def print(self):
        super().print()
    
class AQue (AList):  
    def __init__(self):
        super().__init__() # 상속받은 super class를 초기화해준다

    def enqueue(self, data: Data):
        self.append_item(data)
    
    def dequeue(self)->Data:
        return self.remove_at(0)
    
    def print(self):
        super().print()
    
def main():
    list: AList = AList()
    data: Data = Data(20, "Jinho")
    list.prepend_item(data)
    data_2: Data = Data(52, "Hiongun")
    list.prepend_item(data_2)
    data_3: Data = Data(50, "Sujung")
    list.prepend_item(data_3)
    data_4: Data = Data(20, "Junho")
    list.prepend_item(data_4)
    data_5: Data = Data(20, "Junhyuk")
    list.prepend_item(data_5)
    list.print()
    list.insert_at(1, Data(20, "Yongjun"))
    list.print()
    idx: int = list.find(Data(20, "Yongju"))
    print("idx = %d\n" % idx)

    print("-----")
    list.sort()
    iter: AList.Iterator = list.iterator()
    while (iter.next() != None):
        data_: Data = iter.data
        data_.print()
    print("\n-----")    

    data_6: Data = Data(20, "Min")
    list.append_item(data_6)
    print("%d\n" % (list.get_count()), end= "")
    list.remove_item(data_4)
    print("%d\n" % (list.get_count()), end = "")
    list.print()
    s1: AStack = AStack()
    d1: Data = Data(20, "김유찬")
    s1.push(d1)
    d2: Data = Data(20, "김준")
    s1.push(d2)
    s1.print()
    dx: Data= s1.pop()
    if (dx != None):
        dx.print(); print("\n", end = "")
    
    dx: Data = s1.pop()
    if (dx != None):
        dx.print(); print("\n", end = "")
    
    dx: Data = s1.pop()
    if (dx != None):
        dx.print(); print("\n", end = "")
    
    d3: Data = Data(20, "안재형")
    s1.push(d3)
    s1.print()
    
    q: AQue = AQue()
    d11: Data = Data(20, "김유찬")
    q.enqueue(d11)
    d12: Data = Data(20, "김태환")
    q.enqueue(d12)
    d13: Data = Data(20, "이호준")
    q.enqueue(d13)
    d14: Data = q.dequeue()
    if (d14 != None):
        d14.print()
    q.print(); print("\n", end = "")

main()