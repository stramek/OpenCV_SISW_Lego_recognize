//
//  processUtils.hpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 28.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#ifndef processUtils_hpp
#define processUtils_hpp
#ifdef __GNUC__
#pragma GCC system_header
#endif

#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "constants.hpp"
#include "utils.hpp"
#include "OutputModel.hpp"

using namespace cv;
using namespace std;

namespace processUtils {
    Mat preprocessImage(Mat& input);
    enum OutputModel::blocksType detectColorShape(Mat orginalCroppedPhoto, Mat processedCroppedPhoto, bool debug);
    Mat colorRecog(Mat& input);
    Mat getCroppedAndRotatedMat(RotatedRect rect, Mat& source);
    enum OutputModel::blocksType recogniseShape(array<array<bool, 3>, 2> passedArray, Mat& input);
    void processImages(vector<Mat>& images, bool debug = false);
    array<array<bool, 3>, 2> getShapeArrays(Mat& mat);
    pair<Mat, vector<RotatedRect>> getRecogniseResources(Mat& input, bool debug = false);
    void debugStuff(enum OutputModel::blocksType color, enum OutputModel::blocksType shape, Mat& image, RotatedRect rect);
}

#endif /* processUtils_hpp */
