//
//  OutputModel.cpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#include "OutputModel.hpp"
#include "constants.hpp"
#include "utils.hpp"

OutputModel::OutputModel() {
    remove(utils::string2char(OUTPUT_PATH));
    blocksType.fill(0);
}

void OutputModel::setTypeValue(enum blocksType type, int number) {
    blocksType.at(type) = number;
}

void OutputModel::incrementTypeValue(enum blocksType type) {
    blocksType.at(type) = ++blocksType.at(type);
}

int OutputModel::getNumberOfDetectedShapes() {
    int numberOfRecognizedShapes = 0;
    for (int i = 0; i < blocksType::COLOR_RED; ++i) {
        numberOfRecognizedShapes += blocksType[i];
    }
    return numberOfRecognizedShapes;
}

int OutputModel::getNumberOfDetectedSolidColors() {
    int numberOfRecognizedColors = 0;
    for (int i = blocksType::COLOR_RED; i < blocksType::COLOR_MIX; ++i) {
        numberOfRecognizedColors += blocksType[i];
    }
    return numberOfRecognizedColors;
}

void OutputModel::saveOutput() {
    
    std::ofstream outfile;
    outfile.open(utils::string2char(OUTPUT_PATH), std::ios_base::app);
    
    if (!outfile.is_open()) {
        cout<<FAIL_MESSAGE<<endl;
        exit(-1);
    }
    
    for (int i = 0; i < blocksType.size(); ++i) {
        outfile << std::to_string(blocksType[i]);
        if (i != blocksType.size() -1) {
            outfile << ", ";
        }
    }
    
    outfile << std::endl;
    outfile.close();
    blocksType.fill(0);
}
