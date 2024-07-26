#include "maze.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>
#include <QThread>
#include <QApplication>

Maze::Maze(QWidget *parent) : QWidget(parent),
    mazeWidth(21), mazeHeight(21), squareSize(20)
{
    setFixedSize(mazeWidth * squareSize, mazeHeight * squareSize);

    maze.resize(mazeWidth, QVector<bool>(mazeHeight, true));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Maze::generateMaze);

    int interval = 1000;
    timer->setInterval(interval);
    // To start with a maze instead of black screen
    generateMaze();
    timer->start();
    timerID = timer->timerId();
}

Maze::~Maze()
{
    timer->stop();
    delete timer;
}

void Maze::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate the size of each cell
    int cellSize = std::min(width() / mazeWidth, height() / mazeHeight);

    // Draw the maze
    for (int x = 0; x < mazeWidth; ++x)
    {
        for (int y = 0; y < mazeHeight; ++y)
        {
            if (maze[x][y])
            {
                painter.fillRect(x * cellSize, y * cellSize, cellSize, cellSize, Qt::black);
            }
        }
    }

    // Draw the solution path
    if(!path.isEmpty()){
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        for (const QPoint& point : path)
        {
            int x = point.x();
            int y = point.y();
            painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
        }
    }
}

void Maze::animatePath()
{

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (currentIndex >= 0)
        {
            currentIndex--;
            update();
        }
        else
        {
            timer->stop();
        }
    });
    timer->start(100);
}

void Maze::generateMaze()
{
    //Initialize maze grid with walls
    maze.fill(QVector<bool>(mazeHeight, true));

    //Set the starting point
    QPoint startPoint(0, 0);
    maze[startPoint.x()][startPoint.y()] = false;

    //Generate the maze recursively
    generateMazeRecursive(startPoint);

    //Solve the maze
    solveMaze();

    //Update the widget
    update();
}

void Maze::animateMazeGeneration()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (currentIndex < path.size() - 1)
        {
            currentIndex++;
            maze[path[currentIndex].x()][path[currentIndex].y()] = false;
            update();
        }
        else
        {
            timer->stop();
        }
    });
    timer->start(100);
}

void Maze::generateMazeRecursive(const QPoint& current)
{
    //Generate random order of neighbors
    QVector<QPoint> neighbors = { QPoint(current.x() - 2, current.y()),
                                  QPoint(current.x() + 2, current.y()),
                                  QPoint(current.x(), current.y() - 2),
                                  QPoint(current.x(), current.y() + 2) };

    QRandomGenerator* generator = QRandomGenerator::global();
    int n = neighbors.size();
    for (int i = n - 1; i > 0; --i) {
        int j = generator->bounded(i + 1);
        std::swap(neighbors[i], neighbors[j]);
    }

    for (const QPoint& neighbor : neighbors)
    {
        int wallX = current.x() + (neighbor.x() - current.x()) / 2;
        int wallY = current.y() + (neighbor.y() - current.y()) / 2;

        if (isValidMove(neighbor.x(), neighbor.y()))
        {
            maze[neighbor.x()][neighbor.y()] = false;
            maze[wallX][wallY] = false;
            generateMazeRecursive(neighbor);
        }
    }
}

void Maze::solveMaze()
{
    path.clear();
    solve(0, 0);
    update();
}

void Maze::drawMaze(QPainter &painter)
{
    painter.setPen(Qt::black);  // for block lines
    painter.setBrush(Qt::black);  // for blocks

    for (int x = 0; x < mazeWidth; ++x)
    {
        for (int y = 0; y < mazeHeight; ++y)
        {
            if (maze[x][y])
                painter.drawRect(x * squareSize, y * squareSize, squareSize, squareSize);
        }
    }
}

void Maze::drawPath(QPainter &painter)
{
    if (path.isEmpty())
        return;

    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);

    for (int i = 0; i < currentIndex; ++i) {
        QPoint current = path[i];
        QPoint next = path[i + 1];

        int x1 = current.x() * squareSize + squareSize / 2;
        int y1 = current.y() * squareSize + squareSize / 2;

        int x2 = next.x() * squareSize + squareSize / 2;
        int y2 = next.y() * squareSize + squareSize / 2;

        painter.drawLine(x1, y1, x2, y2);
    }

    QPoint last = path.last();
    int x = last.x() * squareSize;
    int y = last.y() * squareSize;
    painter.drawRect(x, y, squareSize, squareSize);

}

