#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace std;

const char* camera_yaml_file = "camera.yaml";

int main(int argc, char* argv[])
{
    cv::Mat cameraMatrix = cv::Mat::zeros(3, 3, cv::DataType<float>::type);
    cv::Mat distortionCoeffs;
    cv::FileStorage fs;
    if (fs.open(camera_yaml_file, cv::FileStorage::READ))
    {
        try
        {
            /* code */        
            float width = fs["image_width"];
            float height = fs["image_height"];
            float focus = fs["focus_length"];
            float pixSize = fs["pixel_size"];
            cameraMatrix.at<float>(0, 0) = focus / pixSize;
            cameraMatrix.at<float>(1, 1) = focus / pixSize;
            cameraMatrix.at<float>(0, 2) = width / 2;
            cameraMatrix.at<float>(1, 2) = height / 2;
            cameraMatrix.at<float>(2, 2) = 1.0;
            //fs["distortion_coeffs"] >> distortionCoeffs;
        }
        catch(const cv::Exception& e)
        {
            std::cerr << e.msg << '\n';
            std::cerr << e.func << '\n';
        }
        fs.release();
    }
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_100);
    
    cv::VideoCapture camera;
    if (camera.open(0))
    {
        camera.set(CV_CAP_PROP_FRAME_WIDTH, 800);
        camera.set(CV_CAP_PROP_FRAME_HEIGHT, 600);
        for (bool loop = true; loop;)
        {
            cv::Mat image;
            camera.read(image);

            vector<int> ids;
            vector<vector<cv::Point2f>> corners;
            cv::aruco::detectMarkers(image, dict, corners, ids);
            vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(corners, 10, cameraMatrix, distortionCoeffs, rvecs, tvecs);
            for (int i = 0; i < ids.size(); i++)
            {
                cv::aruco::drawAxis(image, cameraMatrix, distortionCoeffs, rvecs[i], tvecs[i], 10);
            }

            cv::aruco::drawDetectedMarkers(image, corners, ids);

            cv::imshow("camera", image);
            if (cv::waitKey(10) == 'q')
            {
                loop = false;
            }
        }
    }
    return 0;
}