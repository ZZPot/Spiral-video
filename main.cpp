#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

#define FILE_NAME		"original.wmv"
#define SPIRAL_NAME		"spiral_video.avi"
#define WND_NAME_ORIG	"Original"
#define WND_NAME_RES	"Result"

void CopyRows(cv::Mat src, cv::Mat dst, unsigned row_start, unsigned count = 1);

int main()
{
	cv::VideoCapture video_cap;
	video_cap.open(FILE_NAME);
	if(!video_cap.isOpened())
		return 1;
	cv::VideoWriter writer;
	cv::Mat frame;
	std::vector<cv::Mat> tail;
	cv::Mat spiral_frame;
	int tail_count = 50; // -1 means frame.rows
	unsigned frame_count = 0;
	while(video_cap.read(frame))
	{
		if (!writer.isOpened())
			writer.open(SPIRAL_NAME, cv::VideoWriter::fourcc('X','V','I','D'), 30, frame.size());
		if(!frame_count)
		{
			if(tail_count <= 0)
				tail_count = frame.rows;
			tail.reserve(tail_count);
			frame.copyTo(spiral_frame);
		}
		tail.push_back(frame.clone());
		if(tail.size() >= tail_count)
		{
			tail.erase(tail.begin(), tail.begin() + (tail.size() - tail_count));
		}
		unsigned line_per_frame = frame.rows / tail_count;
		for(unsigned i = 0; i < tail.size(); i++)
		{

			CopyRows(tail[i], spiral_frame, line_per_frame*i, line_per_frame);
		}
		cv::imshow(WND_NAME_ORIG, frame);
		cv::imshow(WND_NAME_RES, spiral_frame);
		if(cv::waitKey(20) == 27)
			break;
		frame_count++;
		writer << spiral_frame;
	}
	return 0;
}
void CopyRows(cv::Mat src, cv::Mat dst, unsigned row_start, unsigned count)
{
	cv::Mat rows_mask = cv::Mat::zeros(src.size(), CV_8UC1);
	if(row_start >= rows_mask.rows)
		return;
	if(row_start + count >= rows_mask.rows)
		count = rows_mask.rows - row_start - 1;
	cv::Mat rows = rows_mask(cv::Rect(0, row_start, rows_mask.cols, count));
	rows.setTo(cv::Scalar(255));
	src.copyTo(dst, rows_mask);
}