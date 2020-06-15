#include <QtGui>
#include <QTimer>

#include <boost/foreach.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <QTimer>

#include "ImageWidget.h"
#include "Operator.h"


 AlgorithmWindow::AlgorithmWindow(bool drawDirectly):
 myPenWidth(glo_scale),
 myPenColor(Qt::green),
 drawDirectly(drawDirectly)
 {
//     QTimer::singleShot(0, this, SLOT(mainLoopIsRunning()));
     m_timerId = startTimer(10);
 }

 AlgorithmWindow::~AlgorithmWindow()
{
    iterrupt();
    boost::interprocess::scoped_lock<boost::mutex> lock(m_mutex);
}

 bool AlgorithmWindow::openImage(const QString &fileName)
 {
     m_fileName = fileName.toStdString();
     bool ret = image.load(fileName);
     image.convertToFormat(QImage::Format_RGB32);
     setFixedSize(image.size());
     update();
     return ret;
 }

 bool AlgorithmWindow::saveImage(const QString &fileName)
 {
     bool ret = image.save(fileName, "JPEG");
     if(ret == false)
     {
         std::cerr<<"image saving error"<<std::endl;
     }
     return ret;
 }

 void AlgorithmWindow::setPenColor(const QColor &newColor)
 {
     myPenColor = newColor;
 }

 void AlgorithmWindow::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     QRect dirtyRect = event->rect();
     painter.drawImage(dirtyRect, image, dirtyRect);
 }

 void AlgorithmWindow::resizeEvent(QResizeEvent *event)
 {
     if (width() > image.width() || height() > image.height()) {
         int newWidth = qMax(width() + 128, image.width());
         int newHeight = qMax(height() + 128, image.height());
         resizeImage(&image, QSize(newWidth, newHeight));
         update();
     }
     QWidget::resizeEvent(event);
 }

 void AlgorithmWindow::timerEvent(QTimerEvent* event)
 {
     if (event->timerId() == m_timerId)
     {
         QPainter painter(&image);
          painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                           Qt::RoundJoin));
         m_mutex.lock();
         std::vector<QPoint> tempVect = m_pointsToDraw;
         m_pointsToDraw.clear();
         m_mutex.unlock();
         BOOST_FOREACH(const QPoint& point, tempVect)
         {
             painter.drawPoint(point);
         }
         update();
     }
     else
     {
         QWidget::timerEvent(event);
     }
 }

 void AlgorithmWindow::drawPoint(const QPoint &point)
 {
     boost::interprocess::scoped_lock<boost::mutex> lock(m_mutex);
     m_pointsToDraw.push_back(point);
 }

 void AlgorithmWindow::drawPointDirectly(const QPoint &point)
 {
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    painter.drawPoint(point);
 }

 void AlgorithmWindow::resizeImage(QImage *image, const QSize &newSize)
 {
     if (image->size() == newSize)
         return;

     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
     QPainter painter(&newImage);
     painter.drawImage(QPoint(0, 0), *image);
     *image = newImage;
 }

#if WAF
#include "ImageWidget.moc"
#endif
