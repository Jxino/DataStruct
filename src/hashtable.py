class Bucket:  
    NOT_FOUND = -999

    def __init__(self):
        self.keys = []
        self.values = []
    

    def put(self, key, value):
        for i in range (0, len(self.keys)):
            if (self.keys[i] == key):
                self.values[i] = value
                return False
        self.keys.append(key)
        self.values.append(value)
        return True

    def get(self, key):
        for i in range (0, len(self.keys)):
            if (self.keys[i] == key):
                return self.values[i]
        return Bucket.NOT_FOUND
    
    def remove(self, key):
        for i in range (0, len(self.keys)):
            if (self.keys[i] == key):
                for j in range (0, len(self.keys) - 1):
                    self.keys[j] = self.keys[j + 1]
                    self.values[j] = self.values[j + 1]
                return True
        return False

class Iterator:  
    def __init__(self):
        self.table = None
        self.cur_bucket = 0
        self.cur_key = 0
        self.key = None
        self.value = None   

    def next(self):
        table = self.table
        while (self.cur_bucket < table.num_buckets):
            bucket = table.buckets[self.cur_bucket]
            if (self.cur_key < len(bucket.keys)):
                self.key = bucket.keys[self.cur_key]
                self.value = bucket.values[self.cur_key]
                self.cur_key += 1
                return self
            else:
                self.cur_key = 0
            self.cur_bucket += 1
        return None
        
class HashMap:  
    def iterator(self):
        iter = Iterator()
        iter.table = self
        iter.cur_bucket = 0
        iter.cur_key = 0
        self.iter = iter
        return iter

    def __init__(self):
        self.buckets = []
        self.count = 0
        self.num_buckets = 8
        for i in range (0, self.num_buckets):
            self.buckets.append(Bucket())
        self.iter = None

    def hash(key): # Simplest hash function
        # int h = 0
        # int size = sizeof(Integer)
        # char* p = (char*) &key
        # char prime[8] =:2, 3, 7, 11, 13, 17, 19, 23
        # for (int i = 0 i < size i++):
        #     h += p[i] ^ prime[i] # bitwise XOR
        #     h <<= 8
        # 
        # return h
        return key + 42
    

    def resize(self):
        if (self.count <= int(self.num_buckets * 0.75)):
            return
        
        # 새로운 buckets을 할당한다
        new_count = 0
        new_num_buckets = self.num_buckets * 2
        new_buckets = []
        for i in range (0, new_num_buckets):
            new_buckets.append(Bucket())
        
        # 새로운 buckets에 기존 값들을 모두 옮겨 담는다
        iter = self.iterator()
        while (True):
            iter = iter.next()
            if (iter == None):
                break
            
            h = HashMap.hash(iter.key)
            which_bucket = h % new_num_buckets
            inserted = new_buckets[which_bucket].put(iter.key, iter.value)
            if (inserted == True):
                new_count += 1
            
        # 새로운 buckets를 This에 할당
        self.count = new_count
        self.num_buckets = new_num_buckets
        self.buckets = new_buckets
    
    def put(self, key, value):
        h = HashMap.hash(key)
        which_bucket = h % self.num_buckets
        inserted = self.buckets[which_bucket].put(key, value) 
        if (inserted == True):
            self.count += 1
            self.resize() # (75% 이상 찼을 때) 해쉬테이블의 buckets 용량 증가
            return True
        
        return False
    
    def get(self, key):
        h = HashMap.hash(key)
        which_bucket = h % self.num_buckets
        bucket = self.buckets[which_bucket]
        return bucket.get(key)
    
    def remove(self, key):
        h = HashMap.hash(key)
        which_bucket = h % self.num_buckets        
        bucket = self.buckets[which_bucket]
        if (bucket.remove(key) == True):
            self.count -= 1
            return True
        return False
    
    def print(self):
        iter = self.iterator()
        while (iter.next() != None):
            # iter = Iterator_next(iter)
            # if (iter == NULL) break
            print("(%d : %d)" % (iter.key, iter.value))

    def keys(self):
        keys = []
        iter = self.iterator()
        while(iter.next() != None):
            keys.append(iter.key)
        return keys

    def values(self):
        values = []
        iter = self.iterator()
        while(iter.next() != None):
            values.append(iter.value)
        return values
    
    def contains(self, key):
        if (self.get(key) == Bucket.NOT_FOUND):
            return False
        return True
    
    def get_count(self):
        return self.count

