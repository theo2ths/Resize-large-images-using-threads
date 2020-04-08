#ifndef HOMEWORK_H1
#define HOMEWORK_H1

typedef struct {
	unsigned char *matrix;
}image;

typedef struct {
	int start, pas;
	image *im;
} treduri;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */
