//
//  OutputModel.hpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#ifndef OutputModel_hpp
#define OutputModel_hpp

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>

class OutputModel {
    public:
        enum blocksType {
            SHAPE_I,
            SHAPE_T,
            SHAPE_JL,
            SHAPE_O,
            SHAPE_SZ,
            COLOR_RED,
            COLOR_GREEN,
            COLOR_BLUE,
            COLOR_WHITE,
            COLOR_YELLOW,
            COLOR_MIX,
            THRASH
        };
        OutputModel();
        void setTypeValue(enum blocksType type, int number);
        void incrementTypeValue(enum blocksType type);
        int getNumberOfDetectedShapes();
        int getNumberOfDetectedSolidColors();
        void saveOutput();
    private:
        std::array<int, THRASH> blocksType;
};

#endif /* OutputModel_hpp */
