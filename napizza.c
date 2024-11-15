#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <sys/param.h>

int main(int argc, char **argv) {
	double flour = 1000.0;
	double water = 660.0;
	double yeast = 3.5;
	double salt  = 28.0;
	double margin = 10.0;
	double extra = 80.0;
	double weight = 260.0;

	long pizzas = 6;

	int percentage = 0;
	int flour_flag = 0;

	static struct option opts[] = {
		{ "flour",      required_argument, NULL, 'f' },
		{ "water",      required_argument, NULL, 'w' },
		{ "yeast",      required_argument, NULL, 'y' },
		{ "salt",       required_argument, NULL, 's' },
		{ "pizzas",     required_argument, NULL, 'p' },
		{ "margin",     required_argument, NULL, 'm' },
		{ "extra",      required_argument, NULL, 'e' },
		{ "weight",     required_argument, NULL, 'g' },
	};

	char *endptr;
	int c, option_index = 0;
	int error = 0;
	while ((c = getopt_long(argc, argv, "f:w:y:s:p:m:e:g:", opts, &option_index)) != -1) {
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
		default:
			error = 1;
			break;
		}
	}
	if (error || (flour_flag && percentage))
		return 1;

	double totalweight = flour + water + yeast + salt;
	double scaledmargin = MIN(4, pizzas) * margin;
	double factor = (pizzas * weight + scaledmargin + extra) / totalweight;
	double realflour = flour * factor;
	double realwater = water * factor;
	double realyeast = yeast * factor;
	double realsalt  = salt * factor;

	printf("Number of pizzas: %ld\n", pizzas);
	printf("Water percentage: %lg%%\n\n", 100.0 * water / flour);
	printf("Flour  : %lg (g)\n", realflour);
	printf("Water  : %lg (g)\n", realwater);
	printf("Yeast  : %lg (g)\n", realyeast);
	printf("Salt   : %lg (g)\n\n", realsalt);
	printf("Margin : %lg (g)\n", scaledmargin);
	printf("Extra  : %lg (g)\n", extra);

	return 0;
}
