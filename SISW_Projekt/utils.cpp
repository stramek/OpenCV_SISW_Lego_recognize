//
//  utils.cpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 26.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#include "utils.hpp"

namespace utils {
    
    void printProgressBar(const int currentFile, const int numberOfFiles) {
        
        float progress = currentFile / (float) numberOfFiles;
        
        int barWidth = 70;
        
        cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << int(progress * 100.0) << " %\r";
        cout.flush();
    }
    
    void loadPhotosNames(vector<string>& vec) {
        cout<<"Loading file \""<<PHOTOS_LIST_FILE_NAME<<"\"... ";
        ifstream myfile(PHOTOS_LIST_FILE_PATH);
        if (myfile.good()) {
            cout<<SUCCESS_MESSAGE<<endl<<endl;
            copy(istream_iterator<string>(myfile), istream_iterator<string>(), back_inserter(vec));
        } else {
            cout<<FAIL_MESSAGE<<endl<<endl;
            exit(-1);
        }
    }
    
    void loadImages(vector<Mat>& images) {
        vector<string> photosNames;
        loadPhotosNames(photosNames);
        
        cout<<"Loading "<<to_string(photosNames.size())<<" images to analyze... "<<endl;
        
        for (string fileName : photosNames) {
            Mat image = imread(PHOTOS_PATH + fileName);
            if (image.data) {
                images.push_back(image);
                printProgressBar((int) images.size(), (int) photosNames.size());
            }
        }
        
        if (photosNames.size() == images.size()) {
            cout<<SUCCESS_MESSAGE<<endl<<endl;
        } else {
            if (FORCE_TO_LOAD_ALL_PHOTOS) {
                cout<<FAIL_MESSAGE<<endl<<"Loaded "<<to_string(images.size())<<" of "<<to_string(photosNames.size())<<" files. Program will stop."<<endl;
                exit(-1);
            } else {
                cout<<"Loaded "<<to_string(images.size())<<" of "<<to_string(photosNames.size())<<" files. Program will contunue anyway."<<endl;
            }
        }
    }
    
    char* string2char(std::string str) {
        std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
        return cstr.data();
    }
}
