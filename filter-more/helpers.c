#include "helpers.h"
#include <math.h>

int sobeloperator(float gx, float gy);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++)
    {
        for (int j=0; j<width; j++)
        {
            RGBTRIPLE px = image[i][j];

            float red = (float)px.rgbtRed;
            float blue = (float)px.rgbtBlue;
            float green = (float)px.rgbtGreen;

            float avg = (red + blue + green)/3.0;
            int iavg = (int) round(avg);

            image[i][j].rgbtRed = iavg;
            image[i][j].rgbtBlue = iavg;
            image[i][j].rgbtGreen = iavg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++)
    {
        for (int j=0; j < width/2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy image because we need original values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float rsum = 0.0;
            float bsum = 0.0;
            float gsum = 0.0;
            float terms = 0.0;

            for (int n=-1; n<2; n++)
            {
                for (int m=-1; m<2; m++)
                {
                    int a = i+n;
                    int b = j+m;

                    if (a<height && b >= 0 && b<width && a >= 0)
                    {
                        terms += 1.0;
                        RGBTRIPLE px = copy[a][b];
                        rsum += (float)px.rgbtRed;
                        bsum += (float)px.rgbtBlue;
                        gsum += (float)px.rgbtGreen;
                    }
                }
            }

            int ravg = (int) round(rsum/terms);
            int bavg = (int) round(bsum/terms);
            int gavg = (int) round(gsum/terms);

            image[i][j].rgbtRed = ravg;
            image[i][j].rgbtBlue = bavg;
            image[i][j].rgbtGreen = gavg;
        }
    }
    return;
}

// Detect edges
// To compute Gx for the red channel value of a pixel, for instance, we’ll take the original red values for
// the nine pixels that form a 3x3 box around the pixel, multiply them each by the corresponding value in
//the Gx kernel, and take the sum of the resulting values.
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float rgx = 0.0, bgx = 0.0, ggx = 0.0;
            float rgy = 0.0, bgy = 0.0, ggy = 0.0;

            for (int n=-1; n<2; n++)
            {
                for (int m=-1; m<2; m++)
                {
                    int a = i+n;
                    int b = j+m;

                    if (a<height && b >= 0 && b<width && a >= 0)
                    {
                        RGBTRIPLE px = copy[a][b];

                        int matx = x[n+1][m+1];
                        int maty = y[n+1][m+1];

                        rgx += (float)px.rgbtRed*matx;
                        bgx += (float)px.rgbtBlue*matx;
                        ggx += (float)px.rgbtGreen*matx;
                        rgy += (float)px.rgbtRed*maty;
                        bgy += (float)px.rgbtBlue*maty;
                        ggy += (float)px.rgbtGreen*maty;
                    }
                }
            }

            int rsobel = sobeloperator(rgx, rgy);
            int bsobel = sobeloperator(bgx, bgy);
            int gsobel = sobeloperator(ggx, ggy);

            image[i][j].rgbtRed = rsobel;
            image[i][j].rgbtBlue = bsobel;
            image[i][j].rgbtGreen = gsobel;
        }
    }
    return;
}

int sobeloperator(float gx, float gy)
{
    int sobel = (int) round(sqrt(pow(gx, 2) + pow(gy, 2)));

    if (sobel < 0)
    {
        sobel = 0;
    } else if (sobel > 255)
    {
        sobel = 255;
    }
    return sobel;
}
