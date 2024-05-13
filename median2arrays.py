

class Solution:
    def findmedian(self, nums1, nums2, idx1, idx2):
        if idx2 < 0:
            idx2 = 1000
        len1 = len(nums1)
        len2 = len(nums2)

        if (len1 + len2) % 2 == 0:
            average = True
        else:
            average = False

        med = int((len1 + len2 - 1) / 2)
        i = idx1
        j = 0
        while i < len1 and j <= idx2:
            if nums2 is None:
                val2 = -1000000
            val1 = nums1[i]
            print(f'{i}: {j} => {val1}:{val2}')
            if val1 < val2:
                i += 1
            else:
                j += 1
            if (i + j) == med:
                if nums2 is None:
                    None
                if average:
                    median = float((nums1[i] + nums2[j]) / 2)
                elif nums1[i] < nums2[j]:
                    median = float(nums1[i])
                else:
                    median = float(nums2[j])
                break
        return median

    def findMedianSortedArrays(self, nums1, nums2):

        i = int((len(nums1) - 1) / 2)
        j = int((len(nums2) - 1) / 2)

        if len(nums1) is 0 and len(nums2) is 0:
            return None
        if len(nums1) == 0:
            return self.findmedian(nums2, None, j, -1)
        elif len(nums2) == 0:
            return self.findmedian(nums1, None, i, -1)
        elif nums1[i] < nums2[j]:
            return self.findmedian(nums1, nums2, i, j)
        else:
            return self.findmedian(nums2, nums1, j, i)


if __name__ == '__main__':
    input1 = []
    input2 = [2]
    sol = Solution()
    print(sol.findMedianSortedArrays(input1, input2))

