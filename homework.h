#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pixel;

typedef struct {
	char version[3];
	int weight, height, maxValue;
	unsigned char *matrix_bw;
	pixel *matrix_color;
}image;

typedef struct {
	int start;
	int pas;
	image *in;
	image *out;
} treduri;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
