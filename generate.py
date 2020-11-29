import random
import threading

def gen_PUT():
    with open('l1.input', 'w') as f:
        for _ in range(30000000): # 20000000 -> 2.9g
            key = random.randint(0, 9223372036854775807)
            value = str(key)
            while len(value) < 128:
                value = value + '.'
            f.write(f'PUT {key} {value}\n')
    pass

def gen_GET():
    with open('l2.input', 'w') as f:
        for _ in range(160000000): # 20000000 -> 456m
            key = random.randint(0, 9223372036854775807)
            f.write(f'GET {key}\n')
    pass

def gen_ALL():
    with open('l3.input', 'w') as f:
        for _ in range(80000000): # 20000000 -> ???
            act = random.randint(0,2)
            key = random.randint(0, 9223372036854775807)
            if act == 0:
                value = str(key)
                while len(value) < 128:
                    value = value + '.'
                f.write(f'PUT {key} {value}\n')
            elif act == 1:
                f.write(f'GET {key}\n')
            elif act == 2:
                key2 = random.randint(0, 9223372036854775807)
                f.write(f'SCAN {min(key, key2)} {max(key, key2)}\n')
    pass

t1 = threading.Thread(target = gen_PUT)
t2 = threading.Thread(target = gen_GET)
t3 = threading.Thread(target = gen_ALL)

t1.start()
t2.start()
t3.start()
t1.join()
t2.join()
t3.join()