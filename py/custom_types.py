MAX_WEIGHT = 2**32

class weights_t:
	def __init__(self, time_w = 0, fare_w = 0, locals_w = 0):
		self.__weights__ = {'time':time_w, 'fare':fare_w, 'locals':locals_w}
	def __getitem__(self, index):
		return self.__weights__[index]
	def __add__(self, other):
		res = weights_t()
		for p in self.__weights__:
			res.__weights__[p] = self[p] + other[p]
		return res
	def __str__(self):
		return f"time:{self['time']}|fare:{self['fare']}|locals{self['locals']}" 

class Node:
    def __init__(self, data=None):
        self.data = data
        self.next = None
        self.prev = None

class DoublyLinkedList:
    def __init__(self):
        self.size = 0
        self.head = None
        self.tail = None

    def append(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
            self.tail = new_node
        else:
            new_node.prev = self.tail
            self.tail.next = new_node
            self.tail = new_node
        self.size = self.size + 1

    def prepend(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
            self.tail = new_node
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
        self.size = self.size + 1

    def delete(self, data):
        current_node = self.head
        while current_node:
            if current_node.data == data:
                if current_node == self.head:
                    self.head = current_node.next
                    self.head.prev = None
                elif current_node == self.tail:
                    self.tail = current_node.prev
                    self.tail.next = None
                else:
                    current_node.prev.next = current_node.next
                    current_node.next.prev = current_node.prev
                return
            current_node = current_node.next
        self.size = self.size - 1

    def traverse_forward(self):
        current_node = self.head
        while current_node:
            yield current_node.data
            current_node = current_node.next
    def traverse_backward(self):
        current_node = self.tail
        while current_node:
            yield current_node.data
            current_node = current_node.prev
    def insert_after(self, prev, data):
        if prev < 0:
            prev = self.size + prev  
        if self.size < prev:
            raise ValueError("incorrect prev") 
        new_node = Node(data)
        current_node = self.head
        poss = 0
        while poss < prev:
            poss = poss + 1
            current_node = current_node.next
        new_node.prev = current_node
        new_node.next = current_node.next 
        new_node.next.prev = new_node  
        current_node.next = new_node
