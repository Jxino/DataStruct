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
            self.cur_index: int = 0
            self.data: Data = None

        def next(self):
            if (self.cur_index >= self.list.get_count()):
                return None
            self.data = self.list.array[self.cur_index].data
            self.cur_index += 1
            return self

    def iterator(self) -> Iterator:
        iter: self.Iterator = self.Iterator()
        iter.list = self
        iter.cur_index = 0
        iter.data = None
        self.iter = iter
        return iter

    # List를 생성하는 함수 
    def __init__(self): 
        self.array = []
        self.iter = None
    
    def prepend_item(self, data: Data): # List에 data를 추가하는 함수
        # node: Node = Node(data) # data를 이용해서 node를 하나 생성한다
        # self.array.append(node)
        self.insert_at(0, data)
    
    def print(self): # 
        for element in self.array:
            element.print()                                     
        print("\n", end = "")

    def remove_item(self, data: Data): # list에서 data 삭제
        for i, element in enumerate(self.array):
            if (element.data.equals(data)): # 지울 data가 들어 있는 node를 찾으면 
                del self.array[i] # 해당 node를 지운다
                break
             
    def get_count(self) -> int:
        return len(self.array)
    
    def get(self, index: int) -> Node:
        return self.array[index]

    def append_item(self, data: Data): # list에 data를 덧붙이는 함수
        # node: Node = Node(data) # 입력받는 data로 node 생성
        # self.array.append(node)
        self.insert_at(self.get_count(), data)
    
    def get_item(self, index: int) -> Data:
        if (index < 0 or index >= len(self.array)):
            return None
        
        return self.array[index].data
    
    def remove_at(self, index: int) -> Data:
        if (index < 0 or index >= len(self.array)):
            return None
        
        # Node_delete(list->array[index])
        data: Data = self.array[index].data # 일단 data를 피신시키고 
        self.array[index].data = None # node와 data와의 연결을 끊고
        del self.array[index] # node포인터도 지운다 
        return data

    def find(self, data: Data) -> int:
        for i, element in enumerate(self.array):
            if (element.data.equals(data)):
                return i
        return -1

    def insert_at(self, index: int, data: Data) -> bool:
        if (index < 0 or index > len(self.array)): 
            return False
        node: Node = Node(data)
        if (index < self.get_count()):
            self.array.insert(index, node) # insert(index, node)
        else:
            self.array.append(node)
        return True
    
    def sort(self):
        qsort(self.array, 0, len(self.array) - 1, compare)
        
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
    data_6: Data = Data(20, "Min")
    list.append_item(data_6)
    print("%d\n" % (list.get_count()), end= "")
    list.remove_item(data_4)
    print("%d\n" % (list.get_count()), end = "")
    list.print()

    print("-----")
    list.sort()
    iter: AList.Iterator = list.iterator()
    while (iter.next() != None):
        data_: Data = iter.data
        data_.print()
    print("\n-----")    
    
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