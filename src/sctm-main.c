#include "sctm-main.h"
gsl_rng* RANDOM_NUMBER = NULL;

void gibbs(char* artf, char* cmntf, char* odir, int trte, int topics, char* modelName) {

	if (trte == 0)
		make_directory(odir);
	sctm_params* params = get_params(trte, topics, modelName);

	sctm_data* data = read_data(artf, cmntf, params);

	sctm_counts* counts = NULL;
	counts = (sctm_counts*) malloc(sizeof(sctm_counts));

	sctm_latent* latent = (sctm_latent*) malloc(sizeof(sctm_latent));
	
	if (trte==1) {
		load_beta(odir, data, params, latent);
	}
	
	printf("\nDocuments:%d Comments:%d Vocabulary:%d\n",data->D, data->C, data->V);
	fflush(stdout);
	
	allocate_vars(data, params, latent, counts);

	ini_vars(data, params, latent, counts);

	assignment(odir, data, params, latent, counts, -1);

	infer(odir, data, params, latent, counts);
	printf("\n\nInference done.\n");
	fflush(stdout);

	assignment(odir, data, params, latent, counts, params->ITER+1);

	free_vars(data, params, latent, counts);
	free_params(params);
	free_data(data);
	fflush(stdout);
}

int main (int argc, char* argv[]) {
	long t1;
	(void) time(&t1);

	RANDOM_NUMBER = random_generator(t1);

	if (argc == 6 || argc == 7) {
		int trte;
		if (argc == 7) trte = atoi(argv[6]);
		else trte = 0;
		if (trte == 0)
				make_directory(argv[3]);
		FILE *s;
		char fname[100];
		sprintf(fname, "%s/seed.txt", argv[3]);
		s = fopen(fname, "w");
		fprintf(s,"%lu",t1);
		fclose(s);

		gibbs(argv[1], argv[2], argv[3], trte, atoi(argv[4]), argv[5]);

	}
	else {
		printf("Usage : ./sctm <1.articles> <2.comments> <3.output_dir> <4.topics> <5.model> <?6.trte (0:train, 1:test)>\n"
				"\nExample to run demo with 100 topics:"
				"\n./sctm ../input/abagf.AT.txt ../input/cbagf.AT.txt ../output 100 sctm"
				"\n\nSee ReadMe for details.\n\n");
	}

	free_random_generator(RANDOM_NUMBER);
	return 0;
}
