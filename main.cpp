#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <windows.h>
using namespace std;


#define WHITE 0
#define BLACK 1

struct Point
{
    Point(int xx,int yy):x(xx),y(yy){}
    Point(){}
    int x,y;

    bool operator==(const Point&p){return (x==p.x)&&(y==p.y);}
};

enum MoveStatus
{
    GOOD,
    BAD,
    ANULL
};

class Figure;

struct cell
{
    ~cell(){delete myFigure;};

    Figure* myFigure=nullptr;
    bool occupied=false;
    char currentFigure='0';
} chessBoard[8][8];
bool GAME_END=false;

void displayChessboard();


class Figure
{

public:
    Figure(int x,int y,char d,int t):myPos(x,y),display(d),team(t){}

    ///Variables
Point myPos;
char display;
int team;
bool firstMove=true;
bool enPassantNeighbour=false;


virtual vector<Point> calcualtePossibleMoves()=0; ///Every figure has its own way of moving so we make this func virtual
virtual void specialAction(){}///some figures have their own special move like roszada or 2 tiles jumb for pawn at the start
MoveStatus tryToMove()
{
    MoveStatus myStatus = BAD;
    //while(true){

    vector<Point> possibleCells = calcualtePossibleMoves();



    while(myStatus==BAD)
    {//vector<Point> possibleCells = calcualtePossibleMoves();///delete this
        system("cls");
        Point p;


        displayChessboard();


        cout<<endl;
        cout<<"Currently selected figure: "<<display<<" at position: "<<myPos.x<<' '<<myPos.y<<endl;
        cout<<endl;
        ///END DISPLAY

        cout<<"Provide x and y(space between two), (-1 -1) to switch piece"<<endl;
        cin>>p.x>>p.y;

        if(p.x==-1||p.y==-1)return ANULL;

       vector<Point>::iterator it;

       it=find(possibleCells.begin(),possibleCells.end(),p);

       if(it==possibleCells.end())
       {
           ///No instance
       }
        else
        {
        move(p);
        specialAction();
        myStatus=GOOD;
        }
    }
   // }///test
    return myStatus;
}
void move(Point destination)///move figure and if there is sth then delete it and replace with this figure
{
    if(chessBoard[destination.y][destination.x].myFigure!=nullptr)
    {
        ///There is an enemy. Delete him.
        if(chessBoard[destination.y][destination.x].myFigure->display=='W')GAME_END=true;
         delete chessBoard[destination.y][destination.x].myFigure;
         chessBoard[destination.y][destination.x].myFigure=this;

         ///Clear last position
         chessBoard[myPos.y][myPos.x].myFigure=nullptr;
         myPos.y=destination.y;
         myPos.x=destination.x;
    }
    else
    {

         chessBoard[destination.y][destination.x].myFigure=this;

         ///Clear last position
         chessBoard[myPos.y][myPos.x].myFigure=nullptr;
         chessBoard[myPos.y][myPos.x].currentFigure='0';
         myPos.y=destination.y;
         myPos.x=destination.x;
    }
}

};

class Tower :public Figure
{

public:
    Tower( int x,int y,char d,int t): Figure(x,y,d,t){}
    //bool firstMove=true;
     vector<Point> calcualtePossibleMoves()
     {
         ///Check N S W E directions
         vector<Point> possibleMoves;

         Point startPos=myPos;

         ///WESTERN
         startPos.x-=1;
         while(startPos.x>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.x-=1;
         }


         ///EASTERN
         startPos=myPos;
                 startPos.x+=1;
         while(startPos.x<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.x+=1;
         }

                  ///North
         startPos=myPos;
                 startPos.y-=1;
         while(startPos.y>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.y-=1;
         }

                           ///SOUTH
         startPos=myPos;
                 startPos.y+=1;
         while(startPos.y<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.y+=1;
         }

         return possibleMoves;

     }


