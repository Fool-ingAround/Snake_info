#ifndef GRID_H
#define GRID_H
#endif //GRID_H
struct sgrid {
    bool occupied;
    int id;
};

class Grid {
    protected:
    sgrid matrix[25][100];
    public:

    Grid(int columns, int rows);
    void addItem(int y, int x, char item, int difficulty);
    void removeItem(int y, int x);
    bool addSnake(int y, int x);
    void removeSnake(int y, int x);
    bool CheckGrid(int y, int x);
    void Collision(int y, int x);
};