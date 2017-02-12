//
//  main.cpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#include "main.hpp"

int main(int argc, const char * argv[]) {
    vector<Mat> images;
    utils::loadImages(images);
    processUtils::processImages(images);
    return 0;
}
