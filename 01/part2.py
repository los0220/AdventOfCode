#!/usr/bin/python3

import sys

def main(input_path):
    global_sum = 0
    position = 50
    rollout = 100

    with open(input_path, "r") as f:
        input_contents = f.read().strip('\n')

    steps = input_contents.replace('L', '-').replace('R', '+').split('\n')

    for step in map(int, steps):

        while step != 0:
            if step > 0:
                position = (position + 1) % rollout
                step -= 1

            if step < 0:
                position = (position - 1) % rollout
                step += 1

            if position == 0:
                global_sum += 1
            
    print(global_sum)

if __name__ == "__main__":
    main(sys.argv[1])
