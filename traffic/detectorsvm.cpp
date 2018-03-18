#include "detectorsvm.h"


void createSlider(int &val, int max, string title, string wname) {
    title = title + " " + std::to_string(max);
    createTrackbar(title, wname, &val, max, NULL);
}

int val[6];

DetectorSVM::DetectorSVM()
{
    this->model = "/home/ubuntu/Desktop/LaneDetection/train.txt";
//    this->model = "/home/taquy/Projects/python/ctr-crop/TestSVM/data-new/train.txt";
    this->data = "/home/taquy/Projects/python/ctr-crop/TestSVM/data-new/";
    this->sample = "/home/taquy/Projects/python/ctr-crop/TestSVM/data-new/1/81.jpg";

    if (this->isTrain == 0) this->load();


    string signs[] = {
        "Left",
        "Right",
        "Stop",
        "No sign detected"
    };
    this->setLabels(signs);

    // create window for slider testing
    if (this->isDebug) {
        std::fill(val, val + 6, 0);

//        namedWindow("mask", CV_WINDOW_AUTOSIZE);
//        createSlider(val[0], 30, "Opening", "mask");
//        createSlider(val[1], 30, "Closing", "mask");
    }
}

void DetectorSVM::setLabels(string *labels) {
    for (int i = 0; i < labels->size(); i++) {
        this->labels.push_back(labels[i]);
    }
}

void DetectorSVM::load() {
    if (this->isTrain == 0) {
        try {
            svm = SVM::create();
            svm = SVM::load(this->model);
        } catch (const std::exception& e) {
            cout << "Load model failed." << endl;
        }
    }
}


void DetectorSVM::filter(Mat &hsv, Mat &mask){
    inRange(hsv, Scalar(60, 105, 83), Scalar(255, 255, 255), mask);

    if (!this->isDebug) {
        Mat k(0, 0, CV_8U, Scalar(1));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, k);

        Mat m(0, 0, CV_8U, Scalar(1));
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, m);

        medianBlur(mask, mask, 3);

    } else {
        Mat k(val[0], val[0], CV_8U, Scalar(1));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, k);

        Mat m(val[1], val[1], CV_8U, Scalar(1));
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, m);

        medianBlur(mask, mask, 3);

        imshow("mask", mask);
    }
}

Rect DetectorSVM::pooling(Mat &mask, Mat &out, Mat &gray){

    vector<vector<Point>> contours;
    vector<Rect> boundRect(contours.size());
    vector<Vec4i> hierarchy;

    findContours(mask, contours, hierarchy, RETR_TREE , CHAIN_APPROX_SIMPLE, Point(0, 0) );

    if (contours.size() <= 0) return Rect();

    vector<vector<Point> > contours_poly(contours.size());
    vector<Point2f>center(contours.size());
    vector<float> radius(contours.size());

    int largest_area = 0;
    int largest_contour = 0;


    // get largest contour
    Rect rect;

    for(unsigned int i = 0; i< contours.size(); i++) {
        double a = contourArea( contours[i],false);
        if( a > largest_area){
            largest_area = a;
            largest_contour = i;
            rect = boundingRect(contours[i]);
        }
    }

    if(rect.area() < mask.size().height * mask.size().width) {

        if (this->isDebug) {
            cv::rectangle(mask, rect, Scalar(255, 255, 255), 1, 8, 0);
            cv::rectangle(gray, rect, Scalar(255, 255, 255), 1, 8, 0);

            imshow("gray", gray);
        }

        gray(rect).copyTo(out);
        return rect;
    }


    for(unsigned int i = 0; i< contours.size(); i++) {
        double a = contourArea( contours[i],false);
        if( a > largest_area){
            largest_area = a;
            largest_contour = i;
            rect = boundingRect(contours[i]);
        }
    }



}


int DetectorSVM::predict(Mat &test) {

      resize(test,test,Size(64,48));
      HOGDescriptor hog( Size(64,48),Size(8,8), Size(4,4), Size(4,4), 9);
      vector<float> size_;
      hog.compute(test,size_);
      int col= size_.size();
      Mat testMat(1, col, CV_32FC1);
      vector<float> des;
      hog.compute(test,des);
      for (int i = 0; i < col; i++)
      testMat.at<float>(0,i)= des[i];

      return this->svm->predict(testMat);
}

