NOT_NEEDED = 0
NEED_CAMERA = 1
HAS_CAMERA = 2

# min. number of cameras to cover parent, left, and right nodes
class Solution:
    def __init__(self):
        self.cams = 0

    def minCameraCover(self, root: Optional[TreeNode]) -> int:
        if self.cameranum(root) == NEED_CAMERA:
            return (self.cams + 1)
        return self.cams

    def cameranum(self, root):
        if root is None:
            return NOT_NEEDED

        left = self.cameranum(root.left)
        right = self.cameranum(root.right)
        if left == NEED_CAMERA or right == NEED_CAMERA:
            self.cams += 1
            print(self.cams)
            return HAS_CAMERA
        elif left == HAS_CAMERA or right == HAS_CAMERA:
            return NOT_NEEDED
        else:
            return NEED_CAMERA

# reverse linkedlist
#
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def __init__(self):
        self.newhead = None
        self.lastnode = None

    def reverseList(self, head, endnode):
        if head is None:
            return head

        prev_node = head
        curr_node = head
        next_node = head.next
        while next_node is not endnode:
            prev_node = curr_node
            curr_node = next_node
            next_node = next_node.next
            if prev_node == head:
                prev_node.next = endnode
            curr_node.next = prev_node

        head = curr_node
        return head


    def reverseKGroup(self, head: Optional[ListNode], k: int) -> Optional[ListNode]:
        if head is None:
            return head

        node = head
        newhead = None
        while True:
            i = 0
            begin = node
            while i < k:
                end = node.next
                node = node.next
                i += 1
                if node is None:
                    break

            if i < k:
                break
            newhead = self.reverseList(begin, end)
            if self.lastnode is not None:
                self.lastnode.next = newhead
            self.lastnode = begin

            if self.newhead is None:
                print(newhead)
                self.newhead = newhead
            if node is None:
                break
        return self.newhead


# adding two independent list values into another
class Solution:
    def addTwoNumbers(self, l1: Optional[ListNode], l2: Optional[ListNode]) -> Optional[ListNode]:
        node1 = l1
        node2 = l2
        head = None
        prev = None
        over = 0
        while True:
            if node1 is None and node2 is None:
                if over <= 0:
                    break
            if node1 is None:
                num1 = 0
            else:
                num1 = node1.val
                node1 = node1.next
            if node2 is None:
                num2 = 0
            else:
                num2 = node2.val
                node2 = node2.next

            num = num1 + num2 + over
            over = int(num / 10)
            print(over)
            new = ListNode(num % 10)
            if prev is not None:
                prev.next = new
            prev = new
            if head is None:
                head = new

        return head

import math


class Solution:
    def latestTimeCatchTheBus(self, buses: List[int], passengers: List[int], capacity: int) -> int:
