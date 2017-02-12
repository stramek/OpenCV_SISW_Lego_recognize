//
//  utils.hpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp
#ifdef __GNUC__
#pragma GCC system_header
#endif

#include "opencv2/opencv.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "constants.hpp"

using namespace cv;
using namespace std;

namespace utils {
    void printProgressBar(int currentFile, int numberOfFiles);
    void loadPhotosNames(vector<string>& vec);
    void loadImages(vector<Mat>& images);
    char* string2char(std::string str);
}

#endif /* utils_hpp */
