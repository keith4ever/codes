

class SwapTwo:

    def swapnodes(self, node):
        if node.next is None:
            return node
        first = node.next
        second = node
        third = node.next.next
        node = first
        node.next = second
        second.next = third
        return first

    def swapPairs(self, head):
        if head is None:
            return
        if head.next is None:
            return head

        node = head
        prevnode = None
        newhead = None
        while node is not None:
            node = self.swapnodes(node)
            if prevnode is not None:
                prevnode.next = node
            print(node)
            if newhead is None:
                newhead = node
            if node.next is None:
                break
            prevnode = node.next
            node = node.next.next
        return newhead


class ListNode:
    def __init__(self, val=0):
         self.val = val
         self.next = None


def compose(list):
    head = None
    prevnode = None
    for val in list:
        node = ListNode(val)
        if prevnode is not None:
            prevnode.next = node
        if head is None:
            head = node
        prevnode = node
    print(head)
    return head


if __name__ == '__main__':
    input = [1, 2, 3, 4]
    sol = SwapTwo()
    head = compose(input)
    print(sol.swapPairs(head))