     void specialAction()
     {
         if(firstMove)///Roszada
         {

             Point startPos=myPos;

             /// W - king R - rook  ->    W R  - rook comes from right side

             startPos.x-=1;
             if(startPos.x>0)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team==team)///There is sth so lets see wheather its our king
                    {

                        if(chessBoard[startPos.y][startPos.x].myFigure->display=='W'&&chessBoard[startPos.y][startPos.x].myFigure->firstMove)///is it the king
                            {
                                chessBoard[startPos.y][startPos.x+2].myFigure=chessBoard[startPos.y][startPos.x].myFigure;
                                chessBoard[startPos.y][startPos.x+2].myFigure->myPos=Point(startPos.x+2,startPos.y);
                                chessBoard[startPos.y][startPos.x+2].myFigure->firstMove=false;
                                chessBoard[startPos.y][startPos.x].myFigure=nullptr;

                            }

                    }


             }
             startPos=myPos;
             /// W - king R - rook  ->    R W  - rook comes from left side

             startPos.x+=1;
             if(startPos.x<7)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team==team)///There is sth so lets see wheather its our king
                    {

                        if(chessBoard[startPos.y][startPos.x].myFigure->display=='W'&&chessBoard[startPos.y][startPos.x].myFigure->firstMove)///is it the king
                            {
                                chessBoard[startPos.y][startPos.x-2].myFigure=chessBoard[startPos.y][startPos.x].myFigure;
                                chessBoard[startPos.y][startPos.x-2].myFigure->myPos=Point(startPos.x-2,startPos.y);
                                chessBoard[startPos.y][startPos.x-2].myFigure->firstMove=false;
                                chessBoard[startPos.y][startPos.x].myFigure=nullptr;

                            }

                    }


             }

         }


         firstMove=false;
     }


};


class Bishop :public Figure
{
    public:
     Bishop( int x,int y,char d,int t): Figure(x,y,d,t){}

     vector<Point> calcualtePossibleMoves()
     {

        ///Check diagonal directions
         vector<Point> possibleMoves;

         Point startPos=myPos;

         ///North West
         startPos.x-=1;
         startPos.y-=1;
         while(startPos.x>=0&&startPos.y>=0)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x-=1;
             startPos.y-=1;
         }


         ///North East
         startPos=myPos;
         startPos.x+=1;
         startPos.y-=1;
         while(startPos.x<=7&&startPos.y>=0)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x+=1;
             startPos.y-=1;
         }

        ///South East
         startPos=myPos;
         startPos.x+=1;
         startPos.y+=1;
         while(startPos.x<=7&&startPos.y<=7)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x+=1;
             startPos.y+=1;
         }


         ///South West
         startPos=myPos;
         startPos.x-=1;
         startPos.y+=1;
         while(startPos.x>=0&&startPos.y<=7)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x-=1;
             startPos.y+=1;
         }

         return possibleMoves;
     }


};

class Knight :public Figure
{


        public:
     Knight( int x,int y,char d,int t): Figure(x,y,d,t){}

     vector<Point> calcualtePossibleMoves()
     {
        /// this is gonna be a long function :)

     ///   0X0X0
     ///   X000X
     ///   00M00
     ///   X000X
     ///   0X0X0

        ///Check diagonal directions
         vector<Point> possibleMoves;

         Point startPos=myPos;

         /// Up Left
        startPos.x-=1;
        startPos.y-=2;
        if(startPos.x>=0&&startPos.y>=0)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure==nullptr || (chessBoard[startPos.y][startPos.y].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Up right
        startPos=myPos;
         startPos.x+=1;
        startPos.y-=2;
        if(startPos.x<=7&&startPos.y>=0)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Down left
        startPos=myPos;
         startPos.x-=1;
        startPos.y+=2;
        if(startPos.x>=0&&startPos.y<=7)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Down right
        startPos=myPos;
         startPos.x+=1;
        startPos.y+=2;
        if(startPos.x<=7&&startPos.y<=7)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Left Up
        startPos=myPos;
         startPos.x-=2;
        startPos.y-=1;
        if(startPos.x>=0&&startPos.y>=0)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Left Down
        startPos=myPos;
         startPos.x-=2;
        startPos.y+=1;
        if(startPos.x>=0&&startPos.y<=7)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }


        ///Right Up
        startPos=myPos;
         startPos.x+=2;
        startPos.y-=1;
        if(startPos.x<=7&&startPos.y>=0)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }

        ///Right Down
        startPos=myPos;
         startPos.x+=2;
        startPos.y+=1;
        if(startPos.x<=7&&startPos.y<=7)
        {
                 ///There should be check for teams
                 //if(chessBoard[startPos.y][startPos.y].myFigure!=nullptr)
                 //{
                 //    if(chessBoard[startPos.y][startPos.y].myFigure->team!=team)possibleMoves.push_back(startPos);
                 //}
                 //else
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
                 possibleMoves.push_back(startPos);
        }


        return possibleMoves;
     }


};



class Queen :public Figure
{

public:
    Queen( int x,int y,char d,int t): Figure(x,y,d,t){}

     vector<Point> calcualtePossibleMoves()
     {
         ///Check N S W E directions
         vector<Point> possibleMoves;

         Point startPos=myPos;

         ///BISHOP PART
         ///WHOOOO SMOKI I GO£E BABY
         ///RYCERZ NA KONIU MKNIE
         ///PÊDZI JAK POEBANY
         ///SZYBCIEJ JU¯ NIE DA SIÊÊÊÊÊ

         ///North West
         startPos.x-=1;
         startPos.y-=1;
         while(startPos.x>=0&&startPos.y>=0)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x-=1;
             startPos.y-=1;
         }


         ///North East
         startPos=myPos;
         startPos.x+=1;
         startPos.y-=1;
         while(startPos.x<=7&&startPos.y>=0)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {

                 possibleMoves.push_back(startPos);
             }

             startPos.x+=1;
             startPos.y-=1;
         }

        ///South East
         startPos=myPos;
         startPos.x+=1;
         startPos.y+=1;
         while(startPos.x<=7&&startPos.y<=7)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x+=1;
             startPos.y+=1;
         }


         ///South West
         startPos=myPos;
         startPos.x-=1;
         startPos.y+=1;
         while(startPos.x>=0&&startPos.y<=7)
         {
            if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);
             }

             startPos.x-=1;
             startPos.y+=1;
         }


        ///ROOK PART
         ///
         ///
         ///
         ///

         startPos=myPos;

         ///WESTERN
         startPos.x-=1;
         while(startPos.x>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.x-=1;
         }


         ///EASTERN
         startPos=myPos;
                 startPos.x+=1;
         while(startPos.x<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.x+=1;
         }

                  ///North
         startPos=myPos;
                 startPos.y-=1;
         while(startPos.y>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.y-=1;
         }

                           ///SOUTH
         startPos=myPos;
                 startPos.y+=1;
         while(startPos.y<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr)///There should be check for teams
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                 possibleMoves.push_back(startPos);
                 break;
             }
             else
             {
                 possibleMoves.push_back(startPos);

             }

            startPos.y+=1;
         }


         return possibleMoves;
     }

};

