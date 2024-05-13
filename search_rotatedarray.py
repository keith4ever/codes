
class Solution(object):
    def __init__(self):
        pass

    def search(self, nums, target):
        rotate_idx = 0
        prev = nums[0]
        for i in range(1, len(nums)):
            if nums[i] < prev:
                rotate_idx = i
                break
            if i == len(nums) - 1:
                rotate_idx = 0
            prev = nums[i]
        # max is nums[rotate_idx - 1], min is nums[rotate_idx]
        if target < nums[rotate_idx]:
            return -1
        elif target > nums[rotate_idx - 1]:
            return -1
        elif target == nums[rotate_idx]:
            return rotate_idx
        elif target == nums[rotate_idx-1]:
            return rotate_idx - 1
        
        if target >= nums[0]:
            for i in range(0, rotate_idx - 1):
                if target == nums[i]:
                    return i
                elif target < nums[i]:
                    return -1
            return -1
        else:
            for i in range(rotate_idx, len(nums)):
                if target == nums[i]:
                    return i
                elif target < nums[i]:
                    return -1
            return -1


if __name__ == "__main__":
    nums = [1,3]
    solution = Solution()
    index = solution.search(nums, 3)
    print(f'index: {index}')
