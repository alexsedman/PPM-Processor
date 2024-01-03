//
// Sedman, Jason
// Image Processing Program CW
// A simple C program for PPM image manipulation 
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#define MAX_FILENAME_LENGTH 200
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

//----------------STRUCTURES----------------//
//------------------------------------------//
// Structure to store PPM image data with a pointer to pixel data.
// As the data will be allocated in a one-dimensional data array, this will be very efficient
typedef struct {
    char format[3];
    int width, height, max_colour;
    unsigned char *data;
} PPMImage;

//-----------------FUNCTION-----------------//
//----------------PROTOTYPES----------------//
// Instanciating the functions before main to avoid errors
PPMImage *readPPM(const char *filename); // Task 1
void savePPM(const char *filename, PPMImage *image); // Task 2
void displayPPM(PPMImage *image); // Task 3
PPMImage *addPPM(PPMImage *image1, PPMImage *image2); // Task 4
PPMImage *edgePPM(PPMImage *image); // Task 5
PPMImage *patternPPM(PPMImage *image1, PPMImage *image2); // Task 6
PPMImage *drawBox(PPMImage *image, int width, int height, int x, int y, int boxWidth, int boxHeight);

//----------------MAIN---------------------//
//-----------------------------------------//
int main() {    
    // Variables declared. Input files 1 and 2 correspond to images 1 and 2.
    char inputFile1[MAX_FILENAME_LENGTH];
    char inputFile2[MAX_FILENAME_LENGTH];
    char outputFile[MAX_FILENAME_LENGTH];
    PPMImage *image1;
    PPMImage *image2;
    char userInput; // User input
    char cwd[MAX_FILENAME_LENGTH]; // Working directory

    while (1) {
        // Output menu
        printf("\n");
        printf("Welcome to the Image Processor.\n");

        // Prints current directory
        getcwd(cwd, sizeof(cwd));
        printf("Current working directory: %s\n\n", cwd);

        printf("--MENU--\n");
        printf("Type 'r' to read your PPM file\n");
        printf("Type 's' to save your PPM file\n");
        printf("Type 'd' to display your PPM file\n");
        printf("Type 'a' to add two PPM files together\n");
        printf("Type 'e' to perform an edge detection\n");
        printf("Type 'p' to detect a pattern in your PPM file\n");
        printf("Type 'q' to quit the program\n");
        printf("Enter your choice: ");
        scanf(" %c", &userInput);
    
        // Clears the input buffer by cycling through each inputted character with a while loop
        while (getchar() != '\n');

        // Switch-case layout to perform the desired functions
        switch (userInput) {
        
            // READ PPM (r)
            case 'r':
                // Free memory if already used, to avoid overflow
                if (image1 != NULL) {
                    free(image1);
                    free(image1->data);
                }

                printf("Enter the input filepath: ");
                scanf("%99s", inputFile1);

                // Clear buffer
                while (getchar() != '\n');

                // Read PPM image from file
                image1 = readPPM(inputFile1);
                break;

            // SAVE PPM (s)
            case 's':
                // Read check
                if (image1 == NULL) {
                    printf("Error: No image has been read yet. Please read an image first.\n");
                    break;
                }

                printf("Enter the output filename: ");
                scanf("%99s", outputFile);

                // Save PPM image to file
                savePPM(outputFile, image1);
                break;

            // DISPLAY PPM DATA (d)
            case 'd':
                // Read check
                if (image1 == NULL) {
                    printf("Error: No image has been read yet. Please read an image first.\n");
                    break;
                }

                // Displays a PPM image
                displayPPM(image1);
                break;

            // ADD PPMS (a)
            case 'a':
                // Free memory if needed
                if (image1 != NULL) {
                    free(image1);
                    free(image1->data);
                }
                if (image2 != NULL) {
                    free(image2);
                    free(image2->data);
                }

                // Adds two PPM images together
                // Get the filenames from the user and read
                // File 1
                printf("Enter the first PPM filepath: ");
                scanf("%s", inputFile1);
                image1 = readPPM(inputFile1);

                // Clear buffer
                while (getchar() != '\n');

                // File 2
                printf("Enter the second PPM filepath: ");
                scanf("%s", inputFile2);
                image2 = readPPM(inputFile2);

                // Clear buffer
                while (getchar() != '\n');

                // Error checks
                // Check if the images are of the same size
                if (image1->width != image2->width || image1->height != image2->height) {
                    printf("Error: Images are not of the same size.\n");
                    break;
                // Check that both images are the same type
                } else if (strcmp(image1->format, image2->format) != 0) {
                    printf("Error: Images have different magic values.\n");
                    break;
                // Check that images were read properly
                } else if (image1 == NULL || image2 == NULL) {
                    printf("Error: Image read failed. Please make sure your filepaths are correct.\n");
                    break;
                }

                PPMImage *combinedImage = addPPM(image1, image2);

                // Write the combined image to a new file
                savePPM("combined.ppm", combinedImage);
                free(combinedImage);

                printf("Images combined successfully. Result saved to combined.ppm\n");
                break;

            // EDGE DETECT (e)
            case 'e':
                // Read check
                if (image1 == NULL) {
                    printf("Error: No image has been read yet. Please read an image first.\n");
                    break;
                }

                PPMImage *edgeImage = edgePPM(image1);
                savePPM("edgedetect.ppm", edgeImage);
                free(edgeImage);
                break;

            // PATTERN DETECT (p)
            case 'p':
                // Free memory if needed
                if (image1 != NULL) {
                    free(image1);
                    free(image1->data);
                }
                if (image2 != NULL) {
                    free(image2);
                    free(image2->data);
                }

                // Get the filenames from the user and read
                // File 1
                printf("Enter the first PPM filepath: ");
                scanf("%s", inputFile1);
                image1 = readPPM(inputFile1);

                // File 2
                printf("Enter the second PPM filepath: ");
                scanf("%s", inputFile2);
                image2 = readPPM(inputFile2);

                // Error checks
                // Check that both images are the same type
                if (strcmp(image1->format, image2->format) != 0) {
                    printf("Error: Images have different magic values.\n");
                    break;
                // Check that images were read properly
                } else if (image1 == NULL || image2 == NULL) {
                    printf("Error: Image read failed. Please make sure your filepaths are correct.\n");
                    break;
                }

                PPMImage *patternImage = patternPPM(image1, image2);

                // Breaks if pattern not found and NULL returned
                if (patternImage == NULL) {
                    break;
                }

                // Save file
                savePPM("patterndetect.ppm", patternImage);
                free(patternImage);
                break;

            // QUIT (q)
            case 'q':
                printf("Exiting program.\n");
                return 0;

            // INVALID INPUT
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

//----------------FUNCTIONS----------------//
//-----------------------------------------//
// 1: Function to read PPM image from a file (r).
PPMImage *readPPM(const char *filename) {
    // Attempts to open file
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Allocates memory for file
    PPMImage *image = (PPMImage *)malloc(sizeof(PPMImage));
    // Error if fails to allocate memory
    if (!image) {
        fprintf(stderr, "Memory allocation failed for PPMImage\n");
        fclose(file);
        return NULL;
    }

    // Checks file format is correct before continuing
    fscanf(file, "%2s", image->format);
    if (strcmp(image->format, "P2") != 0 && strcmp(image->format, "P3") != 0) {
        fprintf(stderr, "Invalid PPM file format. This program only uses P2 or P3 formatted PPMs.\n");
        free(image);
        fclose(file);
        return NULL; // Throws error and returns if format is wrong
    }

    // Complete reading header
    fscanf(file, "%d %d", &image->width, &image->height);
    fscanf(file, "%d", &image->max_colour);
    // Consume the newline character after max_colour
    fgetc(file);
    
    // Allocate memory for data
    if (strcmp(image->format, "P2") == 0) {
        // P2 format (grayscale)
        image->data = (unsigned char *)malloc(image->width * image->height);
    } else if (strcmp(image->format, "P3") == 0) {
        // P3 format (RGB)
        image->data = (unsigned char *)malloc(3 * image->width * image->height);
    }

    // Throw error if fail
    if (!image->data) {
        fprintf(stderr, "Memory allocation failed for image data\n");
        free(image);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < image->width * image->height; ++i) {
        if (strcmp(image->format, "P2") == 0) {
            // P2 format (grayscale)
            // Reads 1 byte at a time
            fscanf(file, "%hhu", &image->data[i]);
        } else if (strcmp(image->format, "P3") == 0) {
            // P3 format (RGB)
            // Reads 3 bytes at a time
            fscanf(file, "%hhu %hhu %hhu", &image->data[i * 3], &image->data[i * 3 + 1], &image->data[i * 3 + 2]);
        }
    }

    // Close file and return
    fclose(file);
    printf("PPM file read successfully.\n");
    return image;
}

// 2: Function to save PPM image to file (s).
void savePPM(const char *filename, PPMImage *image) {
    FILE *file = fopen(filename, "wb");

    // Throw error if file open fails
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write header
    fprintf(file, "%s\n", image->format);
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "%d\n", image->max_colour);

    // Write pixel data to file for either grayscale or RGB.
    if (strcmp(image->format, "P2") == 0) {
        // P2 format (grayscale)
        // Writes 1 byte at at a time
        for (int i = 0; i < image->width * image->height; ++i) {
            fprintf(file, "%hhu ", image->data[i]);
        }
    } else if (strcmp(image->format, "P3") == 0) {
        // P3 format (RGB)
        // Writes 3 bytes at a time with a newline between each of the RGB pixel values
        for (int i = 0; i < image->width * image->height; ++i) {
            fprintf(file, "%hhu %hhu %hhu\n", image->data[i * 3], image->data[i * 3 + 1], image->data[i * 3 + 2]);
        }
    }

    fclose(file);
}

// 3: Function to display PPM image data (d).
void displayPPM(PPMImage *image) {
    // Header info
    printf("\nFormat: %s\n", image->format);
    printf("Width: %d\n", image->width);    
    printf("Height: %d\n", image->height);
    printf("Max Colour: %d\n", image->max_colour);

    // Display data
    // NOTE: for each function, the following universally represent each pixel value for RGB:
    // data[i * 3] - RED
    // data[i * 3 + 1] - GREEN
    // data[i * 3 + 2] - BLUE
    // All of the RGB pixel data will always be in the correct position in the array this way
    // This is because the program uses 1-dimensional pointer managed data structures for efficiency
    // (although this does sacrifice readability)
    printf("Pixel Data:\n");
    if (strcmp(image->format, "P2") == 0) {
        // P2 format (grayscale)
        // Display 1 byte at at a time
        for (int i = 0; i < image->width * image->height; ++i) {
            printf("%hhu ", image->data[i]);
        }
    } else if (strcmp(image->format, "P3") == 0) {
        // P3 format (RGB)
        // Display 3 bytes at a time
        for (int i = 0; i < image->width * image->height; ++i) {
            printf("%hhu %hhu %hhu\n", image->data[i * 3], image->data[i * 3 + 1], image->data[i * 3 + 2]);
        }
    }
}

// 4: Function to add two images (a).
PPMImage *addPPM(PPMImage *image1, PPMImage *image2) {
    PPMImage *combinedImage = (PPMImage *)malloc(sizeof(PPMImage)); // Creates a data structure for the combined image

    // Copies image1 header info to combined image
    strcpy(combinedImage->format, image1->format);
    combinedImage->width = image1->width;
    combinedImage->height = image1->height;
    combinedImage->max_colour = image1->max_colour;

    // Allocate memory for combined image data
    combinedImage->data = (unsigned char *)malloc(sizeof(image1->data));

    // Combine the images pixel by pixel
    for (int i = 0; i < combinedImage->width * combinedImage->height; i++) {
        // Take the average of corresponding pixels from both images, for either grayscale or RGB
        if (strcmp(combinedImage->format, "P2") == 0) {
            // Grayscale add
            combinedImage->data[i] = (image1->data[i] + image2->data[i]) / 2;
        } else if (strcmp(combinedImage->format, "P3") == 0) {
            // RGB add
            combinedImage->data[i * 3] = (image1->data[i * 3] + image2->data[i * 3]) / 2; // Red pixel add
            combinedImage->data[i * 3 + 1] = (image1->data[i * 3 + 1] + image2->data[i * 3 + 1]) / 2; // Green pixel add
            combinedImage->data[i * 3 + 2] = (image1->data[i * 3 + 2] + image2->data[i * 3 + 2]) / 2; // Blue pixel add
        }
    }

    return combinedImage;
}

// 5: Function to edge detect (e).
PPMImage *edgePPM(PPMImage *image) {
    // Create a new PPMImage to store the results
    PPMImage *edgeImage = (PPMImage*)malloc(sizeof(image));

    // Copy header information to new image
    strcpy(edgeImage->format, image->format);
    edgeImage->width = image->width;
    edgeImage->height = image->height;
    edgeImage->max_colour = image->max_colour;

    // Allocate memory for new image data
    edgeImage->data = (unsigned char *)malloc(sizeof(image->data));

    // Define Sobel kernels
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Note: for loops here use different starting values for mathematical reasons
    if (strcmp(edgeImage->format, "P2") == 0) {
         // Grayscale edge detection algorithm
        for (int y = 1; y < image->height - 1; ++y) {
            for (int x = 1; x < image->width - 1; ++x) {
                // Apply the Sobel operator for grayscale
                int intensityX = 0, intensityY = 0;

                // For each row and column, apply the Sobel convolution matrix ('sliding window')
                for (int row = -1; row <= 1; ++row) {
                    for (int col = -1; col <= 1; ++col) {
                        int idx = (y + row) * image->width + x + col;
                        intensityX += sobelX[row + 1][col + 1] * image->data[idx]; // Grayscale horizontal convolution
                        intensityY += sobelY[row + 1][col + 1] * image->data[idx]; // Grayscale vertical convolution
                    }
                }

                // Combine the results
                int resultIdx = y * image->width + x;
                edgeImage->data[resultIdx] = (unsigned char)(abs(intensityX) + abs(intensityY)) / 2;
            }
        }   
            
    } else if (strcmp(edgeImage->format, "P3") == 0) {
        // RGB edge detection algorithm
        for (int y = 1; y < image->height - 1; ++y) {
            for (int x = 1; x < image->width - 1; ++x) {
                // Apply the Sobel operator for RGB
                int redX = 0, greenX = 0, blueX = 0;
                int redY = 0, greenY = 0, blueY = 0;

                // For each row and column, apply the Sobel convolution matrix
                for (int row = -1; row <= 1; ++row) {
                    for (int col = -1; col <= 1; ++col) {
                        int idx = 3 * ((y + row) * image->width + x + col); // Assigning pixel id for the data array (i.e. data[idx])
                        redX += sobelX[row + 1][col + 1] * image->data[idx]; // Red pixel horizontal convolution
                        greenX += sobelX[row + 1][col + 1] * image->data[idx + 1]; // Green pixel horizontal convolution
                        blueX += sobelX[row + 1][col + 1] * image->data[idx + 2]; // Blue pixel horizontal convolution

                        redY += sobelY[row + 1][col + 1] * image->data[idx]; // Red pixel vertical convolution
                        greenY += sobelY[row + 1][col + 1] * image->data[idx + 1]; // Green pixel vertical convolution
                        blueY += sobelY[row + 1][col + 1] * image->data[idx + 2]; // Blue pixel vertical convolution
                    }
                }

                // Combine the results (magnitude calculation of Sobel function)
                int resultIdx = 3 * (y * image->width + x); // Assigning pixel id for the data array (i.e. data[idx])
                edgeImage->data[resultIdx] = (unsigned char)(abs(redX) + abs(redY)) / 2;
                edgeImage->data[resultIdx + 1] = (unsigned char)(abs(greenX) + abs(greenY)) / 2;
                edgeImage->data[resultIdx + 2] = (unsigned char)(abs(blueX) + abs(blueY)) / 2;
            }
        }
    }
    
    return edgeImage;
}

// 6: Function to pattern detect (p).
PPMImage *patternPPM(PPMImage *image1, PPMImage *image2) {
    int x1, y1, x2, y2; // (x,y) coordinates for 

    // For loops for image 1, for cycling through pixels relative to the width and height information of the images
    for (x1 = 0; x1 <= image1->width - image2->width; ++x1) {
        for (y1 = 0; y1 <= image1->height - image2->height; ++y1) {

            int match = 1; // This variable will check for pixel matches between images
                           // 1 for true (a match)
                           // 0 for false (no match)

            // Nested for loops for image 2
            for (x2 = 0; x2 < image2->width; ++x2) {
                for (y2 = 0; y2 < image2->height; ++y2) {
                    int px1, px2;

                    // Find pixel location (for either P2 or P3)
                    // P2
                    if (strcmp(image1->format, "P2") == 0) {
                        px1 = (y1 + y2) * image1->width + (x1 + x2);
                        px2 = y2 * image2->width + x2;

                        // Check pixel equality for grayscale
                        if (image1->data[px1] != image2->data[px2]) {
                            match = 0;
                            break;
                        }

                    // P3
                    } else if (strcmp(image1->format, "P3") == 0) {
                        px1 = 3 * ((y1 + y2) * image1->width + (x1 + x2));
                        px2 = 3 * (y2 * image2->width + x2);
                        
                        // Check pixel equality for RGB
                        if (image1->data[px1] != image2->data[px2] ||
                        image1->data[px1 + 1] != image2->data[px2 + 1] ||
                        image1->data[px1 + 2] != image2->data[px2 + 2]) {
                            match = 0;
                            break;
                        }
                    }                    
                }

                // If match is still 0 after the image2 scan, then just restart on the next image1 position
                if (!match) {
                    break;
                }
            }

            // If the (x2, y2) for loop completes without breaking, then a match has been found
            if (match) {
                printf("Second image found at position: (%d, %d)\n", x1, y1);

                // Creates a copy of image1:
                // Create a new PPMImage to store the results
                PPMImage *patternImage = (PPMImage*)malloc(sizeof(image1));
                // Copy header information to new image
                strcpy(patternImage->format, image1->format);
                patternImage->width = image1->width;
                patternImage->height = image1->height;
                patternImage->max_colour = image1->max_colour;
                // Allocate memory for new image data
                patternImage->data = (unsigned char *)malloc(sizeof(image1->data));
                // Copy data from image1 to patternImage
                for (int i = 0; i < image1->width * image1->height * 3; i++) {
                    patternImage->data[i] = image1->data[i];
                }

                // Draws a box over the copied image using 'drawBox()'
                patternImage = drawBox(patternImage, patternImage->width, patternImage->height, x1, y1, image2->width, image2->height);
                return patternImage;
            }
        }
    }

    printf("Second image not found in the first image!\n");
    return NULL;
}

// X: Draw box around pattern-detected images, using arguments from 'patternPPM'
PPMImage *drawBox(PPMImage *image, int width, int height, int x, int y, int boxWidth, int boxHeight) {
    // Draw a 1-pixel thick orange border around the specified region in the new image
    int i, j;

    // Grayscale border
    if (strcmp(image->format, "P2") == 0) {

        // Top and bottom borders drawn using for loop
        for (i = x; i < x + boxWidth; ++i) {
            image->data[y * width + i] = image->max_colour; // Set pixel value to maximum intensity (white)
            image->data[(y + boxHeight - 1) * width + i] = image->max_colour;  
        }

        // Left and right borders drawn using for loop
        for (j = y; j < y + boxHeight; ++j) {
            image->data[j * width + x] = image->max_colour;
            image->data[j * width + x + boxWidth - 1] = image->max_colour;  
        }
        
    // RGB border
    } else if (strcmp(image->format, "P3") == 0) {
        // Top and bottom borders drawn using for loop
        for (i = x; i < x + boxWidth; ++i) {
            image->data[(y * width + i) * 3] = 255; // Red
            image->data[(y * width + i) * 3 + 1] = 0; // Green
            image->data[(y * width + i) * 3 + 2] = 0; // Blue

            image->data[((y + boxHeight - 1) * width + i) * 3] = 255; // Red
            image->data[((y + boxHeight - 1) * width + i) * 3 + 1] = 0; // Green
            image->data[((y + boxHeight - 1) * width + i) * 3 + 2] = 0; // Blue
        }

        // Left and right borders drawn using for loop
        for (j = y; j < y + boxHeight; ++j) {
            image->data[(j * width + x) * 3] = 255; // Red
            image->data[(j * width + x) * 3 + 1] = 0; // Green
            image->data[(j * width + x) * 3 + 2] = 0; // Blue

            image->data[(j * width + x + boxWidth - 1) * 3] = 255; // Red
            image->data[(j * width + x + boxWidth - 1) * 3 + 1] = 0; // Green
            image->data[(j * width + x + boxWidth - 1) * 3 + 2] = 0; // Blue
        }
    }
    return image;
}