class King :public Figure
{

public:
    King( int x,int y,char d,int t): Figure(x,y,d,t){}
    //bool firstMove=true;
     vector<Point> calcualtePossibleMoves()
     {
         ///***
         ///*P*
         ///***
         vector<Point> possibleMoves;

         Point startPos=myPos;

         ///NORTH
         startPos.y-=1;
         if(startPos.y>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///SOUTH
         startPos=myPos;
         startPos.y+=1;
         if(startPos.y<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///WEST
         startPos=myPos;
         startPos.x-=1;
         if(startPos.x>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///EAST
         startPos=myPos;
         startPos.x+=1;
         if(startPos.x<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///NORTH WEST
         startPos=myPos;
         startPos.y-=1;
         startPos.x-=1;
         if(startPos.y>=0&&startPos.x>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///NORTH EAST
         startPos=myPos;
         startPos.y-=1;
         startPos.x+=1;
         if(startPos.y>=0&&startPos.x<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///SOUTH WEST
         startPos=myPos;
         startPos.y+=1;
         startPos.x-=1;
         if(startPos.y<=7&&startPos.x>=0)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }

         ///SOUTH EAST
         startPos=myPos;
         startPos.y+=1;
         startPos.x+=1;
         if(startPos.y<=7&&startPos.x<=7)
         {
             if(chessBoard[startPos.y][startPos.x].myFigure==nullptr || (chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team))
             possibleMoves.push_back(startPos);
         }


         return possibleMoves;
     }

     void specialAction()
     {
         firstMove=false;
     }

};

class Pawn :public Figure
{

public:
    Pawn( int x,int y,char d,int t,bool from): Figure(x,y,d,t),startsFromTop(from){}
    //bool firstMove=true;
    bool startsFromTop;
    bool didDoubleJump=false;
    ///bool enPassantNeighbour=false;
     vector<Point> calcualtePossibleMoves()
     {
         ///Check N S W E directions
         vector<Point> possibleMoves;

         Point startPos=myPos;


         if(startsFromTop)
         {
             if(firstMove)///First move can be 2 tiles
             {
                 startPos.y+=1;
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr)
                    {
                        possibleMoves.push_back(startPos);
                        startPos.y+=1;
                        if(chessBoard[startPos.y][startPos.x].myFigure==nullptr){possibleMoves.push_back(startPos);}
                    }

                 ///firstMove=false; dunno how to unset it
             }
             else
             {
                  startPos.y+=1;
                 if(startPos.y<=7&&chessBoard[startPos.y][startPos.x].myFigure==nullptr)
                    {
                        possibleMoves.push_back(startPos);
                    }

             }


             ///Attack Left Wing
             startPos=myPos;
             startPos.x-=1;
             startPos.y+=1;
             if(startPos.x>=0&&startPos.y<=7)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                    possibleMoves.push_back(startPos);

             }

             ///Attack Right Wing
             startPos=myPos;
             startPos.x+=1;
             startPos.y+=1;
             if(startPos.x<=7&&startPos.y<=7)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                    possibleMoves.push_back(startPos);

             }

             ///EN PASSANT RIGHT
            startPos=myPos;
             startPos.x+=1;
              if(startPos.x<=7&&startPos.y<=7)
              {
                if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team&&chessBoard[startPos.y][startPos.x].myFigure->display=='P')
                {
                    startPos.y+=1;
                    possibleMoves.push_back(startPos);
                }
              }

                  ///EN PASSANT LEFT
            startPos=myPos;
             startPos.x-=1;
              if(startPos.x>=0&&startPos.y<=7)
              {
                if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team&&chessBoard[startPos.y][startPos.x].myFigure->display=='P')
                {
                    startPos.y+=1;
                    possibleMoves.push_back(startPos);
                }
              }

         }

            else ///starts from bottom
         {
             if(firstMove)///First move can be 2 tiles
             {
                 startPos.y-=1;
                 if(chessBoard[startPos.y][startPos.x].myFigure==nullptr)
                    {
                        possibleMoves.push_back(startPos);
                        startPos.y-=1;
                        if(chessBoard[startPos.y][startPos.x].myFigure==nullptr){possibleMoves.push_back(startPos);}
                    }

                 ///firstMove=false; dunno how to unset it
             }
             else
             {
                  startPos.y-=1;
                 if(startPos.y>=0&&chessBoard[startPos.y][startPos.x].myFigure==nullptr)
                    {
                        possibleMoves.push_back(startPos);
                    }

             }


             ///Attack Left Wing
             startPos=myPos;
             startPos.x-=1;
             startPos.y-=1;
             if(startPos.x>=0&&startPos.y>=0)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                    possibleMoves.push_back(startPos);

             }

             ///Attack Right Wing
             startPos=myPos;
             startPos.x+=1;
             startPos.y-=1;
             if(startPos.x<=7&&startPos.y>=0)
             {
                 if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team)
                    possibleMoves.push_back(startPos);

             }


