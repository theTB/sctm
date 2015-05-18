# Specific Correspondence Topic Model
=====================================

Code for the paper: 
#### "[Going Beyond Corr-LDA for Detecting Specific Comments on News & Blogs](http://dl.acm.org/citation.cfm?id=2556231)". In ACM international conference on Web Search and Data Mining (WSDM), 2014.

This implementation contains the following 3 models:  

0. *Latent Dirichlet Allocation* (LDA)  
   Basic LDA model with collapsed Gibbs sampling (no correspondence). A bonus feature included is _sparse topics_ which allows learning sparse topic distributions which are more diverse on their set of top words (see paper for details).

0. *Correspondence LDA* (CorrLDA)  
   CorrLDA model for articles and comments (or any two paired sets of documents). The latent topic space is shared between the articles and comments. As an improvement over vanilla model, this also includes an _irrelevant topic_ for comments (see paper) and the feature of _sparse topics_. Inference is collapsed Gibbs sampling.

0. *Specific Correspondence Topic Model* (SCTM)  
   This is the model proposed in the paper for modeling specific correspondence between articles and comments. Includes the features of _irrelevant topic_ and _sparse topics_. Implements _multiple topic vectors_ and _specific correspondence_ (see paper for details).


#### Using the Code
Download all the code files and go into the folder called "_Release_". This contains the Makefile. Compile the code,  `make clean; make`. The main exectuable is called "_sctm_".  

Usage : `./sctm <1.article-file> <2.comments-file> <3.output-dir> <4.topics> <5.model> <6.train/test>`  
where
- _article-file_ is the location of the file containing the article contents
- _comments-file_ is the location of the file containing the comment contents
- _output-dir_ is the location and name of the directory to write output
- _topics_ is the number of topics (K)
- _model_ is the model to train, one of: lda, corrlda, sctm
- _train/test_ (optional), 1 for test data (in this case output-dir should point to location of trained model)

There is a sample pre-processed dataset of 501 documents and some comments provided in the folder "_input_". To run a demo on this dataset with 100 topics, use the command:  
`./sctm ../input/abagf.AT.txt ../input/cbagf.AT.txt ../output 100 sctm`


#### Input Data Format
All words should be converted to integer vocabulary ids' starting from 0.  
- Article Format  
   The first line contains the number of documents D. Each document begins with the number of sentences in first line S. Each of the S lines begin with the number of words in the sentence (N), followed by each word id.
   Example:  
   D  
   S<sub>1</sub>  
   N<sub>1</sub> w<sub>1</sub> w<sub>2</sub> .... w<sub>N<sub>1</sub></sub>  

- Comments Format
   Similar to above. First line is D. For each document, first line is number of comments C, followed by C lines with each line beginning with number of comment words N, followed by each comment word. Note that if there is a document with no comments, it should be present in the file with 0 (for C) and no following lines.


#### Output
Following are the four outputs from the model:
- Topic distributions: The learned topics by the model. The last topic is always the _irrelevant topic_. So if the model is run with K topics, then K+1 topics are output. The last topic should be ignored for LDA model. File "_beta_".  
- Comment topic distribution: Topic distribution (over K+1 topics) of each comment. File "*y_dist.txt*".  
- Article topic distribution: Overall topic distribution (over K topics) of each article. File "*z_dist.txt*"  
- Sentence selection probability: The probability of correspondence of a comment to each article sentence (see paper). File "*xi_prob.txt*".  

The dot products of the article and comment topic distributions are used for the applications described in the paper.


#### Queries/Help
Direct any queries to "trapitbansal at gmail dot com".
