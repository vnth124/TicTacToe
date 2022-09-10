#include "tictactoemainwindow.h"
#include "ui_tictactoemainwindow.h"

#include <QtGui>
#include <QPalette>
#include<QMessageBox>

const int DEFAULT_X_OFFSET= 100;
const int DEFAULT_Y_OFFSET= 100;
const int DEFAULT_WIDTH  =  100;
const int DEFAULT_HEIGHT =  100;
int ColumnIndex=1;
int RowIndex=1;
int status[10][10];

TicTacToeMainWindow::TicTacToeMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TicTacToeMainWindow)
{

    ui->setupUi(this);
    Initialize();

    m_pClientSocket = new QTcpSocket(this);

}

TicTacToeMainWindow::~TicTacToeMainWindow()
{
    delete ui;
}

void TicTacToeMainWindow::paintEvent(QPaintEvent *pEvent)
{
   // int X=DEFAULT_X_OFFSET;
   // int Y=DEFAULT_Y_OFFSET;

    QWidget::paintEvent(pEvent);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);


 painter.setFont(QFont("Italic",20) );
    painter.setPen(Qt::black);

    for (int RowIndex = 0; RowIndex<m_NoOfRows; RowIndex++)
    {
        for (int ColumnIndex = 0; ColumnIndex<m_NoOfCols; ColumnIndex++)
        {
            painter.drawRect(m_CellArray[RowIndex][ColumnIndex].m_CellRect);

            if(m_CellArray[RowIndex][ColumnIndex].m_strText!="i")
            {
                painter.drawText(m_CellArray[RowIndex][ColumnIndex].m_CellRect.left()-m_Height/2,
                                m_CellArray[RowIndex][ColumnIndex].m_CellRect.top()-m_width/2,
                                m_CellArray[RowIndex][ColumnIndex].m_strText);


            }

        }
    }

}
void TicTacToeMainWindow::Initialize()
{
    m_NoOfRows=DEFAULT_NO_ROWS;
    m_NoOfCols=DEFAULT_NO_COLS;
    m_width=DEFAULT_WIDTH;
    m_Height=DEFAULT_HEIGHT;
    m_XOffset=DEFAULT_X_OFFSET;
    m_YOffset=DEFAULT_Y_OFFSET;
    //Initialize the Rectange

   for (int RowIndex = 0; RowIndex<m_NoOfRows; RowIndex++)
   {
       for (int ColumnIndex = 0; ColumnIndex<m_NoOfCols; ColumnIndex++)
       {
            m_CellArray[RowIndex][ColumnIndex].m_CellRect.setSize(QSize(m_width, m_Height));
            m_CellArray[RowIndex][ColumnIndex].m_CellRect.setTopLeft(QPoint(m_XOffset + (m_width*(ColumnIndex+1)), m_YOffset + (m_Height*(RowIndex+1))));

       }
   }
}

QString TicTacToeMainWindow::CreateMessage()
{
    m_Message="";
    for(int i=0;i<DEFAULT_NO_ROWS;i++)
    {
        for(int j=0;j<DEFAULT_NO_COLS;j++)
        {
            m_Message+=m_CellArray[i][j].m_strText;

        }
    }
    return m_Message;

}
void TicTacToeMainWindow::DisplayStatusMessage(QString Message)
{
  ui->labelResult->setText(Message);
}


void TicTacToeMainWindow::SendMovesToRemotePC(QString Message)
{

    //This is a simple protocol message ie Just storing only cell moves
    //User might need to expand the protocol struct
    //to accomodate other settings paramaters eg Message="GridSize"+"#"+"X or Y"+"#"+Message;
    //based on the requirements

m_pClientSocket->write(QString(Message + "\n").toUtf8());


}

void TicTacToeMainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
            case QAbstractSocket::RemoteHostClosedError:
                break;
            case QAbstractSocket::HostNotFoundError:
                QMessageBox::information(this, tr("Fortune TicTacToeMainWindow"),
                                         tr("The host was not found. Please check the "
                                            "host name and port settings."));
                break;
            case QAbstractSocket::ConnectionRefusedError:
                QMessageBox::information(this, tr("Fortune TicTacToeMainWindow"),
                                         tr("The connection was refused by the peer. "
                                            "Make sure the fortune server is running, "
                                            "and check that the host name and port "
                                            "settings are correct."));
                break;
            default:
                QMessageBox::information(this, tr("Fortune TicTacToeMainWindow"),
                                         tr("The following error occurred: %1.")
                                         .arg(m_pClientSocket->errorString()));
    }
}
void TicTacToeMainWindow::reset()
{
    for(int i=0;i<DEFAULT_NO_ROWS;i++)
        {
            for(int j=0;j<DEFAULT_NO_COLS;j++)
            {
                m_CellArray[i][j].m_strText="";

                this->repaint();
            }

        }
}


void TicTacToeMainWindow::wincheck()

{	if ((m_CellArray[0][0].m_strText == 'X' && m_CellArray[0][1].m_strText == 'X'&&m_CellArray[0][2].m_strText == 'X')||
            (m_CellArray[1][0].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[1][2].m_strText == 'X') ||
            (m_CellArray[2][0].m_strText == 'X' && m_CellArray[2][1].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X') ||
            (m_CellArray[0][0].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X') ||
            (m_CellArray[0][2].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][0].m_strText == 'X') ||
            (m_CellArray[0][0].m_strText == 'X' && m_CellArray[1][0].m_strText == 'X' && m_CellArray[2][0].m_strText == 'X') ||
            (m_CellArray[0][1].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][1].m_strText == 'X') ||
            (m_CellArray[0][2].m_strText == 'X' && m_CellArray[1][2].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X'))
   {



     QMessageBox::information(this, tr("Message"),
                              tr("X Win."));
     QMessageBox::information(this, tr("Message"),
                              tr(" GameOver"));
   reset();
    }

      else if ((m_CellArray[0][0].m_strText == 'O' && m_CellArray[0][1].m_strText == 'O' &&m_CellArray[0][2].m_strText == 'O')||
              (m_CellArray[1][0].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[1][2].m_strText == 'O') ||
              (m_CellArray[2][0].m_strText == 'O' && m_CellArray[2][1].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O') ||
              (m_CellArray[0][0].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O') ||
              (m_CellArray[0][2].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][0].m_strText == 'O') ||
              (m_CellArray[0][0].m_strText == 'O' && m_CellArray[1][0].m_strText == 'O' && m_CellArray[2][0].m_strText == 'O') ||
              (m_CellArray[0][1].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][1].m_strText == 'O') ||
              (m_CellArray[0][2].m_strText == 'O' && m_CellArray[1][2].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O'))
   {

     QMessageBox::information(this, tr("Message"),
                              tr(" O Win."));

     QMessageBox::information(this, tr("Message"),
                              tr(" GameOver"));
    reset();
    }

}

void TicTacToeMainWindow::newgame()
{
   wincheck();
}
void TicTacToeMainWindow::on_pushButtonStart_clicked()
{
    if(ui->pushButtonStart->text()=="Start")
    {
         m_pBoxServer= new HelloWorldServer(this);
         bool success = m_pBoxServer->listen(QHostAddress::Any, quint16(ui->plainTextEditPort->toPlainText().toInt()));
         if(!success)
         {
             DisplayStatusMessage("Server failed...");
         }
         else
         {
             DisplayStatusMessage("Server Started...");
             this->setWindowTitle(this->windowTitle()+"|"+ui->plainTextEditPort->toPlainText());
             ui->pushButtonStart->setText("Stop");
         }

     }
     else
     {
         m_pBoxServer->close();
         DisplayStatusMessage("Server Stopped...");
         ui->pushButtonStart->setText("Start");
     }

    }






void TicTacToeMainWindow::on_pushButtonConnect_clicked()
{
    try
    {
        if(ui->pushButtonConnect->text()=="Connect")
        {
            m_pClientSocket->connectToHost(ui->plainTextEditRemoteIPAddr->toPlainText(),quint16(ui->plainTextEditRemotePort->toPlainText().toInt()) );
                connect(m_pClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
            ui->pushButtonConnect->setText("Disconnect");
            DisplayStatusMessage("Connected to Host =>" + ui->plainTextEditRemoteIPAddr->toPlainText() + "Port =>" + ui->plainTextEditRemotePort->toPlainText());
                 QMessageBox::information(this,"Message","Connection Successfull..");
        }
        else
        {
            m_pClientSocket->disconnectFromHost();
            m_pClientSocket->close();
            ui->pushButtonConnect->setText("Connect");
            DisplayStatusMessage("Disconnected from Host =>" + ui->plainTextEditRemoteIPAddr->toPlainText());

            QMessageBox::information(this,"Message","Connection Successfull..");
        }
    }
    catch(QException* ex)
    {
        DisplayStatusMessage(QString("Error in Connection=>") + QString(ex->what()));
    }
}


void TicTacToeMainWindow::mousePressEvent(QMouseEvent *event)
{
    wincheck();
    QWidget::mousePressEvent(event);

   DisableClick=0;
       if (event->button() == Qt::LeftButton)
       {
        // DisableClick=0;

          for(int i=0;i<DEFAULT_NO_ROWS;i++)
           {
             // DisableClick=0;
               for(int j=0;j<DEFAULT_NO_COLS;j++)
               {
                   QRect widgetRect = m_CellArray[i][j].m_CellRect;
                   widgetRect.moveTopLeft(this->parentWidget()->mapToGlobal(widgetRect.topLeft()));
                   QPoint CurretPoint=event->pos();
                   if(widgetRect.contains(CurretPoint))
                   {

                 if (DisableClick == 0)
                {

                       if ( m_CellArray[i][j].m_strText=="X"||  m_CellArray[i][j].m_strText=="O")
                       {

                             QMessageBox::information(this,"Message","Box Already Occupied,Try Another");
                             DisableClick = 0;
                        }


                            QString plaintxt=ui->plainTextEditRemotePort->toPlainText();

                            if(plaintxt=="5001")
                            {


                                 m_CellArray[i][j].m_strText="X";


                               DisableClick=1;

                          QString Message=CreateMessage();
                          DisplayStatusMessage(Message);
                           this->repaint();

                            // Sending moves to the Remote Connect PC
                            SendMovesToRemotePC(Message);



                      wincheck();

                         // DisableClick = 1;
                              return;
                         }
                          if(plaintxt=="5002")
                             {

                            m_CellArray[i][j].m_strText="O";

                             DisableClick=1;

                            QString Message=CreateMessage();
                            DisplayStatusMessage(Message);
                             this->repaint();

                              //Now send moves to the Remote Connect PC
                              SendMovesToRemotePC(Message);

                              wincheck();

                              return;
                            }
                         }

                 else

                        {


                           QMessageBox::information(this,"Message","Wait for the Opponent to play");
                            DisableClick=2;

                       }
                     }
                   }
               }
           }
   }
void TicTacToeMainWindow::DisplayRemotePCMessage(QString Message)
{

    //Check the message length first ..
    //This is a simple protocol message ie Just storing only cell moves
    //User might need to expand the protocol struct
    //to accomodate other settings paramaters eg Message="GridSize"+"#"+"X or Y"+"#"+Message;
    //based on the requirements
    if(Message.length()>=DEFAULT_NO_ROWS*DEFAULT_NO_COLS)
    {
        DisplayStatusMessage(Message);
    }
    else
    {
        DisplayStatusMessage("Message Length Small");
        return;
    }

    int index=0;
    for(int i=0;i<DEFAULT_NO_ROWS;i++)
    {
        for(int j=0;j<DEFAULT_NO_COLS;j++)
        {
            m_CellArray[i][j].m_strText=Message[index];
            index=index+1;
        }
    }
   this->repaint();

}




