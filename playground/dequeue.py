class Queue:
    def __init__(self, cap: int):
        self.items = [None] * cap
        self.cap = cap
        self.size = 0
        self.front = 0

    def insert_tail(self, x):
        if self.size == self.cap:
            return
        else:
            tail = (self.front + self.size) % self.cap
            self.items[tail] = x
            self.size += 1

    def insert_front(self, x):
        if self.size == self.cap:
            return
        else:
            self.front = (self.front - 1) % self.cap
            self.items[self.front] = x
            self.size += 1

    def deleteFront(self):

        # Check if deque is empty
        if self.size == 0:
            return None
        else:
            res = self.items[self.front]
            self.front = (self.front + 1) % self.cap
            self.size -= 1
            return res

    def deleteRear(self):

        # Check if deque is empty
        if self.size == 0:
            return None
        else:
            rear = (self.front + self.size - 1) % self.cap
            res = self.items[rear]
            self.size -= 1
            return res
