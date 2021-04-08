#include <iostream>
#include "drtface.h"
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// void cvMat2tfTensor(cv::Mat input, float inputMean, float inputSTD, tensorflow::Tensor& outputTensor)
// 	{
// 		auto outputTensorMapped = outputTensor.tensor<float, 4>();
 
// 		input.convertTo(input, CV_32FC3);
// 		cv::resize(input, input, cv::Size(inputWidth, inputHeight));
 
// 		input = input - inputMean;
// 		input = input / inputSTD;
 
// 		int height = input.size().height;
// 		int width = input.size().width;
// 		int depth = input.channels();
 
// 		const float* data = (float*)input.data;
// 		for (int y = 0; y < height; ++y)
// 		{
// 			const float* dataRow = data + (y * width * depth);
// 			for (int x = 0; x < width; ++x)
// 			{
// 				const float* dataPixel = dataRow + (x * depth);
// 				for (int c = 0; c < depth; ++c)
// 				{
// 					const float* dataValue = dataPixel + c;
// 					outputTensorMapped(0, y, x, c) = *dataValue;
// 				}
// 			}
// 		}
// 	}

int main(int argc, char** argv) {
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0)
    {   //child process
        execlp("/usr/bin/python3","python3", "../../p_test/test2.py", NULL); //execute the command
        wait(nullptr);
    }
    else
    {
        std::cout << "Now in parent process. you can continue with something here" << std::endl;
        // exit(EXIT_SUCCESS);
    }

    return 0;
}