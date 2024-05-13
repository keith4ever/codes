# min. number of cameras to cover parent, left, and right nodes

import math

class Solution:
    def __init__(self, height) -> None:
        self.height = height

    def water(self, i, j) -> int:
        if len(self.height) <= i or len(self.height) <= j:
            return 0
        
        height = min(self.height[i], self.height[j])
        return height * abs(i - j)
    
    def most_water(self) -> int:
        max_water = 0

        for i in range(len(self.height)):
            for j in range(i+1, len(self.height)):
                curr_water = self.water(i, j)
                max_water = max(max_water, curr_water)
        return max_water

if __name__ == "__main__":
    height = [1,1]
    solution = Solution(height)
    print(f'most water: {solution.most_water()}')
