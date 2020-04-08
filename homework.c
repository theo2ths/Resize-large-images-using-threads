#include "homework.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num_threads;
int resize_factor;
int scale;
int gaussian[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

void* threadFunction(void *var) {
    treduri tred = * ( treduri * )var;
    int a, b, c;
    int line, col, i, k, l;
    image *in  = tred.in;
    image *out = tred.out;
    int pas = tred.pas;
    int start = tred.start;
    int fact1;
    int fact2;
    int N = out->height * out->weight;
    for(i = start * pas; i < N && i < (start + 1) * pas; i ++) {
	    a = b = c = 0;
	    line = i/out->weight;
	    col = i - out->weight * line;
	    fact1 = line * resize_factor;
	    fact2 = col * resize_factor;
	    if(!strcmp(tred.in->version, "P6")) {
		if(resize_factor == 3) {
			for(k = fact1; k - resize_factor < fact1; k++) {
				for(l = fact2; l - resize_factor < fact2; l++) {
					a += in->matrix_color[k * in->weight + l].r * gaussian[k - fact1][l - fact2];
					b += in->matrix_color[k * in->weight + l].g * gaussian[k - fact1][l - fact2];
					c += in->matrix_color[k * in->weight + l].b * gaussian[k - fact1][l - fact2];
				}
			}
	    	} else {
			for(k = fact1; k - resize_factor < fact1; k++) {
                                for(l = fact2; l - resize_factor < fact2; l++) {
                                        a += in->matrix_color[k * in->weight + l].r;
                                        b += in->matrix_color[k * in->weight + l].g;
                                        c += in->matrix_color[k * in->weight + l].b;
                                }
                        }
		}
		out->matrix_color[i].r = a/scale;
		out->matrix_color[i].g = b/scale;
		out->matrix_color[i].b = c/scale;		
    	} else {
                if(resize_factor == 3) {
                        for(k = fact1; k - resize_factor < fact1; k++) {
                                for(l = fact2; l - resize_factor < fact2; l++) {
                                        a += in->matrix_bw[k * in->weight + l] * gaussian[k - fact1][l - fact2];
                                }
                        }
                } else {
                        for(k = fact1; k - resize_factor < fact1; k++) {
                                for(l = fact2; l - resize_factor < fact2; l++) {
                                        a += in->matrix_bw[k * in->weight + l];
                                }
                        }
                }
                out->matrix_bw[i] = a/scale;
	}
   }
	return NULL;
}

void readInput(const char * fileName, image *img) {
    if(resize_factor == 3) {
	    scale = 16;
    } else {
	    scale = resize_factor * resize_factor;
    }
    FILE *in;
    int i;
    in  = fopen(fileName, "rb");
    fscanf(in, "%s\n%d %d\n%d\n",img->version, &(img->weight), &(img->height), &(img->maxValue));
    if(!strcmp(img->version, "P5")) {
	img->matrix_bw = malloc(img->weight * img->height * sizeof(unsigned char));
       	for(i = 0; i < img->weight * img->height; i++) {
		fread(&(img->matrix_bw[i]), sizeof(unsigned char), 1, in);
	}
	fclose(in);
	return;
    }
    img->matrix_color = malloc(img->weight * img->height * sizeof(pixel));
    for(int i = 0; i < img->weight * img->height; i++) {
	     fread(&(img->matrix_color[i]), sizeof(pixel), 1, in);
    }
    fclose(in);
}

void writeData(const char * fileName, image *img) {
    FILE *out;
    out  = fopen(fileName, "wb");
    int i;
    fprintf(out, "%s\n%d %d\n%d\n", img->version, img->weight,img->height, img->maxValue);
    if(!strcmp(img->version, "P5")) {
        for(i = 0; i < img->height * img->weight; i++) {
       		fwrite(&(img->matrix_bw[i]), sizeof(unsigned char), 1, out);
        }
	fclose(out);
	return;
    } 
    for(i = 0; i < img->weight * img->height; i++) {
	fwrite(&(img->matrix_color[i]), sizeof(pixel), 1, out);
    }
	fclose(out);
}

void resize(image *in, image * out) {
    pthread_t tred[num_threads + 5];
    treduri tt[num_threads + 5];
    out->version[0] = 'P';
    out->version[1] = in->version[1];
    out->version[2] = '\0';
    int height = in->height/resize_factor;
    out->height = height;
    int weight = in->weight/resize_factor;
    out->weight = weight;
    int maxValue = in->maxValue;
    out->maxValue = maxValue;
    if(!strcmp(in->version, "P5")) {
	    out->matrix_bw = malloc(out->weight * out->height * sizeof(unsigned char));
    } else {
	    out->matrix_color = malloc(out->weight * out->height * sizeof(pixel));
    }
    int pas = (out->height * out->weight)/num_threads + 1;
    for(int i = 0; i < num_threads; i++) {
        tt[i].start = i;
        tt[i].in = in;
        tt[i].out = out;
	tt[i].pas = pas;
    }
    for(int i = 0; i < num_threads; i++) {
        pthread_create(&(tred[i]), NULL, threadFunction, &(tt[i]));
    }
    for(int i = 0; i < num_threads; i++) {
	    pthread_join(tred[i], NULL);
    }
}
