
class QuickSort:
    def __init__(self, input):
        self.list = input

    def partition(self, start, end):
        if start >= end:
            return
        pivot = self.list[end]
        i = start
        for j in range(start, end):
            if pivot > self.list[j]:
                (self.list[i], self.list[j]) = (self.list[j], self.list[i])
                i += 1
        (self.list[i], self.list[end]) = (self.list[end], self.list[i])
        return i

    def sort(self, start, end):
        if start >= end:
            return
        i = self.partition(start, end)
        self.sort(start, i-1)
        self.sort(i+1, end)


if __name__ == "__main__":
    input = [1, 48, 2, 3, 49, 8, 57, 2, 9, 65, 14]
    sort = QuickSort(input)
    sort.sort(0, len(input)-1)
    print(input)