             ///EN PASSANT RIGHT
            startPos=myPos;
             startPos.x+=1;
              if(startPos.x<=7&&startPos.y>=0)
              {
                if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team&&chessBoard[startPos.y][startPos.x].myFigure->display=='P'&&chessBoard[startPos.y][startPos.x].myFigure->enPassantNeighbour)
                {
                    startPos.y-=1;
                    possibleMoves.push_back(startPos);
                }
              }

                  ///EN PASSANT LEFT
            startPos=myPos;
             startPos.x-=1;
              if(startPos.x>=0&&startPos.y<=7)
              {
                if(chessBoard[startPos.y][startPos.x].myFigure!=nullptr&&chessBoard[startPos.y][startPos.x].myFigure->team!=team&&chessBoard[startPos.y][startPos.x].myFigure->display=='P'&&chessBoard[startPos.y][startPos.x].myFigure->enPassantNeighbour)
                {
                    startPos.y-=1;
                    possibleMoves.push_back(startPos);
                }
              }



         }


         return possibleMoves;
     }

     void specialAction()
     {


         if(firstMove)///neighbours can perform En passant
         {
             ///MMAGIC TO FIND OUT DOUBLE JUMP OR NOT

             if(startsFromTop)
             {
                 if(myPos.y==3)///1st move so player had to move by 2 tiles
                        {enPassantNeighbour=true;}
             }
             if(!startsFromTop)
             {
                 if(myPos.y==4)///1st move so player had to move by 2 tiles
                        {enPassantNeighbour=true;}
             }


         }


         firstMove=false;
         //didDoubleJump=false;
     }

};

class player{



int color;
bool startsfromTop;
vector<Figure* > pieces;

public:
int team;
Figure* myKing=nullptr;

    player(int t,int c,bool fromTop):team(t),startsfromTop(fromTop),color(c)
    {

        if(startsfromTop)
        {


        chessBoard[1][0].myFigure=new Pawn(0,1,'P',team,true);
        chessBoard[1][1].myFigure=new Pawn(1,1,'P',team,true);
        chessBoard[1][2].myFigure=new Pawn(2,1,'P',team,true);
        chessBoard[1][3].myFigure=new Pawn(3,1,'P',team,true);
        chessBoard[1][4].myFigure=new Pawn(4,1,'P',team,true);
        chessBoard[1][5].myFigure=new Pawn(5,1,'P',team,true);
        chessBoard[1][6].myFigure=new Pawn(6,1,'P',team,true);
        chessBoard[1][7].myFigure=new Pawn(7,1,'P',team,true);

        chessBoard[0][0].myFigure=new Tower(0,0,'R',team);
        chessBoard[0][7].myFigure=new Tower(7,0,'R',team);

        chessBoard[0][1].myFigure=new Knight(1,0,'K',team);
        chessBoard[0][6].myFigure=new Knight(6,0,'K',team);

        chessBoard[0][2].myFigure=new Bishop(2,0,'B',team);
        chessBoard[0][5].myFigure=new Bishop(5,0,'B',team);

        chessBoard[0][3].myFigure=new Queen(3,0,'Q',team);
        chessBoard[0][4].myFigure=new King(4,0,'W',team);

        myKing=chessBoard[0][4].myFigure;
        }
        else
        {


        chessBoard[6][0].myFigure=new Pawn(0,6,'P',team,false);
        chessBoard[6][1].myFigure=new Pawn(1,6,'P',team,false);
        chessBoard[6][2].myFigure=new Pawn(2,6,'P',team,false);
        chessBoard[6][3].myFigure=new Pawn(3,6,'P',team,false);
        chessBoard[6][4].myFigure=new Pawn(4,6,'P',team,false);
        chessBoard[6][5].myFigure=new Pawn(5,6,'P',team,false);
        chessBoard[6][6].myFigure=new Pawn(6,6,'P',team,false);
        chessBoard[6][7].myFigure=new Pawn(7,6,'P',team,false);

        chessBoard[7][0].myFigure=new Tower(0,7,'R',team);
        chessBoard[7][7].myFigure=new Tower(7,7,'R',team);

        chessBoard[7][1].myFigure=new Knight(1,7,'K',team);
        chessBoard[7][6].myFigure=new Knight(6,7,'K',team);

        chessBoard[7][2].myFigure=new Bishop(2,7,'B',team);
        chessBoard[7][5].myFigure=new Bishop(5,7,'B',team);

        chessBoard[7][3].myFigure=new Queen(3,7,'Q',team);
        chessBoard[7][4].myFigure=new King(4,7,'W',team);

        myKing=chessBoard[7][4].myFigure;

        }


    }


