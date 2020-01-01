#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerout()));

    start = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="connect")
    {
        tcpClient->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->pushButton->setText("disconnect");
        }
    }
    else
    {
       tcpClient->close();
       ui->pushButton->setText("connect");
    }
}



void Widget::readData()
{
   QByteArray data;
   QMatrix matrix;
   matrix.rotate(180);

   tcp_jpeg_stream_head *msg;
   uint8_t buf[sizeof(tcp_jpeg_stream_head)];
   data = tcpClient->readAll();

   if(frame_ok)
   {
       frame_ok = false;
       for(uint8_t i=0;i<sizeof(tcp_jpeg_stream_head);i++)
       {
         buf[i] = (uint8_t)data.at(i);
       }
       msg = (tcp_jpeg_stream_head*)buf;
       if(msg->head==0xaa&&msg->version==1)
       {
           if(msg->err==0)
           {
               if(msg->cmd==0x81)
               {
                  jpeg_size = (uint8_t)data.at(4) | (uint8_t)data.at(5)<<8 | (uint8_t)data.at(6)<<16 | (uint8_t)data.at(7)<<24;
                  if(jpeg_size==data.length()-sizeof(tcp_jpeg_stream_head)-2)
                  {
                    jpeg = data.mid(8,jpeg_size);
                    img.loadFromData(jpeg);
                    ui->label->setPixmap(QPixmap::fromImage(img).transformed(matrix, Qt::SmoothTransformation));
                    frame_ok = true;

           //         ui->pushButton_2->setText("start");



                    jpeg.clear();
                    jpeg_size=0;
                    jpeg_rev_size=0;
                    num--;
                    if(start&&num==0)timer->start(10);

                  }
                  else
                  {
                      jpeg_rev_size = data.length()-8;
                      jpeg = data.mid(8,jpeg_rev_size);

                      qDebug()<< "jpeg[0]:"<<jpeg.at(0);
                      qDebug()<< "jpeg_size:"<<jpeg_size;
                      qDebug()<< "jpeg_rev_size:"<<jpeg_rev_size;
                  }

               }
               else
               {
                    ui->pushButton_2->setText("start");
                    start = false;
                    tcpClient->close();
                    ui->pushButton->setText("connect");
               }

           }
           else
           {
                ui->pushButton_2->setText("start");
                tcpClient->close();
                start = false;
                ui->pushButton->setText("connect");
           }
       }
   }
   else
   {
       if(data.length()+jpeg_rev_size<=jpeg_size)
       {
            jpeg.append(data);
            jpeg_rev_size +=data.length();
            qDebug()<< "jpeg_rev_size:"<<jpeg_rev_size;
       }
       else
       {
           QByteArray temp;
           temp = data.mid(0,jpeg_size-jpeg_rev_size);
           jpeg_rev_size += data.length();
           qDebug()<< "jpeg_rev_size:"<<jpeg_rev_size;
           jpeg.append(temp);
           img.loadFromData(jpeg);
           ui->label->setPixmap(QPixmap::fromImage(img).transformed(matrix, Qt::SmoothTransformation));
           frame_ok = true;


//               QFile file("1.jpg");
//               if (!file.open(QIODevice::WriteOnly))
//               {
//                   QMessageBox::warning(nullptr, "Err", "Cannot open file for reading!");
//                   return ;
//               }
//               file.write(jpeg);
//               file.close();




//           ui->pushButton_2->setText("start");
           jpeg.clear();
           jpeg_size=0;
           jpeg_rev_size=0;

           num--;
           if(start&&num==0)timer->start(20);

       }
   }





}

void Widget::readError(QAbstractSocket::SocketError)
{

}

void Widget::timerout()
{
    tcp_jpeg_stream_head *msg;
    uint8_t data[sizeof(tcp_jpeg_stream_head)+1+2];
    msg = (tcp_jpeg_stream_head *)data;
    msg->head = 0xaa;
    msg->version = 0x01;
    msg->err = 0;
    msg->cmd = 1;
    msg->payload_size = 1;
    data[sizeof(tcp_jpeg_stream_head)]=1;
    //crc
    data[sizeof(tcp_jpeg_stream_head)+1]=0x11;
    data[sizeof(tcp_jpeg_stream_head)+2]=0x22;

    tcpClient->write((const char*)data,sizeof(tcp_jpeg_stream_head)+1+2);

    num = 1;
}

void Widget::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text()=="start")
    {
        timer->start(20);
        frame_ok = true;
        ui->pushButton_2->setText("stop");
        start = true;
    }
    else
    {
        timer->stop();
        ui->pushButton_2->setText("start");
        start = false;
    }
}

