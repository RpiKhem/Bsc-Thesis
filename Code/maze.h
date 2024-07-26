#ifndef MAZE_H
#define MAZE_H

#include <QWidget>
#include <QVector>
#include <iostream>
enum class MazeAlgorithm
{
    Prim,
    RecursiveBacktracking
};

class Maze : public QWidget
{
    Q_OBJECT

public:
    explicit Maze(QWidget *parent = nullptr);
    ~Maze();

    void resetMaze();
    void resizeMaze(int width, int height);
    void generateMazeWithAlgorithm(MazeAlgorithm algorithm);
    void changeSquareSize(int size);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int currentIndex = -1;
    int mazeWidth;
    int mazeHeight;
    int squareSize;
    QVector<QVector<bool>> maze;
    QVector<QPoint> path;
    QTimer* timer;
    int timerID;

    void generateMaze();
    void animateMazeGeneration();
    void generateMazeRecursive(const QPoint& current);
    void generateMazeUsingPrim();
    void addWalls(const QPoint& cell, QVector<QPoint>& walls);
    QPoint getNearestVisitedCell(const QPoint& cell);
    void solveMaze();
    bool solve(int x, int y);
    void drawMaze(QPainter& painter);
    void drawPath(QPainter& painter);
    void animatePath();
    bool isValidMove(int x, int y);
};

#endif // MAZE_H
