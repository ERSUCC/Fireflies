#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "list.h"

list* fireflies = list_allocate();

void resetList(const int value, void* data)
{
    list_free(fireflies);

    fireflies = list_allocate();
}

int main(int argc, char** argv)
{
    const cv::String window_title("Preview");

    cv::namedWindow(window_title);
    cv::moveWindow(window_title, 100, 100);

    int min_hue = 59;
    int max_hue = 71;
    int min_sat = 19;
    int max_sat = 100;
    int min_val = 60;
    int max_val = 100;

    cv::createTrackbar("Min Hue", window_title, &min_hue, 360, resetList);
    cv::createTrackbar("Max Hue", window_title, &max_hue, 360, resetList);
    cv::createTrackbar("Min Sat", window_title, &min_sat, 100, resetList);
    cv::createTrackbar("Max Sat", window_title, &max_sat, 100, resetList);
    cv::createTrackbar("Min Val", window_title, &min_val, 100, resetList);
    cv::createTrackbar("Max Val", window_title, &max_val, 100, resetList);

    cv::VideoCapture cap("C0004_cutM.MP4");

    const unsigned int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const unsigned int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    const unsigned int memory = 300000;
    const unsigned int max_rad_sq = 2500;

    unsigned int frame_num = 0;

    cv::Mat mat;

    while (1)
    {
        list_node* node = fireflies->head;

        while (node)
        {
            if (frame_num - node->frame > memory)
            {
                list_remove(fireflies, node);
            }

            node = node->next;
        }

        if (!cap.read(mat))
        {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);

            frame_num = 0;

            list_free(fireflies);

            fireflies = list_allocate();

            continue;
        }

        unsigned int min_r;
        unsigned int max_r;
        unsigned int min_g;
        unsigned int max_g;
        unsigned int min_b;
        unsigned int max_b;

        const float min_c = (min_val / 100.0f) * (min_sat / 100.0f);
        const float max_c = (max_val / 100.0f) * (max_sat / 100.0f);

        const float min_x = min_c * (1 - fabsf(fmodf(min_hue / 60.0f, 2) - 1));
        const float max_x = max_c * (1 - fabsf(fmodf(max_hue / 60.0f, 2) - 1));

        const float min_m = (min_val / 100.0f) - min_c;
        const float max_m = (max_val / 100.0f) - max_c;

        if (min_hue < 60)
        {
            min_r = (min_c + min_m) * 255;
            min_g = (min_x + min_m) * 255;
            min_b = min_m * 255;
        }

        else if (min_hue < 120)
        {
            min_r = (min_x + min_m) * 255;
            min_g = (min_c + min_m) * 255;
            min_b = min_m * 255;
        }

        else if (min_hue < 180)
        {
            min_r = min_m * 255;
            min_g = (min_c + min_m) * 255;
            min_b = (min_x + min_m) * 255;
        }

        else if (min_hue < 240)
        {
            min_r = min_m * 255;
            min_g = (min_x + min_m) * 255;
            min_b = (min_c + min_m) * 255;
        }

        else if (min_hue < 300)
        {
            min_r = (min_x + min_m) * 255;
            min_g = min_m * 255;
            min_b = (min_c + min_m) * 255;
        }

        else
        {
            min_r = (min_c + min_m) * 255;
            min_g = min_m * 255;
            min_b = (min_x + min_m) * 255;
        }

        if (max_hue < 60)
        {
            max_r = (max_c + max_m) * 255;
            max_g = (max_x + max_m) * 255;
            max_b = max_m * 255;
        }

        else if (max_hue < 120)
        {
            max_r = (max_x + max_m) * 255;
            max_g = (max_c + max_m) * 255;
            max_b = max_m * 255;
        }

        else if (max_hue < 180)
        {
            max_r = max_m * 255;
            max_g = (max_c + max_m) * 255;
            max_b = (max_x + max_m) * 255;
        }

        else if (max_hue < 240)
        {
            max_r = max_m * 255;
            max_g = (max_x + max_m) * 255;
            max_b = (max_c + max_m) * 255;
        }

        else if (max_hue < 300)
        {
            max_r = (max_x + max_m) * 255;
            max_g = max_m * 255;
            max_b = (max_c + max_m) * 255;
        }

        else
        {
            max_r = (max_c + max_m) * 255;
            max_g = max_m * 255;
            max_b = (max_x + max_m) * 255;
        }

        for (unsigned int i = 0; i < width * height; i++)
        {
            const unsigned int r = mat.data[i * 3 + 2];
            const unsigned int g = mat.data[i * 3 + 1];
            const unsigned int b = mat.data[i * 3];

            if (r >= min_r && r <= max_r && g >= min_g && g <= max_g && b >= min_b && b <= max_b)
            {
                const int x = i % width;
                const int y = i / width;

                bool found = false;

                list_node* node = fireflies->head;

                while (node)
                {
                    if ((node->ax - x) * (node->ax - x) + (node->ay - y) * (node->ay - y) <= max_rad_sq)
                    {
                        node->ax = (node->ax * node->point_count + x) / (node->point_count + 1);
                        node->ay = (node->ay * node->point_count + y) / (node->point_count + 1);
                        node->point_count++;

                        found = true;

                        break;
                    }

                    node = node->next;
                }

                if (!found)
                {
                    list_add(fireflies, x, y, frame_num);
                }
            }
        }

        const float xscale = 960.0 / width;
        const float yscale = 540.0 / height;

        cv::resize(mat, mat, cv::Size(960, 540));

        list_node* current = fireflies->head;

        while (current)
        {
            cv::rectangle(mat, cv::Rect(current->ax * xscale - 7, current->ay * yscale - 7, 14, 14), cv::Scalar(0, 0, 255, 255));
            cv::putText(mat, std::to_string(current->id), cv::Point(current->ax * xscale + 7, current->ay * yscale + 7), cv::FONT_HERSHEY_DUPLEX, 0.3, cv::Scalar(0, 0, 255, 255));

            if (current->id < 10)
            {
                std::cout << current->id << "\n";
            }

            current = current->next;
        }

        cv::putText(mat, std::to_string(frame_num), cv::Point(5, 15), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255, 255));

        cv::imshow(window_title, mat);
        cv::waitKey(17);

        if (cv::getWindowProperty(window_title, cv::WND_PROP_VISIBLE) < 1)
        {
            break;
        }

        frame_num++;
    }

    list_free(fireflies);

    return 0;
}
