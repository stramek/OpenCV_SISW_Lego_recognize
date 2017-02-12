//
//  constants.hpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#ifndef constants_hpp
#define constants_hpp
#ifdef __GNUC__
#pragma GCC system_header
#endif

#include "opencv2/opencv.hpp"

const bool FORCE_TO_LOAD_ALL_PHOTOS = true;

const cv::Scalar YELLOW_MIN = cv::Scalar(15, 115, 50);
const cv::Scalar YELLOW_MAX = cv::Scalar(35, 255, 255);
const cv::Scalar GREEN_MIN = cv::Scalar(70, 55, 50);
const cv::Scalar GREEN_MAX = cv::Scalar(85, 255, 255);
const cv::Scalar RED_LOWER_MIN = cv::Scalar(0, 80, 50);
const cv::Scalar RED_LOWER_MAX = cv::Scalar(10, 255, 255);
const cv::Scalar RED_UPPER_MIN = cv::Scalar(160, 55, 50);
const cv::Scalar RED_UPPER_MAX = cv::Scalar(179, 255, 255);
const cv::Scalar BLUE_MIN = cv::Scalar(90, 55, 50);
const cv::Scalar BLUE_MAX = cv::Scalar(115, 255, 255);
const cv::Scalar WHITE_MIN = cv::Scalar(0, 0, 150);
const cv::Scalar WHITE_MAX = cv::Scalar(180, 50, 255);
const cv::Scalar BLACK_MIN = cv::Scalar(0, 0, 0);
const cv::Scalar BLACK_MAX = cv::Scalar(180, 255, 10);

const bool L_SHAPE_TEMPLATE[4][2][3] = {{
    {1, 0, 0},
    {1, 1, 1}
},{
    {1, 1, 1},
    {1, 0, 0}
},{
    {0, 0, 1},
    {1, 1, 1}
},{
    {1, 1, 1},
    {0, 0, 1}
}};

const bool T_SHAPE_TEMPLATE[2][2][3] = {{
    {0, 1, 0},
    {1, 1, 1}
},{
    {1, 1, 1},
    {0, 1, 0}
}};

const bool S_SHAPE_TEMPLATE[2][2][3] = {{
    {0, 1, 1},
    {1, 1, 0}
},{
    {1, 1, 0},
    {0, 1, 1}
}};

const bool I_OR_O_TEMPLATE[1][2][3] = {{
    {1, 1, 1},
    {1, 1, 1}
}};

const double MINIMUM_PERCENT = 8.0;

const std::string SUCCESS_MESSAGE = "Success!";
const std::string FAIL_MESSAGE = "Fail!";

const std::string PHOTOS_LIST_LOCATION = "nazwy_zdjec";
const std::string PHOTOS_LIST_FILE_NAME = "nazwy_zdjec.txt";
const std::string PHOTOS_LOCATION = "zdjecia";

const std::string OUTPUT_LOCATION = "wyniki";
const std::string OUTPUT_FILE_NAME = "Stramowski_Marcin.txt";

const std::string OUTPUT_PATH = OUTPUT_LOCATION + "/" + OUTPUT_FILE_NAME;
const std::string PHOTOS_LIST_FILE_PATH = PHOTOS_LIST_LOCATION + "/" + PHOTOS_LIST_FILE_NAME;
const std::string PHOTOS_PATH = PHOTOS_LOCATION + "/";

#endif /* constants_hpp */
