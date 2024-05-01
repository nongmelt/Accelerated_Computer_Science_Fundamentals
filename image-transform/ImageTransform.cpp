#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>

#include "ImageTransform.h"
#include "uiuc/HSLAPixel.h"
#include "uiuc/PNG.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name: Pongporn Supa
Email: pongporn.supa@gmail.com

(...end multi-line comment.)
******************** */

using uiuc::HSLAPixel;
using uiuc::PNG;

// Constant variables
const double ILLINI_ORANGE = 11.0;
const double ILLINI_BLUE = 216.0;
const double HALF_CIRCLE_DEGREE = 180.0;
const double FULL_CIRCLE_DEGREE = 360.0;
const double LUMINANCE_DISTANCE = 160.0;
const double LUMINANCE_DECREASE = 0.005;
const double LUMINANCE_MAXIMUM = 0.8;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel &pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}

/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel
 * euclidean distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 *
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      double dist =
          sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
      HSLAPixel &pixel = image.getPixel(x, y);
      pixel.l = (dist < LUMINANCE_DISTANCE)
                    ? (pixel.l * (1 - LUMINANCE_DECREASE * dist))
                    : (pixel.l * (1 - LUMINANCE_MAXIMUM));
    }
  }
  return image;
}

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
 **/
PNG illinify(PNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel &pixel = image.getPixel(x, y);
      double diff_blue = abs(pixel.h - ILLINI_BLUE);
      double diff_orange = abs(pixel.h - ILLINI_ORANGE);
      if (diff_blue > HALF_CIRCLE_DEGREE) {
        diff_blue = FULL_CIRCLE_DEGREE - diff_blue;
      }
      if (diff_orange > HALF_CIRCLE_DEGREE) {
        diff_orange = FULL_CIRCLE_DEGREE - diff_orange;
      }

      pixel.h = (diff_blue < diff_orange) ? ILLINI_BLUE : ILLINI_ORANGE;
    }
  }

  return image;
}

/**
 * Returns an immge that has been watermarked by another image.
 *
 * The luminance of every pixel of the second image is checked, if that
 * pixel's luminance is 1 (100%), then the pixel at the same location on
 * the first image has its luminance increased by 0.2.
 *
 * @param firstImage  The first of the two PNGs, which is the base image.
 * @param secondImage The second of the two PNGs, which acts as the stencil.
 *
 * @return The watermarked image.
 */
PNG watermark(PNG firstImage, PNG secondImage) {
  for (unsigned x = 0; x < firstImage.width(); x++) {
    for (unsigned y = 0; y < firstImage.height(); y++) {
      HSLAPixel &first_pixel = firstImage.getPixel(x, y);
      HSLAPixel &second_pixel = secondImage.getPixel(x, y);
      if (fabs(second_pixel.l - 1.0) <= 10e-6) {
        first_pixel.l = std::min(first_pixel.l + 0.2, 1.0);
      }
    }
  }
  return firstImage;
}