string DetectorSVM::label(int &id) {
    if (id < 0 || id >= this->labels.size())
        id = this->labels.size() - 1;
    return this->labels[id];
}

int DetectorSVM::train(){

    if (this->isTrain == 0) return -1;

    int m = 0;
    int numfiles;
    Mat img, img0;
    vector<string> files;
    vector<string> folders;

    string f = this->data;

    img0 = imread(this->sample, 1);

    if(img0.empty()){
        cout << "Failed open image 0" << endl;
        return -1;
    }

    Mat re;

    resize(img0, re, Size(64,48));

    vector<float> size_;

    HOGDescriptor hog( Size(64,48), Size(8,8), Size(4,4), Size(4,4), 9);

    hog.compute(re, size_);

    this->lsdirs(f, folders);

    for(string s: folders) cout << s << endl;

    int num_imgs = 200;
    numfiles = num_imgs  * folders.size();
    Mat labels( numfiles, 1, CV_32S);
    Mat trainMat( numfiles, size_.size(), CV_32FC1);

    for(unsigned int x = 0; x < folders.size(); x++ ){
    this->lsfiles(folders[x], files);
    cout << folders[x] << endl;

    for(unsigned int y = 0; y< num_imgs; y++){
        cout << files[y] << endl;

        img = imread(files[y], 0);
        resize(img, img, Size(64,48));

        vector<float> des;
        hog.compute(img,des);
        for (unsigned int i = 0; i < size_.size(); i++)
            trainMat.at<float>(m,i)= des[i];
            labels.at<int>(m, 0) = x;
            m++;
        }
    }

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, 1e-6));
    svm->train(trainMat, ROW_SAMPLE, labels);
    svm->save(f + "train.txt");
    cout << "FINISHED" << endl;

}


int DetectorSVM::detect(Mat &img) {
    Mat gray ,mask , hsv, out;

    cvtColor(img, gray, COLOR_BGR2GRAY);
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // pre-processing
    this->filter(hsv, mask);

    // detect object
    Rect box = this->pooling(mask, out, gray);

    int st, end, frame = 0;

    if(!out.empty()){

        // classification
        int id = predict(out);

        if (this->isDebug) {

            // find circle
            vector<Vec3f> circles;

            cv::resize(out, out, Size(60, 60));

            HoughCircles(out, circles, HOUGH_GRADIENT, 1, 10, 100, 30, 30, 30);


            for( size_t i = 0; i < circles.size(); i++ )
            {
                Vec3i c = circles[i];
                circle( out, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
            }


            vector<Rect> boxes;
            boxes.push_back(box);

            string lbl = this->label(id);
            cout << this->label(id) << endl;

            Mat rsl;
//            rsl = out;
            cv::resize(out, rsl, Size(out.cols * 2, out.rows * 2));


            if (id != this->labels.size() - 1) {
                img = this->draw(img, boxes, lbl);

                imshow("region", rsl);
            } else {
                imshow("region", rsl);
            }

        }

        return id;
    }

    return -1;
}

Mat DetectorSVM::draw(Mat frame, vector<Rect> boxes, String label) {

    // draw rects
    for( size_t i = 0; i < boxes.size(); i++ )
    {
        int x = boxes[i].x;
        int y = boxes[i].y;
        Point a(x, y);
        Point b(x + boxes[i].width, y + boxes[i].height);

        rectangle(frame, a, b, Scalar(0, 255, 0), 3);

        putText(frame, label, a, FONT_HERSHEY_SIMPLEX, 1, (0, 125, 255), 3, 0, false);
    }
    return frame;
}

void DetectorSVM::lsdirs(string path, vector<string> &folders){
    folders.clear();
    struct dirent *entry;

    DIR *dir = opendir(path.c_str());

    while((entry = readdir(dir))!= NULL){
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
            string s =  string(path)  + string(entry->d_name) ;
            folders.push_back(s);
        }
    }

    closedir(dir);
    sort(folders.begin(),folders.end());
}


void DetectorSVM::lsfiles(string path, vector<string> &files) {

    files.clear();

    struct dirent *entry;

    DIR *dir = opendir(path.c_str());

    while((entry = readdir(dir))!= NULL){
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
            string s =  string(path) + "/" + string(entry->d_name) ;
            files.push_back(s);
        }
    }

    closedir(dir);
    sort(files.begin(),files.end());
}
