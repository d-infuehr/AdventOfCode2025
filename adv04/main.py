data = []
width = 0

def valueOf(x, y):
    if (x < 0 or x >= width) or (y < 0 or y >= height):
        return 0
    return 1 if data[y * width + x] == "@" else 0

def read_environment(x, y):
    sum = 0
    sum += valueOf(x - 1, y - 1)
    sum += valueOf(x + 0, y - 1)
    sum += valueOf(x + 1, y - 1)
    sum += valueOf(x - 1, y + 0)

    sum += valueOf(x + 1, y + 0)
    sum += valueOf(x - 1, y + 1)
    sum += valueOf(x + 0, y + 1)
    sum += valueOf(x + 1, y + 1)
    return 1 if sum < 4 else 0
    
with open("input.txt") as f:
  width = len(f.readline().rstrip())
  f.seek(0)
  for x in f:
      for y in x.rstrip():
        data.append(y)
height = len(data) // width

sum = 0
for idx, x in enumerate(data):
    if x == '@':
        result = read_environment(idx % width, idx // width)
        sum += result

print(f"Part 1 sum: {sum}")

sum = 0
old_sum = -1
while(sum != old_sum):
    old_sum = sum
    removal = []
    for idx, x in enumerate(data):
        if x == '@':
            result = read_environment(idx % width, idx // width)
            if (result != 0):
                removal.append(idx)
            sum += result
    print(f"Removal this round: {len(removal)}")
    for idx in removal:
        data[idx] = '.'
print(f"Part 2 sum: {sum}")