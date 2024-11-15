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
   while (true)
   {
      std::system("cls");

      evaluation = game.Evaluation();
      
      std::cout << "\nТекущее состояние игрового поля:\n";
      game.print_Field();
      std::cout << "\nОценка позиции: " << evaluation << std::endl;

      if (evaluation > 0)
      {
         std::cout << "Преимущество компьютера.\n";
      }
      else if (evaluation < 0)
      {
         std::cout << "Преимущество игрока.\n";
      }
      else
      {
         std::cout << "Позиция сбалансирована.\n";
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
      std::cout << "Ваш ход, укажите координаты\n" << "строка : ";
      std::cin >> player_coord.row;
      std::cout << "столбец : ";
      std::cin >> player_coord.col;
      game.set_Piece(player_coord.row, player_coord.col, 1);

      std::system("cls");

      std::cout << "\nТекущее состояние игрового поля:\n";
      game.print_Field();
      std::cout << "\nОценка позиции: " << evaluation << std::endl;

      if (evaluation > 0)
      {
         std::cout << "Преимущество компьютера.\n";
      }
      else if (evaluation < 0)
      {
         std::cout << "Преимущество игрока.\n";
      }
      else
      {
         std::cout << "Позиция сбалансирована.\n";
      }
      if (evaluation > 9000000 || evaluation < -9000000)
      {
         if (evaluation > 0)
         {
            std::cout << "GAME OVER\n";
         }
         break;
      }

      game.findBestMove();
   }








   /*                                             // Устанавливаем фишки игрока (1)
   game.set_Piece(5, 5, 1);
   game.set_Piece(6, 6, 1);
   game.set_Piece(7, 7, 1);
   game.set_Piece(5, 3, 1);
   game.set_Piece(5, 6, 1);
   game.set_Piece(7, 5, 1);
   game.set_Piece(1, 3, 1);
   game.set_Piece(3, 5, 1);
                                                 // Устанавливаем фишки компьютера (2)
   game.set_Piece(4, 5, 2);
   game.set_Piece(5, 4, 2);
   game.set_Piece(5, 7, 2);
   game.set_Piece(6, 5, 2);
   game.set_Piece(6, 7, 2);
   game.set_Piece(6, 4, 2);
   game.set_Piece(0, 0, 2);
   game.set_Piece(0, 1, 2);

   game.generateSpiralOrder(FIELD_SIZE);

   std::cout << "\nТекущее состояние игрового поля:\n";
   game.print_Field();

   int evaluation = game.Evaluation();

   std::cout << "\nОценка позиции: " << evaluation << std::endl;

   if (evaluation > 0)
   {
      std::cout << "Преимущество компьютера.\n";
   }
   else if (evaluation < 0)
   {
      std::cout << "Преимущество игрока.\n";
   }
   else
   {
      std::cout << "Позиция сбалансирована.\n";
   }

   game.findBestMove();

   std::cout << "\nТекущее состояние игрового поля:\n";
   game.print_Field();

   evaluation = game.Evaluation();

   std::cout << "\nОценка позиции: " << evaluation << std::endl;

   if (evaluation > 0)
   {
      std::cout << "Преимущество компьютера.\n";
   }
   else if (evaluation < 0)
   {
      std::cout << "Преимущество игрока.\n";
   }
   else
   {
      std::cout << "Позиция сбалансирована.\n";
   }


   game.set_Piece(8, 8, 1);

   std::cout << "\nТекущее состояние игрового поля:\n";
   game.print_Field();

   evaluation = game.Evaluation();

   std::cout << "\nОценка позиции: " << evaluation << std::endl;

   if (evaluation > 0)
   {
      std::cout << "Преимущество компьютера.\n";
   }
   else if (evaluation < 0)
   {
      std::cout << "Преимущество игрока.\n";
   }
   else
   {
      std::cout << "Позиция сбалансирована.\n";
   }


   game.findBestMove();

   std::cout << "\nТекущее состояние игрового поля:\n";
   game.print_Field();

   evaluation = game.Evaluation();

   std::cout << "\nОценка позиции: " << evaluation << std::endl;

   if (evaluation > 0)
   {
      std::cout << "Преимущество компьютера.\n";
   }
   else if (evaluation < 0)
   {
      std::cout << "Преимущество игрока.\n";
   }
   else
   {
      std::cout << "Позиция сбалансирована.\n";
   }
   */
   return 0;
}
