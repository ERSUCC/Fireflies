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

    int hue = 65;
    int value = 90;
    int spread = 15;

    cv::createTrackbar("Hue", window_title, &hue, 360, resetList);
    cv::createTrackbar("Value", window_title, &value, 100, resetList);
    cv::createTrackbar("Spread", window_title, &spread, 100, resetList);

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

        const float kr = fmodf(5 + hue / 60.0f, 6);
        const float kg = fmodf(3 + hue / 60.0f, 6);

        const unsigned int r = (1 - fmaxf(fminf(fminf(kr, 4 - kr), 1), 0)) * (value / 100.0f) * 255;
        const unsigned int g = (1 - fmaxf(fminf(fminf(kg, 4 - kg), 1), 0)) * (value / 100.0f) * 255;

        for (unsigned int i = 0; i < width * height; i++)
        {
            const unsigned int fr = mat.data[i * 3 + 2];
            const unsigned int fg = mat.data[i * 3 + 1];

            if (fr >= r - spread && fr <= r + spread && fg >= g - spread && fg <= g + spread)
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
                std::cout << current->id << std::endl;
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
