import threading


class DiningPhilosophers:
    def __init__(self):
        self.forks = [threading.Lock() for i in range(5)]

    # call the functions directly to execute, for example, eat()
    def wantsToEat(self,
                   philosopher: int,
                   pickLeftFork: 'Callable[[], None]',
                   pickRightFork: 'Callable[[], None]',
                   eat: 'Callable[[], None]',
                   putLeftFork: 'Callable[[], None]',
                   putRightFork: 'Callable[[], None]') -> None:
        leftfork = philosopher
        rightfork = (philosopher - 1) % 5

        while True:
            llocked = self.forks[leftfork].acquire(timeout=0.0001)
            rlocked = self.forks[rightfork].acquire(timeout=0.0001)
            if llocked and rlocked:
                pickLeftFork()
                pickRightFork()
                eat()
                putLeftFork()
                putRightFork()
                self.forks[leftfork].release()
                self.forks[rightfork].release()
                return
            elif llocked:
                self.forks[leftfork].release()
            elif rlocked:
                self.forks[rightfork].release()
