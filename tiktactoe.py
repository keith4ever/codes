class Tiktactoe:
    def __init__(self):
        self.mat = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

    def print(self):
        for i in range(0, 3):
            print(self.mat[i])

    def check(self, i, j, val):
        if i < 0 or i > 2 or j < 0 or j > 2:
            print(f"out of bound location: 0 <= {i},{j} <= 2")
            return False
        elif self.mat[i][j] != 0:
            print(f"it is already occupied: {self.mat[i][j]}")
            return False
        self.mat[i][j] = val
        return True

    def win(self):
        for i in range(0, 3):
            hor = self.mat[i][0] * self.mat[i][1] * self.mat[i][2]
            ver = self.mat[0][i] * self.mat[1][i] * self.mat[2][i]
            if hor == 1 or ver == 1:
                return 1
            elif hor == 8 or ver == 8:
                return 2
        x1 = self.mat[0][0] * self.mat[1][1] * self.mat[2][2]
        x2 = self.mat[0][2] * self.mat[1][1] * self.mat[2][0]
        if x1 == 1 or x2 == 1:
            return 1
        elif x1 == 8 or x2 == 8:
            return 2

        return 0


if __name__ == "__main__":
    tik = Tiktactoe()
    b1turn = True

    while True:
        tik.print()
        if b1turn:
            val = 1
        else:
            val = 2
        stri, strj = input(f"[{val}] mark your spot: ").split()
        i, j = [int(stri), int(strj)]
        if tik.check(i, j, val) is False:
            continue
        if tik.win() == val:
            print("*********")
            print(f"player [{val}] has won !!")
            tik.print()
            exit(0)
        b1turn = not b1turn
