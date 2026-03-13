# OS Synchronization Programs — Main Logic

## 1. Readers-Writers Problem

```
SEMAPHORE wrt = 1        → controls writer access
MUTEX                    → protects readcount

READER:
  lock mutex
    readcount++
    if first reader → block writers (sem_wait wrt)
  unlock mutex
  → READ
  lock mutex
    readcount--
    if last reader → unblock writers (sem_post wrt)
  unlock mutex

WRITER:
  wait for wrt (blocks if any reader/writer active)
  → WRITE
  post wrt (release for next)

KEY RULES:
  - Multiple readers allowed simultaneously
  - Writer needs exclusive access (no readers or writers)
  - First reader locks out writers
  - Last reader unlocks for writers
```

---

## 2. Dining Philosophers Problem

```
5 philosophers, 5 chopsticks (semaphores = 1)
Each philosopher needs LEFT and RIGHT chopstick to eat

PHILOSOPHER:
  THINK  → sleep
  HUNGRY → pick up both chopsticks
  EAT    → sleep
  DONE   → put down both chopsticks

DEADLOCK PREVENTION:
  Philosophers 0-3 → pick LEFT first, then RIGHT
  Philosopher 4    → pick RIGHT first, then LEFT
  (breaks circular wait → prevents deadlock)

CHOPSTICK ORDER:
  left  = id
  right = (id+1) % N

KEY RULES:
  - No two adjacent philosophers eat simultaneously
  - Asymmetric pickup order breaks deadlock
  - sem_wait = pick up chopstick
  - sem_post = put down chopstick
```

---

## macOS Note

> `sem_init` and `sem_destroy` are deprecated on macOS.
> Use named semaphores: `sem_open` / `sem_close` / `sem_unlink`

```c
// Instead of sem_init(&sem, 0, 1)
sem_t *sem = sem_open("/name", O_CREAT, 0644, 1);

// Instead of sem_destroy(&sem)
sem_close(sem);
sem_unlink("/name");
```

---

## Compile & Run

```bash
gcc readers_writers.c -o rw -lpthread
./rw

gcc dining.c -o dining -lpthread
./dining
```
