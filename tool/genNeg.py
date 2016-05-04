import os
import cv2
import errno


class Generate:
    def __init__(self, input, output):
        self.input = input
        self.output = output
        self.array_input = []
        self.files = []

    def load_dir(self):
        for cur, dirs, files in os.walk(self.input):
            self.array_input.append(files)

    def split_file(self):
        cnt = 0
        for item in self.array_input:
            for file in item:
                cnt += 1
                if (cnt % 20) == 1:
                    self.files.append(file)
        self.array_input = []

    def make_image(self):
        cnt = 0
        for item in self.files:
            self.cut_image(self.input + "/" + item, cnt)
            cnt += 1

    def prepare_directory(self, path):
        try:
            os.makedirs(path)
        except OSError as exception:
            if exception.errno != errno.EEXIST:
                raise

    def cut_image(self, file, cnt):
        # Load image
        img = cv2.imread(file, 1)

        # Make  output directory
        self.prepare_directory(self.output + str(cnt))

        res_img = cv2.resize(img, (480, 640), interpolation=cv2.INTER_AREA)

        (winW, winH) = (75, 75)

        i = 0
        for (x, y, window) in self.sliding_window(res_img, stepSize=75, windowSize=(winW, winH)):
            if window.shape[0] != winH or window.shape[1] != winW:
                continue

            i += 1
            output = self.output + str(cnt) + '/' + str(cnt) + "_" + str(i) + ".ppm"
            cv2.imwrite(output, window)

    def sliding_window(self, image, stepSize, windowSize):
        # slide a window across the image
        for y in xrange(0, image.shape[0], stepSize):
            for x in xrange(0, image.shape[1], stepSize):
                # yield the current window
                yield (x, y, image[y:y + windowSize[1], x:x + windowSize[0]])


def main():
    point = Generate("/run/media/maiikeru/HDD_DATA/Background/NonTSImages/TrainingBG", "/tmp/N/")
    point.load_dir()
    point.split_file()
    point.make_image()


if __name__ == '__main__':
    main()