    void playersTurn()
    {
        ///1st select piece
        ///2nd move it or anul and go back to 1st element
        ///3rd if a piece moved then end this function
        MoveStatus status=BAD;

        while(status!=GOOD)
        {


        Figure* selectedFigure=nullptr;

        while(selectedFigure==nullptr||(selectedFigure!=nullptr&&selectedFigure->team!=team))
        {
            Point p;
            system("cls");
            cout<<"Current player: "<<team<<endl<<endl;

            displayChessboard();
            cout<<endl;

            cout<<"Which piece to move (x and y)"<<endl;
            cin>>p.x>>p.y;

            if(p.x>7||p.x<0||p.y<0||p.y>7)continue;

            selectedFigure=chessBoard[p.y][p.x].myFigure;

        }

        status=selectedFigure->tryToMove();


        }



        return;
    }



};



class game
{

    ///win condition
    ///current player

    int currentPlayer=0;
    bool winCondition=true;
    player* gracz[2];
public:
    game()
    {
    gracz[0]=new player(0,WHITE,true);
    gracz[1]=new player(1,BLACK,false);
    }
    ~game()
    {
        delete [] gracz;
    }

    void start()
    {
        while(!GAME_END)
        {

        //gracz[currentPlayer]->playersTurn();
        gracz[currentPlayer]->playersTurn();






        ///EN PASSANT FOR PLAYER STARTING FROM THE TOP
        if(currentPlayer%2==0)
        {
            ///DESTROY ACCORDING TO EN PASSANT RULES
         for(int i=0;i<=7;i++)
        {
        if(chessBoard[5][i].myFigure!=nullptr&&chessBoard[4][i].myFigure!=nullptr&&
               chessBoard[5][i].myFigure->team!=chessBoard[4][i].myFigure->team&&
               chessBoard[4][i].myFigure->display=='P'&&
               chessBoard[5][i].myFigure->display=='P'&&
               chessBoard[4][i].myFigure->enPassantNeighbour
               )
            {
                delete chessBoard[4][i].myFigure;
                chessBoard[4][i].myFigure=nullptr;
            }
        }
        ///SET YOUR ENEMY's EN PASSANT TO FALSE
        for(int i=0;i<=7;i++)
        {
        if(chessBoard[4][i].myFigure!=nullptr&&chessBoard[4][i].myFigure->team!=currentPlayer&&chessBoard[4][i].myFigure->display=='P')
            chessBoard[4][i].myFigure->enPassantNeighbour=false;
        if(chessBoard[3][i].myFigure!=nullptr&&chessBoard[3][i].myFigure->team!=currentPlayer&&chessBoard[3][i].myFigure->display=='P')
            chessBoard[3][i].myFigure->enPassantNeighbour=false;
        }

        }///if from the top
        else ///from the bottom
        {
            for(int i=0;i<=7;i++)
        {
            if(chessBoard[2][i].myFigure!=nullptr&&chessBoard[3][i].myFigure!=nullptr&&
               chessBoard[2][i].myFigure->team!=chessBoard[3][i].myFigure->team&&
               chessBoard[2][i].myFigure->display=='P'&&
               chessBoard[3][i].myFigure->display=='P'&&
               chessBoard[3][i].myFigure->enPassantNeighbour
               )
            {
                delete chessBoard[3][i].myFigure;
                chessBoard[3][i].myFigure=nullptr;
            }
        }///for

        for(int i=0;i<=7;i++)
        {
        if(chessBoard[4][i].myFigure!=nullptr&&chessBoard[4][i].myFigure->team!=currentPlayer&&chessBoard[4][i].myFigure->display=='P')
            chessBoard[4][i].myFigure->enPassantNeighbour=false;
        if(chessBoard[3][i].myFigure!=nullptr&&chessBoard[3][i].myFigure->team!=currentPlayer&&chessBoard[3][i].myFigure->display=='P')
            chessBoard[3][i].myFigure->enPassantNeighbour=false;
        }

        }///else


        ///We will transform any pawn that has reached last tile into a Queen
        ///its a bit inefficient but dunno how to do it in any other way

        for(int i=0;i<=7;i++)
        {
            ///TOP
            Figure* currentFigure=chessBoard[0][i].myFigure;
            if(currentFigure!=nullptr&&currentFigure->display=='P')
            {
                int t=currentFigure->team;
                delete currentFigure;
                currentFigure=nullptr;

                chessBoard[0][i].myFigure=new Queen(i,0,'Q',t);
            }
            ///BOTTOM
            currentFigure=chessBoard[7][i].myFigure;
            if(currentFigure!=nullptr&&currentFigure->display=='P')
            {
                int t=currentFigure->team;
                delete currentFigure;
                currentFigure=nullptr;

                chessBoard[7][i].myFigure=new Queen(i,7,'Q',t);
            }

        }

        ///CHECK FOR EN PASSANT IN ROWS 2-3 5-4
        /*
        for(int i=0;i<=7;i++)
        {
            if(chessBoard[2][i].myFigure!=nullptr&&chessBoard[3][i].myFigure!=nullptr&&
               chessBoard[2][i].myFigure->team!=chessBoard[3][i].myFigure->team&&
               chessBoard[2][i].myFigure->display=='P'&&
               chessBoard[3][i].myFigure->display=='P'&&
               chessBoard[3][i].myFigure->enPassantNeighbour
               )
            {
                delete chessBoard[3][i].myFigure;
                chessBoard[3][i].myFigure=nullptr;
            }

            else if(chessBoard[5][i].myFigure!=nullptr&&chessBoard[4][i].myFigure!=nullptr&&
               chessBoard[5][i].myFigure->team!=chessBoard[4][i].myFigure->team&&
               chessBoard[4][i].myFigure->display=='P'&&
               chessBoard[5][i].myFigure->display=='P'&&
               chessBoard[4][i].myFigure->enPassantNeighbour
               )
            {
                delete chessBoard[4][i].myFigure;
                chessBoard[4][i].myFigure=nullptr;
            }


        }
*/

        ///NOW IF ANY PAWN HAS EN PASSANT FLAG SET TO TRUE WE HAVE TO SET IT TO FALSE

  if(currentPlayer==1)currentPlayer=0;
        else currentPlayer=1;

        }///while - main game loop





    }