class Set(HashMap):
    def count(self):
        return self.get_count()

    def has(self, element):
        return self.contains(element)

    def add(self, element):
        self.put(element, True)

    def delete(self, element):
        return self.remove(element)
    
    def intersection(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            if (set2.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def union(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            set3.add(iter.key)
        iter = set2.iterator()
        while (iter.next() != None):
            set3.add(iter.key)
        # set4 = set2.keys()
        # for i in range(0, len(set4)):
        #     if (not self.has(set4[i])):
        #         set3.append(set4[i])
        return set3
    
    def difference(self, set2):
        set3 = Set()
        iter = self.iterator()
        while (iter.next() != None):
            if (not set2.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def symmetric_difference(self, set2):
        # set3 = self.difference(set2)
        # set4 = set2.difference(self)
        # for i in range(0, len(set4)):
        #     set3.append(set4[i])
        set3 = Set()
        iter = self.iterator()
        while(iter.next() != None):
            if (not set2.has(iter.key)):
                set3.add(iter.key)
        iter = set2.iterator()
        while(iter.next() != None):
            if (not self.has(iter.key)):
                set3.add(iter.key)
        return set3
    
    def print(self):
        set3 = self.keys()
        print("{", end = "")
        for i in range(0, len(set3)):
            if (i <= len(set3) - 2):
                print("%d, " % (set3[i]), end = "")
            else:
                print("%d" % (set3[i]), end = "")
        print("}")

def qsort(arr, left, right, cmpf):
    if (left >= right): return
    pivot = arr[right]
    pivotIdx = left
    for i in range(left, right):
        if (cmpf(arr[i], pivot) > 0):
            [arr[pivotIdx], arr[i]] = [arr[i], arr[pivotIdx]] # swap
            pivotIdx += 1
        
    [arr[pivotIdx], arr[right]] = [arr[right], arr[pivotIdx]] # swap

    qsort(arr, left, pivotIdx - 1, cmpf)
    qsort(arr, pivotIdx + 1, right, cmpf)

def comp(a, b):
    return b - a        

def main():
    table = HashMap()
    table.put(5, 500)
    table.put(7, 700)
    table.put(1, 100)
    table.put(3, 300)
    table.put(2, 200)
    table.put(15, 1500)
    table.put(17, 1700)
    table.put(11, 1100)
    table.put(13, 1300)
    table.put(12, 1200)
    table.print()
    val = table.get(3)
    print("val : %d" % (val))
    val2 = table.get(9)
    if (val2 == Bucket.NOT_FOUND):
        print("key = %d not found" % (9))
    else:
        print("val : %d" % (val2))
    table.remove(17)
    table.print()

    print("----------")
    keys = table.keys()
    qsort(keys, 0, len(keys) - 1, comp)
    for key in keys:
        print("key: %d, value: %d" % (key, table.get(key)))
    print("----------")
    
    setx = Set()
    setx.add(23)
    setx.add(37)
    setx.add(41)
    setx.add(23)
    setx.print()
    sety = Set()
    sety.add(37)
    sety.add(67)
    sety.add(83)
    sety.add(41)
    sety.print()
    setz = setx.intersection(sety)
    setz.print()
    setw = setx.union(sety)
    setw.print()
    setu = setx.difference(sety)
    setu.print()
    setk = sety.difference(setx)
    setk.print()
    setl = setx.symmetric_difference(sety)
    setl.print()

main()