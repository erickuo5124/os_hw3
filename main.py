import sys
import numpy as np

def PUT(key, value):
    key = int(key)
    arr_num, index = key >> 16, key & 0xffff
    
    try:
        arr = np.load(f'./stroage/{arr_num}.npy', allow_pickle=True)
    except FileNotFoundError:
        arr = np.empty([0xffff], dtype=object)
        arr[:] = 'EMPTY'

    arr[index] = value
    np.save(f'./stroage/{arr_num}', arr)
    pass

def GET(key, file):
    key = int(key)
    arr_num, index = key >> 16, key & 0xffff

    with open(f'{file}.output', 'a') as fout:
        try:
            arr = np.load(f'./stroage/{arr_num}.npy', allow_pickle=True)
            fout.write(arr[index])
        except FileNotFoundError:
            fout.write('EMPTY')
        fout.write('\n')
    pass

def SCAN(key1, key2, file): 
    key1 = int(key1)
    key2 = int(key2)
    for key in range(key1, key2+1):
        GET(key, file)
    pass

with open(sys.argv[1], 'r') as fin:
    for line in fin.readlines():
        command = line.split()
        if command[0] == 'PUT':
            PUT(command[1], command[2])
        elif command[0] == 'GET':
            GET(command[1], sys.argv[1][:-6])
        elif command[0] == 'SCAN':
            SCAN(command[1], command[2], sys.argv[1][:-6])