    bool checkForMat()
    {
        if(gracz[0]->myKing==nullptr)
        {  ///Gracz 0 wygrywa
           cout<<"GRACZ "<<gracz[1]->team<<" wygrywa"<<endl;  return true;
        }
        if(gracz[1]->myKing==nullptr)
        {
            cout<<"GRACZ "<<gracz[0]->team<<" wygrywa"<<endl;  return true;
        }
        return false;
    }


};



int main()
{
/*
chessBoard[0][4].myFigure=new Tower(4,0,'T',0);
chessBoard[2][3].myFigure=new Pawn(3,2,'P',1,true);
chessBoard[7][4].myFigure=new King(4,7,'A',1);
chessBoard[5][0].myFigure=new Knight(0,5,'K',1);
chessBoard[5][5].myFigure=new King(5,5,'W',1);
chessBoard[5][0].myFigure->tryToMove();
*/


//player p1(0,WHITE,true);
//player p2(1,BLACK,false);
//p2.playersTurn();


//chessBoard[1][1].myFigure=new Pawn(1,1,'P',1,true);
//chessBoard[1][1].myFigure->tryToMove();

game g;
g.start();

    return 0;
}



void displayChessboard()
{
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for(int i=0;i<8;i++)
        {
            if(i==0)cout<<" x  0 1 2 3 4 5 6 7"<<endl;
            SetConsoleTextAttribute(hConsole,10);
            if(i==0)cout<<'y'<<endl;
            cout<<endl<<i<<"   ";
            for(int j=0;j<8;j++)
            {


                if(chessBoard[i][j].myFigure==nullptr){SetConsoleTextAttribute(hConsole,10);cout<<'0'<<' ';}
                else
                {

                 if(chessBoard[i][j].myFigure->team==0)   SetConsoleTextAttribute(hConsole,4);
                    else SetConsoleTextAttribute(hConsole,2);

                cout<<chessBoard[i][j].myFigure->display<<' ';

                }
            }
            if(i==1)cout<<"  Player 0";
            if(i==6)cout<<"  Player 1";
            cout<<endl;
        }
SetConsoleTextAttribute(hConsole,10);

}





