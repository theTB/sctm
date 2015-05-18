# Specific Correspondence Topic Model
=====================================

Code for the paper: 
#### "[Going Beyond Corr-LDA for Detecting Specific Comments on News & Blogs](http://dl.acm.org/citation.cfm?id=2556231)". In ACM international conference on Web Search and Data Mining (WSDM), 2014.

This implementation contains the following 3 models:  

0. *Latent Dirichlet Allocation* (LDA)  
   Basic LDA model with Gibbs sampling (no correspondence). A bonus feature included is _sparse topics_ which allows learning sparse topic distributions which are more diverse on their set of top words (see paper for details).

0. *Correspondence LDA* (CorrLDA)  
   CorrLDA model for articles and comments (or any two paired sets of documents). The latent topic space is shared between the articles and comments. As an improvement over vanilla model, this also includes an _irrelevant topic_ for comments (see paper) and the feature of _sparse topics_.

0. *Specific Correspondence Topic Model* (SCTM)  
   This is the model proposed in the paper for modeling specific correspondence between articles and comments. Includes the features of _irrelevant topic_ and _sparse topics_. Implements _multiple topic vectors_ and _specific correspondence_ (see paper for details).


#### Using the Code
Download all the code files and go into the folder called "Release". This contains the Makefile. Compile the code,  `make clean; make`. The main exectuable is called "sctm".  

Usage : `./sctm <1.article-file> <2.comments-file> <3.output-dir> <4.topics> <5.model> <6.train/test>`  
where
- _article-file_ is the location of the file containing the article contents
- _comments-file_ is the location of the file containing the comment contents
- _output-dir_ is the location and name of the directory to write output
- _topics_ is the number of topics (K)
- _model_ is the model to train, one of: lda, corrlda, sctm
- _train/test_ (optional), 1 for test data (in this case output-dir should point to location of trained model)

There is a sample pre-processed dataset of 501 documents and some comments provided in the folder _input_. To run a demo on this dataset with 100 topics, use the command:  
`./sctm ../input/abagf.AT.txt ../input/cbagf.AT.txt ../output 100 sctm`


#### Input Data Format


#### Output
