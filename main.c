#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include "Image.h"
#include "Utils.h"
#include "Kernel.h"
#include "Processing.h"

/**
 * Min value of kernel dimension to test (MUST be odd)
 */
const int KERNEL_DIM_MIN = 3;
/**
 * Max value of kernel dimension to test (MUST be odd)
 */
const int KERNEL_DIM_MAX = 5;
/**
 * Step on values of kernel dimension (MUST be even)
 */
const int KERNEL_DIM_STEP = 2;
/**
 * Image dimension to test: 4K, 5K, 6K or 7K
 */
const char IMAGE_DIMENSION[] = "4K";
/**
 * Number of image of each dimension to test (max 3)
 */
const int IMAGE_QUANTITY = 3;
/**
 * Number of times to test each image
 */
const int REPETITIONS = 1;

void saveTextFile(int *kDim, double *times, char *filename) {
    FILE *file = fopen(filename, "w");

    for (int k = 0; k <= (KERNEL_DIM_MAX - KERNEL_DIM_MIN); k += KERNEL_DIM_STEP) {
        fprintf(file, "%d %f\n", kDim[k], times[k]);
    }

    fclose(file);
}

int main() {
    int *kDim = (int *) malloc(sizeof(int) * (KERNEL_DIM_MAX - KERNEL_DIM_MIN + 1));
    double *times = (double *) malloc(sizeof(double) * (KERNEL_DIM_MAX - KERNEL_DIM_MIN + 1));

    for (int k = KERNEL_DIM_MIN; k <= KERNEL_DIM_MAX; k += KERNEL_DIM_STEP) {
        double cumulativeSec = 0;
        Kernel *krn = Kernel_gaussianBlur(k);
        for (int imageIndex = 1; imageIndex <= IMAGE_QUANTITY; imageIndex++) {
            char *inFilename = (char *) malloc(sizeof(char) * 100);
            sprintf(inFilename, "/home/kevin/CLionProjects/Image_Kernel_Processing/Image/Input/%s-%d.jpg",
                    IMAGE_DIMENSION, imageIndex);
            Image *img = loadJPEG(inFilename);
            Image *res = NULL;
            for (int i = 0; i < REPETITIONS; i++) {
                Image_delete(res); // to free memory of previous result and avoid error

                clock_t start = clock();
                res = process(img, krn);
                clock_t end = clock();

                cumulativeSec += ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("For kernel dimension = %d on image %d, computation time is %f seconds\n", k, imageIndex,
                       ((double) (end - start)) / CLOCKS_PER_SEC);
            }

            char *outFilename = (char *) malloc(sizeof(char) * 100);
            sprintf(outFilename, "/home/kevin/CLionProjects/Image_Kernel_Processing/Image/Output/%s-%d-processed%d.png",
                    IMAGE_DIMENSION, imageIndex, k);
            savePNG(outFilename, res);

            Image_delete(img);
            Image_delete(res);
            free(inFilename);
            free(outFilename);
        }

        double meanTime = cumulativeSec / (REPETITIONS * IMAGE_QUANTITY);
        kDim[k - KERNEL_DIM_MIN] = k;
        times[k - KERNEL_DIM_MIN] = meanTime;
        printf("For kernel dimension = %d, MEAN computation time is %f seconds\n", k, meanTime);

        Kernel_delete(krn);
    }

    char *filename = (char *) malloc(sizeof(char) * 100);
    sprintf(filename, "/home/kevin/CLionProjects/Image_Kernel_Processing/Image/Output/Times/%sExecTimes.txt",
            IMAGE_DIMENSION);
    saveTextFile(kDim, times, filename);

    free(kDim);
    free(times);
    free(filename);

    return 0;
}