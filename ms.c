#include "ms.h"

// Maybe some of your own function prototypes here

board solve_board(board b)
{
    bool changed = true;

    while(changed){
        changed= false;

        int i, j, x, y, known_mines=0;
        for(i=0; i< b.h; i++){
            for(j=0; j<b.w; j++){
                if(b.grid[i][j] == -2)
                    known_mines++;
            }
        }
        if(known_mines == b.totmines){   //rule 1: all the mines on the board
            for(i=0; i< b.h; i++){
                for(j=0; j<b.w; j++){
                    if(b.grid[i][j] == -1){//如果是未知数
                        int mine_count=0;
                        for(x=-1; x<=1; x++){
                            for(y=-1; y<=1; y++){
                                if(i+x>=0 && i+x< b.h && j+y>=0 && j+y< b.w){ //不溢出
                                    if(b.grid[i+x][j+y]==-2)
                                        mine_count++;
                                }
                            }
                        }
                    b.grid[i][j] = mine_count;
                        changed = true;
                    }
                }
            }
        }
        //rule 2
        for(i=0; i<b.h; i++){
            for(j=0; j<b.w; j++){
                if(b.grid[i][j] >= 0 && b.grid[i][j] <= 8){
                    int known_mine_around = 0;
                    int unknowns_around = 0;
                    for(x=-1; x<=1; x++){
                        for(y=-1; y<=1; y++){
                            if(i+x>=0 && i+x< b.h && j+y>=0 && j+y< b.w) { //不溢出
                                if (b.grid[i + x][j + y] == -2)//mine
                                    known_mine_around++;
                                else if (b.grid[i + x][j + y] == -1)
                                    unknowns_around++;
                            }
                        }
                    }
                    if((b.grid[i][j] == known_mine_around + unknowns_around) && unknowns_around > 0){
                        for(x=-1; x<=1; x++){
                            for(y=-1; y<=1; y++){
                                if(i+x>=0 && i+x< b.h && j+y>=0 && j+y< b.w && b.grid[i + x][j + y] == -1 ){
                                    b.grid[i + x][j + y] = -2; //mine
                                    changed= true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
 return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
    int i, j, a=0;
    for(i=0; i<b.h; i++){
        for(j=0; j<b.w; j++){
            if(b.grid[i][j] == -1)
                s[a]= UNK;
            else if(b.grid[i][j] == -2)
                s[a] = MINE;
            else
                s[a]= b.grid[i][j] + '0';
            a++;
        }
    }
    s[a]= '\0';
}

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
    // Make sure number of characters in string == width*height
    if(strlen(inp) != (width * height))
        return false;

    // Ensure only characters are from the set:   012345678?X
    int i;
    int mine_count=0;
    for(i=0; i< width*height; i++){
        char c = inp[i];
        if(!(c == '0'  || c =='1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' \
        || c == '7' || c == '8'|| c == UNK|| c == MINE))
        return false;
         if(c== MINE)
            mine_count++;
    }

    // Ensure mines in string <= totmines
    if(mine_count > totmines)
        return false;

    return true;

}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{
    board b;
    b.totmines = totmines;
    b.w = width;
    b.h = height;

    int a=0;
    int i, j;
    for(i=0; i< height ; i++){
        for(j=0; j< width ; j++){
            char c = inp[a];
            if(c >= '0' && c<= '8')
            b.grid[i][j] = c - '0'; //number square
            else if(c == MINE)
                b.grid[i][j] = -2; //mine square
            else if(c == UNK)
                b.grid[i][j] = -1; //unknown square
            a++;
        }

    }
    return b;
}

void test(void)
{
    char testInput[MAXSQ*MAXSQ+1] = "11?0?X1111111X?11?11?X10?";
    int width = 5;
    int height = 5;
    int totmines = 3;
    board b = make_board(totmines, width, height, testInput);
    assert(syntax_check(3, 5, 5, testInput)==true);
    board solved = solve_board(b);
    char result[MAXSQ*MAXSQ+1];
    board2str(result, solved);
    char expected[MAXSQ*MAXSQ+1] = "11000X1111111X1112111X100";
    //printf("Result: %s\n", result);
    //printf("Expected: %s\n", expected);
    assert(strcmp(result, expected) == 0);
}
