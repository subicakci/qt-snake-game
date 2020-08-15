#include "mainwindow.h"
#include <QDebug>
#include <QPalette>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(400,400);
    m_apple_pos.setX(400);
    m_apple_pos.setY(400);

    label = new QLabel("Snake Game",this);
    label->setGeometry(60,0,400,150);
    label->setStyleSheet("QLabel{font-size: 50px;font-family: Arial;color: rgb(255, 0, 0);background-color: rgb(0,0,0);position: center;}");


    new_game_button = new QPushButton("New Game",this);
    new_game_button->setGeometry(0,250,400,50);
    new_game_button->setStyleSheet("QPushButton{font-size: 20px;font-family: Arial;color: rgb(255, 0, 0);background-color: rgb(0,0,0);}");
//    new_game_button->setFlat(true);
    connect(new_game_button, SIGNAL(released()),this,SLOT(button_pressed()));

    quit_button = new QPushButton("Quit",this);
    quit_button->setGeometry(0,300,400,50);
    quit_button->setStyleSheet("QPushButton{font-size: 20px;font-family: Arial;color: rgb(255, 0, 0);background-color: rgb(0,0,0);}");
//    new_game_button->setFlat(true);
    connect(quit_button, SIGNAL(released()),this,SLOT(close()));



    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(gameLoop()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->drawRect(0,0,400,400);

    if(!isGameover)
    {

        painter->setPen(Qt::black);
        painter->setBrush(Qt::red);

        for(int i = 0;i < m_snake_pos.length();i++)
            painter->drawRect(m_snake_pos.at(i).x(),m_snake_pos.at(i).y(),snake_width,snake_width);


        painter->setPen(Qt::green);
        painter->setFont(QFont("Times", 12, QFont::Bold));
        painter->drawText(QRect(0,0,400,20), Qt::AlignCenter, "Score : " + QString::number(m_snake_pos.length() - 3));

        painter->setPen(Qt::black);
        painter->setBrush(Qt::green);
        if(isEaten)
        {

            qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));

            m_apple_pos.setX((qrand() % snake_width)*snake_width);
            m_apple_pos.setY((qrand() % snake_width)*snake_width);
            isEaten = false;

        }
        painter->drawRect(m_apple_pos.x(),m_apple_pos.y(),snake_width,snake_width);


    }
    else
    {
        painter->setPen(Qt::red);
        painter->setFont(QFont("Times", 40, QFont::Bold));
        painter->drawText(QRect(0,0,400,100), Qt::AlignCenter, "Game Over!");

        painter->setFont(QFont("Times", 30));
        painter->drawText(QRect(0,100,400,100), Qt::AlignCenter, "Score : " + QString::number(m_snake_pos.length() - 3));

        new_game_button->setEnabled(true);
        new_game_button->setVisible(true);
        quit_button->setEnabled(true);
        quit_button->setVisible(true);
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!isGameover)
    {
        if(event->key() == Qt::Key_Right && direction != Qt::Key_Left)
            direction = Qt::Key_Right;
        else if(event->key() == Qt::Key_Left && direction != Qt::Key_Right)
            direction = Qt::Key_Left;
        else if(event->key() == Qt::Key_Up && direction != Qt::Key_Down)
            direction = Qt::Key_Up;
        else if(event->key() == Qt::Key_Down && direction != Qt::Key_Up)
            direction = Qt::Key_Down;
    }
    else if(event->key() == Qt::Key_R)
        newGame();


}

void MainWindow::newGame()
{
    isGameover = false;
    direction = Qt::Key_Right;

    if(!m_snake_pos.isEmpty())
        m_snake_pos.clear();

    for(int i = 0; i < 3; i++)
        m_snake_pos.push_front(QPoint(i*snake_width,0));


    timer->start(40);

}

void MainWindow::gameLoop()
{
    if(m_snake_pos.first().x() == m_apple_pos.x() && m_snake_pos.first().y() == m_apple_pos.y())
    {

        isEaten = true;

        if(direction == Qt::Key_Right )
            m_snake_pos.append(QPoint(m_snake_pos.first().x() + 20, m_snake_pos.first().y() ));
        else if(direction == Qt::Key_Left )
            m_snake_pos.append(QPoint(m_snake_pos.first().x() - 20, m_snake_pos.first().y() ));
        else if(direction == Qt::Key_Up)
            m_snake_pos.append(QPoint(m_snake_pos.first().x(), m_snake_pos.first().y() - 20 ));
        else if(direction == Qt::Key_Down)
            m_snake_pos.append(QPoint(m_snake_pos.first().x(), m_snake_pos.first().y() + 20));

    }


    for (int i = m_snake_pos.length() - 1; i > 0; i--)
    {
        m_snake_pos[i] = m_snake_pos[i - 1];

    }


    if(direction == Qt::Key_Right )
    {

        m_snake_pos.first().rx() += snake_width;
        if(m_snake_pos.first().x() >= 400)
            m_snake_pos.first().rx() = 0;
    }
    else if(direction == Qt::Key_Left)
    {

        if(m_snake_pos.first().x() <= 0)
            m_snake_pos.first().rx() = 400;
        m_snake_pos.first().rx() -= snake_width;
    }
    else if(direction == Qt::Key_Up)
    {

        if(m_snake_pos.first().y() <= 0)
            m_snake_pos.first().ry() = 400;
        m_snake_pos.first().ry() -= snake_width;

    }
    else if(direction == Qt::Key_Down)
    {
        m_snake_pos.first().ry() += snake_width;
        if(m_snake_pos.first().y() >= 400)
            m_snake_pos.first().ry() = 0;

    }

    for (int i = m_snake_pos.length() - 1; i > 0; i--)
    {
        if(m_snake_pos[i] == m_snake_pos[0] && isGameover == false)
        {
            isGameover = true;
            timer->stop();

        }

    }


    update();

}

void MainWindow::button_pressed()
{
    newGame();
    isEaten = true;

    new_game_button->setEnabled(false);
    new_game_button->setVisible(false);
    quit_button->setEnabled(false);
    quit_button->setVisible(false);
    label->setEnabled(false);
    label->setVisible(false);


}
