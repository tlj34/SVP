#include <opencv2/opencv.hpp>
#include <iostream>
#undef max

int main() {
	//定义变量并设置默认值
	int width = 120, height = 90, eps = 4;
	double outfps = -2.0;
	char key[16] = "", value[8] = "";

	//打开设置文件与输出文件
	FILE* in = fopen("setting.txt", "r");
	FILE* out = fopen("output.txt", "w");
	if (in == NULL || out == NULL) {
		perror("ERROR");
		system("pause");
		return 1;
	}

	//读取设置文件
	while (fscanf(in, " %15[^=]=%7s\n", key, value) != EOF) {
		int ret;
		if (!strcmp(key, "width")) ret = sscanf(value, "%d", &width);
		else if (!strcmp(key, "height")) ret = sscanf(value, "%d", &height);
		else if (!strcmp(key, "eps")) ret = sscanf(value, "%d", &eps);
		else if (!strcmp(key, "fps")) ret = sscanf(value, "%lf", &outfps);
		else {
			fprintf(stderr, "Warning: Unrecognized key %s\n", key);
			continue;
		}
		if (ret != 1) {
			fprintf(stderr, "Error: Invalid value for %s\n", key);
			system("pause");
			return 1;
		}
	}

	//获取视频名称
	std::string videoName;
	cv::VideoCapture video;
	getline(std::cin, videoName);

	//去除视频名称两端的双引号
	if (videoName[0] == '\"') {
		videoName.erase(videoName.end() - 1);
		videoName.erase(videoName.begin());
	}

	//获取视频文件
	if (!video.open(videoName)) {
		fprintf(stderr, "ERROR: Unable to open video\n");
		system("pause");
		return 1;
	}

	//定义并初始化变量
	cv::Mat tmp;
	cv::Vec3b color, bgr;
	int totalFrames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT)), cnt;
	double interval = outfps > 0 ? video.get(cv::CAP_PROP_FPS) / outfps : -outfps;

	//此lambda表达式用于处理并输出信息
	auto putinfo = [&cnt, &color, &out]() -> void {
		fprintf(out, "%02x%02x%02x", color[2], color[1], color[0]);
		if (cnt < 2) return;
		std::string num = std::to_string(cnt);
		for (char ch : num) fprintf(out, "%c", ch - '0' + 'G');
		};

	puts("loading...");

	//处理视频并输出
	for (int n = 0, m = 0; ; ++n) {
		video.read(tmp);
		if (tmp.empty()) break;
		else if (static_cast<int>(m * interval) ^ n) continue;

		cv::resize(tmp, tmp, { width, height });
		++m, cnt = 0, color = tmp.at<cv::Vec3b>(0, 0);

		for (int i = 0; i < tmp.rows; ++i) {
			for (int j = 0; j < tmp.cols; ++j) {
				bgr = tmp.at<cv::Vec3b>(i, j);
				if (std::max({ abs(bgr[0] - color[0]), abs(bgr[1] - color[1]), abs(bgr[2] - color[2]) }) > eps)
					putinfo(), cnt = 1, color = bgr;
				else ++cnt;
			}
		}

		putinfo();
		fprintf(out, "\n");

		printf("\033[1G%02.2lf%%", 100.0 * n / totalFrames);
	}

	//释放内存
	video.release();
	fclose(in);
	fclose(out);

	perror("\n");
	system("pause");
	return 0;
}