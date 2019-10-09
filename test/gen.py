import sys
from itertools import product

symbols = 's', 'k', 'p'

def trees(depth):
    if depth == 1:
        return list(symbols)
    return [(L, R)
            for i in range(1, depth)
            for L, R in product(trees(i), trees(depth - i))]

def printtree(tree):
    def treestring(tree):
        if tree in symbols:
            return tree
        return "%s(%s)" % (treestring(tree[0]), treestring(tree[1]))
    print(treestring(tree) + ";")

def main(args):
    for i in range(2, int(args[1]) + 1):
        list(map(printtree, trees(i)))
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
