#include "image_mask.h"
#include "utils.h"

#include <QPainter>

ImageMask::ImageMask() {}
ImageMask::ImageMask(const QString &file, Id2Labels id_labels) {
    cv::Mat col_img = cv::imread(file.toLocal8Bit().toStdString());

    cv::Mat t_id;
    col_img.copyTo(t_id);

    for (auto label : id_labels.values() ) {
        cv::Mat mask;
        cv::Scalar col(label->color.blue(), label->color.green(), label->color.red());
        cv::inRange(col_img, col, col, mask);
        t_id.setTo(cv::Scalar(label->id, label->id, label->id), mask);
    }

    id = QImage((const uchar *)t_id.data, t_id.cols, t_id.rows, int(t_id.step), QImage::Format_RGB888);

    color = idToColor(id, id_labels);
}
ImageMask::ImageMask(QSize s) {
    id = QImage(s, QImage::Format_RGB888);
    color = QImage(s, QImage::Format_RGB888);
    id.fill(QColor(0, 0, 0));
    color.fill(QColor(0, 0, 0));
}

void ImageMask::drawFillCircle(int x, int y, int pen_size, ColorMask cm) {
    QPen pen(QBrush(cm.id), 1.0);
    QPainter painter_id(&id);
    painter_id.setRenderHint(QPainter::Antialiasing, false);
    painter_id.setPen(pen);
    painter_id.setBrush(QBrush(cm.id));
    painter_id.drawEllipse(x, y, pen_size, pen_size);
    painter_id.end();

    QPainter painter_color(&color);
    QPen pen_color(QBrush(cm.color), 1.0);
    painter_color.setRenderHint(QPainter::Antialiasing, false);
    painter_color.setPen(pen_color);
    painter_color.setBrush(QBrush(cm.color));
    painter_color.drawEllipse(x, y, pen_size, pen_size);
    painter_color.end();
}

void ImageMask::drawPixel(int x, int y, ColorMask cm) {
    id.setPixelColor(x, y, cm.id);
    color.setPixelColor(x, y, cm.color);
}

void ImageMask::updateColor(const Id2Labels & labels) {
    idToColor(id, labels, &color);
}