bool Maze::isValidMove(int x, int y)
{
    return (x >= 0 && x < mazeWidth && y >= 0 && y < mazeHeight && maze[x][y]);
}

bool Maze::solve(int x, int y)
{
    if (x == mazeWidth - 1 && y == mazeHeight - 1)
    {
        path.push_back(QPoint(x, y));
        return true;
    }

    if (isValidMove(x, y))
    {
        maze[x][y] = false;
        path.push_back(QPoint(x, y));

        if (solve(x + 1, y) || solve(x, y + 1))
            return true;

        //path.pop_back();
        //maze[x][y] = true;
    }

    return false;
}

//Additional methods

void Maze::resetMaze()
{
    maze.fill(QVector<bool>(mazeHeight, true));
    generateMaze();
    solveMaze();
    update();
}

void Maze::resizeMaze(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    mazeWidth = width;
    mazeHeight = height;
    setFixedSize(mazeWidth * squareSize, mazeHeight * squareSize);

    maze.resize(mazeWidth, QVector<bool>(mazeHeight, true));
    generateMaze();
    solveMaze();
    update();
}

void Maze::generateMazeWithAlgorithm(MazeAlgorithm algorithm)
{
    //Clear the maze and generate a new one based on the selected algorithm
    maze.fill(QVector<bool>(mazeHeight, true));

    switch (algorithm)
    {
        case MazeAlgorithm::Prim:
            generateMazeUsingPrim();
            break;
        case MazeAlgorithm::RecursiveBacktracking:
            generateMaze();
            break;
    }

    solveMaze();
    update();
}

void Maze::generateMazeUsingPrim()
{
    QPoint startPoint = QPoint(0, 0);
    maze[startPoint.x()][startPoint.y()] = false;

    QVector<QPoint> walls;
    addWalls(startPoint, walls);

    while (!walls.isEmpty())
    {
        int randomIndex = QRandomGenerator::global()->bounded(walls.size());
        QPoint currentWall = walls[randomIndex];
        walls.remove(randomIndex);

        QPoint neighbor = currentWall + (currentWall - getNearestVisitedCell(currentWall));

        if (isValidMove(neighbor.x(), neighbor.y()))
        {
            maze[currentWall.x()][currentWall.y()] = false;
            maze[neighbor.x()][neighbor.y()] = false;
            addWalls(neighbor, walls);
        }
    }
}

void Maze::addWalls(const QPoint &cell, QVector<QPoint> &walls)
{
    const QVector<QPoint> directions = { QPoint(-2, 0), QPoint(2, 0), QPoint(0, -2), QPoint(0, 2) };

    for (const QPoint &direction : directions)
    {
        QPoint neighbor = cell + direction;

        if (isValidMove(neighbor.x(), neighbor.y()))
            walls.append(neighbor);
    }
}

QPoint Maze::getNearestVisitedCell(const QPoint &cell)
{
    const QVector<QPoint> directions = { QPoint(-1, 0), QPoint(1, 0), QPoint(0, -1), QPoint(0, 1) };

    for (const QPoint &direction : directions)
    {
        QPoint neighbor = cell + direction;

        if (neighbor.x() >= 0 && neighbor.x() < mazeWidth &&
            neighbor.y() >= 0 && neighbor.y() < mazeHeight &&
            !isValidMove(neighbor.x(), neighbor.y()))
        {
            return neighbor;
        }
    }

    return cell;
}

void Maze::changeSquareSize(int size)
{
    if (size <= 0)
        return;

    squareSize = size;
    setFixedSize(mazeWidth * squareSize, mazeHeight * squareSize);
    update();
}

QSize Maze::minimumSizeHint() const
{
    return QSize(mazeWidth * squareSize, mazeHeight * squareSize);
}

QSize Maze::sizeHint() const
{
    return QSize(mazeWidth * squareSize, mazeHeight * squareSize);
}
