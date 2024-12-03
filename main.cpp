#include <iostream>
#include <string>
#include "gomoku.h"



int main()
{
   setlocale(LC_ALL, "Russian");
   asd_mvv::Gomoku game;

   int evaluation = 0;
   game.set_Piece((int)FIELD_SIZE/2,(int)FIELD_SIZE/2, 2);
   game.generateSpiralOrder(FIELD_SIZE);
   asd_mvv::Coords computersMove;
   computersMove.col = computersMove.row = (int)FIELD_SIZE / 2;
   while (true)
   {
      if(game.occupied_Coords.GetSize() != 1) 
         computersMove = game.findBestMove();

      std::system("cls");




      evaluation = game.Evaluation();
      
      std::cout << "\n������� ��������� �������� ����:\n";
      game.print_Field();

      std::cout << "��������� ������ ���: (" << computersMove.row << ", " << computersMove.col << ")\n";

      std::cout << "\n������ �������: " << evaluation << std::endl;

      if (evaluation > 0)
      {
         std::cout << "������������ ����������.\n";
      }
      else if (evaluation < 0)
      {
         std::cout << "������������ ������.\n";
      }
      else
      {
         std::cout << "������� ��������������.\n";
      }
      if (evaluation > 9000000 || evaluation < -9000000)
      {
         if (evaluation > 0)
         {
            std::cout << "GAME OVER\n";
         }
         break;
      }

      asd_mvv::Coords player_coord;
      std::cout << "��� ���, ������� ����������\n" << "������ : ";
      std::cin >> player_coord.row;
      std::cout << "������� : ";
      std::cin >> player_coord.col;
      game.set_Piece(player_coord.row, player_coord.col, 1);

     

   }

   return 0;
}
