import threading


class H2O:
    def __init__(self, str):
        self.input = list(str)
        self.barrier = threading.Barrier(3)
        self.lock = threading.Lock()

    def hydrogen(self, releaseHydrogen) -> None:
        # releaseHydrogen() outputs "H". Do not change or remove this line.
        while True:
            self.barrier.wait()
            with self.lock:
                if 'H' in self.input:
                    self.input.remove('H')
                else:
                    break
            releaseHydrogen()

    def oxygen(self, releaseOxygen) -> None:
        # releaseOxygen() outputs "O". Do not change or remove this line.
        while True:
            self.barrier.wait()
            with self.lock:
                if 'O' in self.input:
                    self.input.remove('O')
                else:
                    break
            releaseOxygen()


elemants = []


def releaseHydrogen():
    elemants.append('H')


def releaseOyxgen():
    elemants.append('O')


if __name__ == "__main__":
    thrarray = []
    input = "HHOHOOHHOHOHHHH"
    h2o = H2O(input)
    thrarray.append(threading.Thread(target=h2o.hydrogen, args=(releaseHydrogen,)))
    thrarray.append(threading.Thread(target=h2o.hydrogen, args=(releaseHydrogen,)))
    thrarray.append(threading.Thread(target=h2o.oxygen, args=(releaseOyxgen,)))
    for thr in thrarray:
        thr.start()

    for thr in thrarray:
        thr.join()

    print(elemants)
