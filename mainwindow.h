#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QPoint>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<QPoint> m_snake_pos;
    const int snake_width = 20;
    QTimer *timer;
    int direction = Qt::Key_Right;

    QPushButton *new_game_button;
    QPushButton *quit_button;
    QLabel *label;
    QPoint m_apple_pos;
    bool isEaten = false;
    bool isGameover = false;

    int error_counter = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
public slots:
    void newGame();
private slots:
    void gameLoop();
    void button_pressed();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
