import threading


def ispd(input):
    # releaseHydrogen() outputs "H". Do not change or remove this line.
    j = len(input) - 1
    for i, letter in enumerate(input):
        if letter != input[j]:
            return False
        if i >= j:
            break
        j -= 1
    return True


if __name__ == "__main__":
    input = "asdfghjklkjhgfdsa"
    print(ispd(input))
