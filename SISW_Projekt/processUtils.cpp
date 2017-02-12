//
//  processUtils.cpp
//  SISW_Projekt
//
//  Created by Marcin Stramowski on 28.12.2016.
//  Copyright © 2016 Marcin Stramowski. All rights reserved.
//

#include "processUtils.hpp"

namespace processUtils {
    
    Mat segmentImages(Mat& input, Mat& colorMat, bool debug) {
        Mat dist;
        distanceTransform(input, dist, CV_DIST_L2, 3);
        normalize(dist, dist, 0, 1.0, NORM_MINMAX);
        threshold(dist, dist, 0.38, 1.0, CV_THRESH_BINARY);
        Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
        dilate(dist, dist, kernel1);
        Mat dist_8u;
        dist.convertTo(dist_8u, CV_8U);
        vector<vector<Point> > contours;
        findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        Mat markers = Mat::zeros(dist.size(), CV_32SC1);
        for (size_t i = 0; i < contours.size(); i++)
            drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), - 1);
        circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
        watershed(colorMat, markers);
        Mat mark = Mat::zeros(markers.size(), CV_8UC1);
        markers.convertTo(mark, CV_8UC1);
        bitwise_not(mark, mark);
        Mat dst = Mat::zeros(markers.size(), CV_8UC3);
        for (int i = 0; i < markers.rows; i++) {
            for (int j = 0; j < markers.cols; j++) {
                int index = markers.at<int>(i, j);
                if (index > 0 && index <= static_cast<int>(contours.size()))
                    dst.at<Vec3b>(i,j) = Vec3b(255,255,255);
                else
                    dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
            }
        }
        if (debug) imshow("Segment final output", dst);
        cvtColor(dst, dst, CV_BGR2GRAY);
        return dst;
    }
    
    Mat getMatOfWrongColors(Mat& mat) {
        Mat toProcess = mat.clone();
        cvtColor(toProcess, toProcess, CV_BGR2HSV);

        inRange(toProcess, BLACK_MIN, BLACK_MAX, toProcess);
        
        int erosion_size = 25;
        Mat element = getStructuringElement(MORPH_RECT, Size(erosion_size, erosion_size));
        dilate(toProcess, toProcess, element);
        
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(toProcess, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        vector<vector<Point> >hull( contours.size() );
        for( int i = 0; i < contours.size(); i++ ) {
            convexHull( Mat(contours[i]), hull[i], false );
        }

        Mat drawing = Mat::zeros(toProcess.size(), CV_8U);
        for( int i = 0; i< contours.size(); i++ ) {
            drawContours( drawing, hull, i, Scalar(255, 255, 255), 5, 8, vector<Vec4i>(), 0, Point());
        }
        
        return drawing;
    }
    
    pair<Mat, vector<RotatedRect>> getRecogniseResources(Mat& input, bool debug) {
        pair<Mat, vector<RotatedRect>> recogniseResources;
        
        // Mat to process
        Mat toProcess = input.clone();
        
        // Get Mat with edges from color recognition
        Mat colorRecogImage = colorRecog(input);
        
        // Convert mat to grayscale
        Mat blackAndWhite;
        cvtColor(toProcess, blackAndWhite, CV_BGR2GRAY);
        
        // Find edges
        Canny(blackAndWhite.clone(), blackAndWhite, 22, 44);
        
        if (debug) imshow("A1", blackAndWhite);

        // Cut the contuours of object with wrong colors
        blackAndWhite -= getMatOfWrongColors(input);
        
        // Find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( blackAndWhite.clone(), contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        // Draw conturs and bold edges
        Mat drawing = Mat::zeros(blackAndWhite.size(), CV_8U);
        for( int i = 0; i< contours.size(); i++) {
            drawContours( drawing, contours, i, Scalar(255, 255, 255), 8, 8, hierarchy, 0, Point() );
        }
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        erode(drawing, drawing, element);
        
        if (debug) imshow("A2", drawing);
        
        // Find contours
        contours.clear();
        hierarchy.clear();
        findContours( drawing.clone(), contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        // Draw conturs and fill
        drawing = Mat::zeros(blackAndWhite.size(), CV_8U);
        for( int i = 0; i< contours.size(); i++) {
            drawContours(drawing, contours, i, Scalar(255, 255, 255), FILLED, 8, hierarchy, 0, Point() );
        }
        
        // Erode to remove bridges
        erode(drawing, drawing, element);
        
        // Add color detected contours
        drawing += colorRecogImage;
        
        // Smooth edges, remove event more bridges
        medianBlur(drawing, drawing, 11);
        
        // Find contours, remove very big and very small shapes
        contours.clear();
        hierarchy.clear();
        findContours( drawing.clone(), contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end();) {
            double area = contourArea(*it);
            
            if (area < 2000 || area > 15000) {
                it = contours.erase(it);
            } else {
                ++it;
            }
        }
        // Draw conturs and fill
        drawing = Mat::zeros(blackAndWhite.size(), CV_8U);
        for( int i = 0; i< contours.size(); i++) {
            drawContours(drawing, contours, i, Scalar(255, 255, 255), FILLED, 8, hierarchy, 0, Point() );
        }
        
        if (debug) imshow("should be removed", drawing);
        
        if (debug) imshow("A2.5", drawing);
        // segment blocks BETA SECTION!!!
        Mat segmentedImage = segmentImages(drawing, input, debug);
        bitwise_not ( segmentedImage, segmentedImage );
        drawing = drawing - segmentedImage;
        medianBlur(drawing, drawing, 3);
        if (debug) imshow("A2.6", drawing);
    
        
        // Find contours once more
        contours.clear();
        hierarchy.clear();
        findContours( drawing.clone(), contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        // Find area of block we looking for with offset
        double mean = 0;
        double offset = 0;
        int numberOfFigures = 0;
        double sum = 0;
        for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end();) {
            double area = contourArea(*it);
            
            int min = 3000;
            int max = 10000;
            
            if (area >= min && area <= max) {
                sum += area;
                ++numberOfFigures;
            }
            ++it;
        }
        
        // If no proper objects found, prevent from marking thrash
        if (numberOfFigures == 0) {
            mean = 6000;
        } else {
            mean = sum / numberOfFigures;
        }
        offset = mean * 0.25;
        
        if (debug)imshow("A3", drawing);
        // Remove too small and too big blocks
        for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end();) {
            double area = contourArea(*it);
            if (area < (mean - offset) || area > (mean + offset * 2.5f)) {
                it = contours.erase(it);
            } else {
                ++it;
            }
        }
        
        // Create Mat from edges
        drawing = Mat::zeros(blackAndWhite.size(), CV_8U);
        for( int i = 0; i< contours.size(); i++) {
            drawContours( drawing, contours, i, Scalar(255, 255, 255), FILLED, 8, hierarchy, 0, Point() );
        }
        
        erode(drawing, drawing, element);
        
        if (debug) imshow("A4", drawing);

        // Get vector of Rotated rects
        contours.clear();
        hierarchy.clear();
        findContours( drawing.clone(), contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        vector<vector<Point> > contours_poly( contours.size() );
        vector<RotatedRect> minRect( contours.size() );
        for( int i = 0; i < contours.size(); i++ ) {
            approxPolyDP( Mat(contours[i]), contours_poly[i], 2, true );
            minRect[i] = minAreaRect( Mat(contours[i]) );
        }
        
        // Wrap everything to pair
        recogniseResources.first = drawing;
        recogniseResources.second = minRect;
        
        return recogniseResources;
    }
    
    void debugStuff(enum OutputModel::blocksType color, enum OutputModel::blocksType shape, Mat& image, RotatedRect rect) {
        
        Point2f rect_points[4];
        rect.points(rect_points);
        for( int j = 0; j < 4; j++ )
            line(image, rect_points[j], rect_points[(j + 1) % 4], Scalar(255,255,255), 1, 8 );
        
        String colorText = "";
        String shapeText = "";
        switch (color) {
            case OutputModel::COLOR_BLUE:
                colorText = "blue";
                break;
            case OutputModel::COLOR_GREEN:
                colorText = "green";
                break;
            case OutputModel::COLOR_WHITE:
                colorText = "white";
                break;
            case OutputModel::COLOR_YELLOW:
                colorText = "yellow";
                break;
            case OutputModel::COLOR_RED:
                colorText = "red";
                break;
            case OutputModel::COLOR_MIX:
                colorText = "mix";
                break;
            default:
                break;
        }
        switch (shape) {
            case OutputModel::SHAPE_JL:
                shapeText = "JL";
                break;
            case OutputModel::SHAPE_I:
                shapeText = "I";
                break;
            case OutputModel::SHAPE_O:
                shapeText = "O";
                break;
            case OutputModel::SHAPE_SZ:
                shapeText = "SZ";
                break;
            case OutputModel::SHAPE_T:
                shapeText = "T";
                break;
            default:
                break;
        }
        if (shapeText == "") shapeText = "Thrash";
        if (colorText == "") colorText = "Thrash";
        putText(image, "Color: " + colorText, rect_points[2], FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
        Point point = rect_points[2];
        point = point + Point(0, 20);
        putText(image, "Shape: " + shapeText, point, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
    }
    
    void processImages(vector<Mat>& images, bool debug) {
        std::unique_ptr<OutputModel> model(new OutputModel());
        
        cout<<"Processing..."<<endl;
        for (int num = 0; num < images.size(); ++num) {

            resize(images[num], images[num], Size(), 0.3, 0.3);
            Mat imageToDraw = images[num].clone();
            
            pair<Mat, vector<RotatedRect>> resources = getRecogniseResources(images[num], debug);
            Mat outputMat = resources.first;
            vector<RotatedRect> rects = resources.second;
            
            for (RotatedRect rect : rects) {
                
                Mat out = getCroppedAndRotatedMat(rect, outputMat);
                Mat inputClone = images[num].clone();
                Mat outCroppedColor = getCroppedAndRotatedMat(rect, inputClone);
                
                array<array<bool, 3>, 2> shapeArrays = getShapeArrays(out);
                enum OutputModel::blocksType color = detectColorShape(outCroppedColor, out, false);
                enum OutputModel::blocksType shape = recogniseShape(shapeArrays, out);
                if (color != OutputModel::blocksType::THRASH && shape != OutputModel::blocksType::THRASH)
                    model->incrementTypeValue(color);
                if (color != OutputModel::blocksType::THRASH && shape != OutputModel::blocksType::THRASH)
                    model->incrementTypeValue(shape);
                if (debug) debugStuff(color, shape, imageToDraw, rect);
            }
            model->saveOutput();
            utils::printProgressBar(num + 1, (int)images.size());
            if (debug) {
                imshow("Debug window", imageToDraw);
                waitKey();
            }
        }
        cout<<SUCCESS_MESSAGE<<endl<<endl;
    }
    
    array<array<bool, 3>, 2> getShapeArrays(Mat& mat) {
        int height = mat.rows;
        int width = mat.cols;
        int squareHeight = height / 2;
        int squareWidth = width / 3;
        array<array<bool, 3>, 2> arrays;
        
        int startWidth = 0;
        int startHeight = 0;
        
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 3; ++j) {
                arrays[i][j] = false;
            }
        }
        
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 3; ++j) {
                Rect region = Rect(Point(startWidth, startHeight), Point(startWidth + squareWidth, startHeight + squareHeight));
                Mat matRegion = mat(region);
                float percent = countNonZero(matRegion == 255) / (float)(squareHeight * squareWidth);
                startWidth += squareWidth;
                arrays[i][j] = (percent > 0.5f);
            }
            startWidth = 0;
            startHeight += squareHeight;
        }
        
        return arrays;
    }
    
    bool areArraysEqual(const array<array<bool, 3>, 2>& input1, const bool input2[2][3]) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (input1[i][j] != input2[i][j]) return false;
            }
        }
        return true;
    }
    
    enum OutputModel::blocksType recogniseShape(array<array<bool, 3>, 2> passedArray, Mat& input) {
        for (int i = 0; i < 4; ++i) {
            if (areArraysEqual(passedArray, L_SHAPE_TEMPLATE[i])) {
                return OutputModel::SHAPE_JL;
            }
        }
        for (int i = 0; i < 2; ++i) {
            if (areArraysEqual(passedArray, T_SHAPE_TEMPLATE[i])) {
                return OutputModel::SHAPE_T;
            }
        }
        for (int i = 0; i < 2; ++i) {
            if (areArraysEqual(passedArray, S_SHAPE_TEMPLATE[i])) {
                return OutputModel::SHAPE_SZ;
            }
        }
        for (int i = 0; i < 1; ++i) {
            if (areArraysEqual(passedArray, I_OR_O_TEMPLATE[i])) {
                float colsByRows = input.cols / (float)input.rows;
                float rowsByCols = input.rows / (float)input.cols;
                float sizeRatio = (colsByRows > rowsByCols) ? colsByRows : rowsByCols;

                if (sizeRatio > 4.0f) {
                    return OutputModel::THRASH;
                } else if (sizeRatio > 2.0f) {
                    return OutputModel::SHAPE_I;
                } else {
                    return OutputModel::SHAPE_O;
                }
            }
        }
        
        return OutputModel::SHAPE_SZ;
    }
    
    Mat getCroppedAndRotatedMat(RotatedRect rect, Mat& source) {
        Mat input = source.clone();
        Mat M, rotated, cropped;
        float angle = rect.angle;
        Size rect_size = rect.size;
        if (rect.angle < -45.) {
            angle += 90.0;
            swap(rect_size.width, rect_size.height);
        }
        M = getRotationMatrix2D(rect.center, angle, 1.0);
        warpAffine(input, rotated, M, input.size(), INTER_CUBIC);
        getRectSubPix(rotated, rect_size, rect.center, cropped);
        if (cropped.rows > cropped.cols) {
            transpose(cropped, cropped);
            flip(cropped, cropped, 1);
        }
        return cropped;
    }
    
    Mat colorRecog(Mat& input) {
        
        Mat toProcess = input.clone();
        cvtColor(toProcess, toProcess, CV_BGR2HSV);
        
        Mat output = Mat(toProcess.rows, toProcess.cols, CV_8U, Scalar(0,0,0));
        array<Scalar, 6> minVals = {YELLOW_MIN, GREEN_MIN, RED_LOWER_MIN, RED_UPPER_MIN, BLUE_MIN};
        array<Scalar, 6> maxVals = {YELLOW_MAX, GREEN_MAX, RED_LOWER_MAX, RED_UPPER_MAX, BLUE_MAX};
        
        for (int i = 0; i < minVals.size(); ++i) {
            Mat colorMat;
            inRange(toProcess, minVals[i], maxVals[i], colorMat);
            addWeighted(output, 1.0, colorMat, 1.0, 0.0, output);
        }
        medianBlur(output, output, 5);
        //imshow("COLOR ANALYSE", output);
        return output;
    }
    
    bool isColorValid(double value, double max, bool debug = false) {
        if (debug) cout<<(value / max * 100)<<"%"<<endl;
        return ((value / max * 100) > MINIMUM_PERCENT);
    }
    
    enum OutputModel::blocksType detectColorShape(Mat orginalCroppedPhoto, Mat processedCroppedPhoto, bool debug) {
        Mat bitWised;
        Mat toAnalyze;
        bitwise_not(processedCroppedPhoto, bitWised);
        Mat in[] = {bitWised, bitWised, bitWised};
        merge(in, 3, bitWised);
        toAnalyze = orginalCroppedPhoto - bitWised;
        
        Mat toProcess = toAnalyze.clone();
        cvtColor(toProcess, toProcess, CV_BGR2HSV);
        
        int numberOfColors = 0;
        enum OutputModel::blocksType foundColor = OutputModel::blocksType::COLOR_MIX;
        
        for (int color = OutputModel::blocksType::COLOR_RED; color < OutputModel::blocksType::COLOR_MIX; ++color) {
            enum OutputModel::blocksType currentColor = static_cast<enum OutputModel::blocksType>(color);
            Mat detectedColorMat = Mat(toProcess.rows, toProcess.cols, CV_8U, Scalar(0,0,0));
            switch (currentColor) {
                case OutputModel::COLOR_BLUE:
                    if (debug) cout<<"blue"<<endl;
                    inRange(toProcess, BLUE_MIN, BLUE_MAX, detectedColorMat);
                    break;
                case OutputModel::COLOR_GREEN:
                    if (debug) cout<<"green"<<endl;
                    inRange(toProcess, GREEN_MIN, GREEN_MAX, detectedColorMat);
                    break;
                case OutputModel::COLOR_WHITE:
                    if (debug) cout<<"white"<<endl;
                    inRange(toProcess, WHITE_MIN, WHITE_MAX, detectedColorMat);
                    break;
                case OutputModel::COLOR_YELLOW:
                    if (debug) cout<<"yellow"<<endl;
                    inRange(toProcess, YELLOW_MIN, YELLOW_MAX, detectedColorMat);
                    break;
                case OutputModel::COLOR_RED:
                    if (debug) cout<<"red"<<endl;
                    Mat tempRedLow, tempRedUpper;
                    inRange(toProcess, RED_LOWER_MIN, RED_LOWER_MAX, tempRedLow);
                    inRange(toProcess, RED_UPPER_MIN, RED_UPPER_MAX, tempRedUpper);
                    addWeighted(tempRedLow, 1.0, tempRedUpper, 1.0, 0.0, detectedColorMat);
                    break;
            }
            
            if (isColorValid(countNonZero(detectedColorMat == 255), countNonZero(processedCroppedPhoto == 255), debug)) {
                ++numberOfColors;
                foundColor = currentColor;
            }
        }
        if (debug) {
            imshow("cropped", orginalCroppedPhoto);
            imshow("final", processedCroppedPhoto);
            imshow("toAnalyze", toAnalyze);
            cout<<"I assume that this picture has "<<numberOfColors<<" different colors."<<endl;
            waitKey();
        }
        
        if (numberOfColors == 1) {
            return foundColor;
        } else if (numberOfColors == 2) {
            return OutputModel::blocksType::COLOR_MIX;
        } else if (numberOfColors > 2) {
            return OutputModel::blocksType::THRASH;
        } else {
            return OutputModel::blocksType::COLOR_WHITE;
        }
    }
}
