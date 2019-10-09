import sys

symbols = ['s', 'k', 'p']

def trees(depth):
	assert depth >= 1
	if depth == 1:
		return list(symbols)
	ts = []
	for i in range(1, depth):
		left = trees(i)
		right = trees(depth - i)
		for ell in left:
			for r in right:
				ts.append((ell, r))
	return ts

def printtree(tree):
	def treestring(tree):
		if tree in symbols:
			return tree
		return "%s(%s)" % (treestring(tree[0]), treestring(tree[1]))
	print treestring(tree) + ";"

def main(args):
	upper = int(args[1])
	for i in range(2, upper + 1):
		map(printtree, trees(i))
	return 0

if __name__ == "__main__":
	sys.exit(main(sys.argv))
