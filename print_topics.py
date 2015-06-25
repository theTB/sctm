import sys


def print_topics(beta, vocab, n):
	K = len(beta)
	for k in range(K):
		topn = sorted(range(len(beta[k])), key = lambda x: beta[k][x], reverse=True)[:n]
		if k < K-1:
			print "Topic %3d: "%(k+1),
		else: print "Irrelevant Topic %d: "%(k+1),
		print ", ".join(map(lambda x: vocab[x], topn))
	print "\nNOTE: Ignore 'irrelevant topic' for LDA"
			


if __name__ == '__main__':
	if len(sys.argv) < 3:
		print 'Usage: python print_topics.py <beta> <vocab> <?num_words>'
		sys.exit()
		
	with open(sys.argv[1].strip(),'r') as bf:
		K, V = map(int, bf.readline().split())
		beta = []
		for t in bf:
			beta.append(map(lambda x: float(x.strip()), t.strip().split(' ')))
			
	with open(sys.argv[2].strip(),'r') as vf:
		vocab = map(lambda x: x.strip(), vf.readlines())

	V = len(vocab)
	n = 5
	if len(sys.argv) > 3: n = int(sys.argv[3])
	print_topics(beta, vocab, n)
