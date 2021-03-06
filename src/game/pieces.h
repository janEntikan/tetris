#ifndef PIECES
#define PIECES

class Pieces {
public:
    int pieces[7][5][5] = {
      {//block
        0,0,0,0,0,
        0,1,1,0,0,
        0,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
      },

      {//t
        0,0,0,0,0,
        0,0,1,0,0,
        0,1,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
      },

      {//l
        0,0,0,0,0,
        0,1,0,0,0,
        0,1,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
      },

      {//7
        0,0,0,0,0,
        0,0,0,0,0,
        0,1,1,1,0,
        0,1,0,0,0,
        0,0,0,0,0,
      },

      {//s
        0,0,0,0,0,
        0,1,1,0,0,
        0,0,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
      },

      {//z
        0,0,0,0,0,
        0,0,1,1,0,
        0,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
      },

      {//line
        0,0,0,0,0,
        0,0,0,0,0,
        0,1,1,1,1,
        0,0,0,0,0,
        0,0,0,0,0,
      },
    };

    int current_piece[5][5]; 
    int transformed_piece[5][5];
    void transpose();
    void flip_rows();
    void flip_collumns();
    void apply_transform();
    void copy(int);
};

#endif

