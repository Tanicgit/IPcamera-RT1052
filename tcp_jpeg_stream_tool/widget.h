#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QImage>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
#pragma pack(1)
    /*head[1] ver[1] re[1] cmd[1] payloadlen[4] payload[n] check[2]*/
    typedef struct{
        uint8_t head;
        uint8_t version;
        uint8_t err;//0 ok 1err
        uint8_t cmd;//0 为实现   1获取jpeg数据 payload[0] 几幅图像
        uint32_t  payload_size;
    }tcp_jpeg_stream_head;
#pragma pack()

private slots:
    void on_pushButton_clicked();
    void readData();
    void readError(QAbstractSocket::SocketError);
    void timerout();
    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QTimer *timer;
    QTcpSocket *tcpClient;
    QImage  img;
    bool frame_ok;
    bool start;
    uint8_t num;
    QByteArray jpeg;
    uint32_t jpeg_size;
    uint32_t jpeg_rev_size;
};

#endif // WIDGET_H
