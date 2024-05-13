# delete Node in a linked list
class Solution:
    def __init__(self, val=0):
         self.val = val
         self.next = None

    def deleteNode(self, node):
        """
        :type node: ListNode
        :rtype: void Do not return anything, modify node in-place instead.
        """
        movingnode = node
        if movingnode is None:
            return
        while movingnode.next is not None:
            movingnode.val = movingnode.next.val
            if movingnode.next.next is None:
                movingnode.next = None
                break
            movingnode = movingnode.next
        return
