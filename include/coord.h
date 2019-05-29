// This is easier to deal with than int arrays
#ifndef COORD_H
#define COORD_H


class Coord
{
    public:
        // Variables
        int x;
        int y;

        // Functions
        Coord();
        Coord(int x, int y);
        virtual ~Coord();

    protected:

    private:
};

#endif // COORD_H
