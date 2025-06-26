#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <time.h>
#include <sys/param.h>

int main(int argc, char **argv) {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	int phase = tm->tm_yday - 15;

	double flour = 1000.0;
	double water = 720.0;
	double yeast = 0.275 + 0.075 * cos(2 * M_PI * phase / 365);
	double salt  = 28.0;
	double margin = 30.0;
	double extra = 80.0;
	double weight = 260.0;

	int whole_grams = 0;

	long pizzas = 6;

	int percentage = 0;
	int flour_flag = 0;

	static struct option opts[] = {
		{ "flour",       required_argument, NULL, 'f' },
		{ "water",       required_argument, NULL, 'w' },
		{ "yeast",       required_argument, NULL, 'y' },
		{ "salt",        required_argument, NULL, 's' },
		{ "pizzas",      required_argument, NULL, 'p' },
		{ "margin",      required_argument, NULL, 'm' },
		{ "extra",       required_argument, NULL, 'e' },
		{ "weight",      required_argument, NULL, 'g' },
		{ "whole-grams", no_argument,       NULL, 'o' },
	};

	char *endptr;
	int c, option_index = 0;
	int error = 0;
	while ((c = getopt_long(argc, argv, "f:w:y:s:p:m:e:g:n", opts, &option_index)) != -1) {
		switch (c) {
		case 'f':
			flour_flag = 1;
			errno = 0;
			flour = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || flour <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --flour %s\n", optarg);
			}
			break;
		case 'w':
			errno = 0;
			water = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || water <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --water %s\n", optarg);
			}
			break;
		case 'y':
			errno = 0;
			yeast = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || yeast <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --yeast %s\n", optarg);
			}
			break;
		case 's':
			errno = 0;
			salt = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || salt <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --salt %s\n", optarg);
			}
			break;
		case 'p':
			errno = 0;
			pizzas = strtol(optarg, &endptr, 10);
			if (errno || *endptr != '\0' || pizzas <= 0) {
				error = 1;
				fprintf(stderr, "error: --pizzas %s\n", optarg);
			}
			break;
		case 'm':
			errno = 0;
			margin = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || margin <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --margin %s\n", optarg);
			}
			break;
		case 'e':
			errno = 0;
			extra = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || extra < 0.0) {
				error = 1;
				fprintf(stderr, "error: --extra %s\n", optarg);
			}
			break;
		case 'c':
			percentage = 1;
			break;
		case 'g':
			errno = 0;
			weight = strtod(optarg, &endptr);
			if (errno || *endptr != '\0' || weight <= 0.0) {
				error = 1;
				fprintf(stderr, "error: --weight %s\n", optarg);
			}
			break;
		case 'o':
			whole_grams = 1;
			break;
		default:
			error = 1;
			break;
		}
	}
	if (error || (flour_flag && percentage))
		return 1;

	double totalweight = flour + water + yeast + salt;
	double scaledmargin = pizzas * margin;
	double factor = (pizzas * weight + scaledmargin + extra) / totalweight;
	double realflour = flour * factor;
	double realwater = water * factor;
	double realyeast = yeast * factor;
	double realsalt  = salt * factor;

	if (realwater < 100 && whole_grams) {
		printf("The batch is too small for --whole-grams.\n");
		exit(1);
	}

	double yeastwater = 20;

	printf("Pizza\n");
	printf("=====\n");
	printf("Flour            : %lg (g)\n", whole_grams ? round(realflour) : realflour);
	printf("Salt             : %lg (g)\n", whole_grams ? round(realsalt) : realsalt);
	if (whole_grams) {
		printf("Water            : %lg (g)\n", round(realwater - yeastwater));
		printf("Yeast Water      : %lg (g)\n\n", round(yeastwater));
		printf("Yeast Water\n");
		printf("===========\n");
		printf("Yeast            : 20 (g)\n");
		printf("Water            : %lg (g)\n\n", round(20.0 * 20.0 / realyeast - 20.0));
	}
	else {
		printf("Water            : %lg (g)\n", realwater);
		printf("Yeast            : %lg (g)\n\n", realyeast);
	}
	printf("Info\n");
	printf("====\n");
	printf("Number of pizzas : %ld\n", pizzas);
	printf("Water percentage : %lg%%\n", 100.0 * water / flour);
	printf("Margin           : %lg (g)\n", whole_grams ? round(scaledmargin) : scaledmargin);
	printf("Extra            : %lg (g)\n", whole_grams ? round(extra) : extra);

	return 0;
}
