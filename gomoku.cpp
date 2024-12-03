#include "gomoku.h"

namespace asd_mvv
{
   void Gomoku::set_Piece(int i, int j, int player)
   {
      if (i >= 0 && i < FIELD_SIZE && j >= 0 && j < FIELD_SIZE)
      {
         if (field[i][j] == 0)
         {
            field[i][j] = player;
            occupied_Coords.push_back(Coords{ i ,j });
         }
         else
         {
            std::cout << "Клетка (" << i << ", " << j << ") уже занята.\n";
         }
      }
      else
      {
         std::cout << "Некорректные координаты: (" << i << ", " << j << ")\n";
      }
   }

   void Gomoku::print_Field() const
   {
      std::cout << "    ";
      for (int j = 0; j < FIELD_SIZE; ++j)
      {
         std::cout << j << " ";
      }
      std::cout << std::endl;

      for (int i = 0; i < FIELD_SIZE; ++i)
      {
         std::cout << " " << i << " |";
         for (int j = 0; j < FIELD_SIZE; ++j)
         {
            if (field[i][j] == 1)
            {
               std::cout << "o|";         // Фишка игрока
            }
            else if (field[i][j] == 2)
            {
               std::cout << "x|";         // Фишка компьютера
            }
            else
            {
               std::cout << " |";         // Пустая клетка
            }
         }
         std::cout << "\n";
      }
   }

   int Gomoku::Evaluation() const
   {
      //std::cout << "\nВычисление оценки позиции для компьютера:\n";

      int evalComputer = Evaluation_Function(field, 2, 1);              // Оценка для компьютера

      /*std::cout << "Оценка компьютера: " << evalComputer << "\n";

      std::cout << "Вычисление оценки позиции для игрока:\n";*/

      int evalPlayer = Evaluation_Function(field, 1, 2);                // Оценка для игрока

      /*std::cout << "Оценка игрока: " << evalPlayer << "\n";

      std::cout << "Общая оценка позиции (Компьютер - Игрок): "
         << (evalComputer - evalPlayer) << "\n\n";*/

      return evalComputer - evalPlayer;                                 // Разница оценок
   }

   int Gomoku::evaluate_Line(const int* line, int size, int player, bool fork)
   {
      int sum = 0;
      int streak = 0;                                                     // кол-во стоящих подряд фишек (серия)
      bool prev_empty = fork;
      bool post_empty = false;


      for (int i = 0; i < size; ++i)
      {
         if (fork && line[0] != player)                                   // проверка вилки, т.е. если через клетку должна стоять фишка игрока
            return sum;

         if (line[i] == player)
         {
            if (streak == 0)
            {
               prev_empty = (fork || (i > 0 && line[i - 1] == 0));        // Проверяем пустую клетку перед серией
               streak = 1;
            }
            else
               streak++;
         }
         else
         {
            if (streak > 0)
            {
               int current_sum = 0, fork_sum = 0;
               post_empty = (line[i] == 0);                               // Проверяем пустую клетку после серии
               current_sum = point(streak, prev_empty, post_empty);       // Оцениваем текущую серию
               if (post_empty && size - i > 1 && !fork )
               {
                  fork_sum = evaluate_Line(&line[i + 1], size - (i + 1), player, post_empty);     // Проверяем наличие вилки
               }
               if (fork_sum != 0)
               {
                  int multiplier = 15;                                     // Множитель для разных типов вилок
                  if (current_sum + fork_sum < 110)                        // 100
                     multiplier = 50;
                  else if (current_sum + fork_sum < 260)                   // 250
                     multiplier = 30;
                  sum += (current_sum + fork_sum) * multiplier;            // Оцениваем текущую серию c учетом вилки
               }
               else
                  sum += current_sum;
               streak = 0;
               prev_empty = false;
               post_empty = false;
            }
         }
      }


      // Обработка серии в конце линии
      if (streak > 0)
         sum += point(streak, prev_empty, false);                          // Оцениваем последнюю серию

      return sum;
   }

   int Gomoku::Evaluation_Function(const int field[FIELD_SIZE][FIELD_SIZE], int this_player, int enemy_player)
   {
      int sum = 0;

                                                                        // Обработка строк
      //std::cout << "Обработка строк:\n";

      for (int i = 0; i < FIELD_SIZE; ++i)
      {
         int row[FIELD_SIZE]{ 0 };
         memcpy(row, field[i], FIELD_SIZE * sizeof(field[i][0]));        // Копируем строку
         
         //std::cout << "Строка " << i << ": ";
         //for (int j = 0; j < FIELD_SIZE; ++j)
         //{
         //   std::cout << row[j] << " ";                                // Выводим строку
         //}

         int lineSum = evaluate_Line(row, FIELD_SIZE, this_player);      // Оцениваем строку
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
      }

                                                                         // Обработка столбцов
      //std::cout << "\nОбработка столбцов:\n";
      
      for (int j = 0; j < FIELD_SIZE; ++j)
      {
         int column[FIELD_SIZE]{ 0 };
         for (int i = 0; i < FIELD_SIZE; ++i)
         {
            column[i] = field[i][j];                                     // Копируем столбец
         }

         //std::cout << "Столбец " << j << ": ";
         //for (int i = 0; i < FIELD_SIZE; ++i)
         //{
         //   std::cout << column[i] << " ";                             // Выводим столбец
         //}
         
         int lineSum = evaluate_Line(column, FIELD_SIZE, this_player);   // Оцениваем столбец
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
      }


                                                                         // Обработка диагоналей (левый верх -> правый низ)
      //std::cout << "\nОбработка диагоналей (левый верх -> правый низ):\n";
      
      int count{ 0 };
      for (int n = 0; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};

         //std::cout << "Диагональ " << ++count << ": ";
         
         for (int i = n, j = 0; i < FIELD_SIZE && j < size; ++i, ++j)
         {
            diagonal[j] = field[i][j];
            
            //std::cout << diagonal[j] << " ";                              // Выводим диагональ
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // Оцениваем диагональ
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }
      for (int n = 1; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "Диагональ " << ++count << ": ";
         
         for (int j = n, i = 0; j < FIELD_SIZE && i < size; ++i, ++j)
         {
            diagonal[i] = field[i][j];
            
            //std::cout << diagonal[i] << " ";                              // Выводим диагональ
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // Оцениваем диагональ
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }

                                                                         // Обработка диагоналей (левый низ -> правый верх)
      //std::cout << "\nОбработка диагоналей (левый низ -> правый верх):\n";
      count = 0;
      for (int n = 0; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "Диагональ " << ++count << ": ";
         for (int i = FIELD_SIZE - 1 - n, j = 0; i > 0 && j < FIELD_SIZE - n; --i, ++j)
         {
            diagonal[j] = field[i][j];
            
            //std::cout << diagonal[j] << " ";                              // Выводим диагональ
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // Оцениваем диагональ
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }
      for (int n = 1; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "Диагональ " << ++count << ": ";
         
         for (int i = FIELD_SIZE - 1, j = n; j < FIELD_SIZE && i > 0 + n; --i, ++j)
         {
            diagonal[FIELD_SIZE - i] = field[i][j];
            
            //std::cout << diagonal[FIELD_SIZE - i] << " ";                 // Выводим диагональ
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // Оцениваем диагональ
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }

      //std::cout << "Общая сумма оценки: " << sum << "\n\n";

      return sum;
   }

   int Gomoku::point(int streak, bool prev_empty, bool post_empty)
   {
      if (streak >= 5)
      {
         return 10000000;                                                  // Победа
      }
      else if (streak == 4)
      {
         if (prev_empty && post_empty)
            return 10000;                                                // Открытая четверка
         else if (prev_empty || post_empty)
            return 5000;                                                // Полуоткрытая четверка
         else
            return 0;                                                   // Закрытая четверка
      }
      else if (streak == 3)
      {
         if (prev_empty && post_empty)
            return 500;                                                 // Открытая тройка
         else if (prev_empty || post_empty)
            return 100;                                                 // Полуоткрытая тройка
         else
            return 0;                                                   // Закрытая тройка
      }
      else if (streak == 2)
      {
         if (prev_empty && post_empty)
            return 50;                                                  // Открытая двойка
         else if (prev_empty || post_empty)
            return 10;                                                  // Полуоткрытая двойка
         else
            return 0;                                                   // Закрытая двойка
      }
      else if (streak == 1)
      {
         if (prev_empty && post_empty)
            return 2;                                                   // Открытая единица
         else if (prev_empty || post_empty)
            return 1;                                                   // Полуоткрытая единица
         else
            return 0;                                                   // Закрытая единица
      }
      else
      {
         return 0;                                                      // Нет значимой комбинации
      }
   }

   bool Gomoku::checkSurroundingPieces(int row, int col)    // ходы в опр-м радиусе от установленных фишек
   {
      int distance = 2;
      for (Coords k : occupied_Coords)
      {
         if (std::abs(row - k.row) < distance && std::abs(col - k.col) < distance)
         {
            return true;
         }
      }
      return false;
   }

   bestValue Gomoku::Mini_Max(int depth, int start_depth, int alpha, int beta, bool isMaximizingPlayer)
   {
      bestValue Best_Value{ Coords{-1, -1}, 0 };
      CellState currentPlayer = EMPTY;

      if (isMaximizingPlayer)
      {
         currentPlayer = COMPUTER;
         Best_Value.value = INT_MIN;
      }
      else
      {
         currentPlayer = PLAYER;
         Best_Value.value = INT_MAX;
      }

      for (const Coords& cur_pos : spiral_Order)
      {
         if (field[cur_pos.row][cur_pos.col] == 0 && 
            checkSurroundingPieces(cur_pos.row, cur_pos.col))         // Проверка на пустую клетку
         {
            field[cur_pos.row][cur_pos.col] = currentPlayer;          // Возможный ход 

            int score = Evaluation();                                 // Оценка текущего состояние доски
            
            if (depth == 0 || score > 5000000 || score < -5000000)    // выигрыш или проигрыш или макс глубина
            {
               field[cur_pos.row][cur_pos.col] = EMPTY;               // Отменить ход
               return bestValue{ cur_pos, score };
            }

            if (isMaximizingPlayer)
            {
               if (score > 5000000 && depth == start_depth)           // выигрыш на первом ходу, возвращает координаты
               {
                  field[cur_pos.row][cur_pos.col] = EMPTY;            // Отменить ход
                  return bestValue{ cur_pos, score };
               }

               bestValue new_Value = std::max( Best_Value, 
                  Mini_Max(depth - 1, start_depth, alpha, beta, false));
               if (new_Value != Best_Value)
               {
                  Best_Value = new_Value;
                  Best_Value.coords = cur_pos;
               }

               alpha = std::max(alpha, Best_Value.value);              // Обновить alpha
            }
            else
            {
               bestValue new_Value = std::min(Best_Value, 
                  Mini_Max(depth - 1, start_depth, alpha, beta, true));
               if (new_Value != Best_Value)
               {
                  Best_Value = new_Value;
                  Best_Value.coords = cur_pos;
               }
               beta = std::min(beta, Best_Value.value);                // Обновить beta
               
               if (beta <= alpha)
               {
                  field[cur_pos.row][cur_pos.col] = EMPTY;             // Отменить ход
                  return Best_Value;                                   // Отсечение           
               }
            }

            /*std::system("cls");

            std::cout << "\nТекущее состояние игрового поля:\n";
            print_Field();*/

            field[cur_pos.row][cur_pos.col] = EMPTY;                   // Отменить ход
         }
      }
      return Best_Value;
   }

   Coords Gomoku::findBestMove()
   {
      int depth = 4;
      Coords BestMove = Mini_Max(depth, depth, INT_MIN, INT_MAX, true).coords;

      if (BestMove.col != -1 && BestMove.row != -1)          // Устанавливаем лучший ход на поле
      {
         set_Piece(BestMove.row, BestMove.col, COMPUTER);
      }
      else
      {
         std::cout << "Ошибка вычисления лучшего хода" << std::endl;
      }
      return BestMove;
   }
   
   void Gomoku::generateSpiralOrder(int fieldSize)
   {
      int left = 0, right = fieldSize - 1;
      int top = 0, bottom = fieldSize - 1;

      while (left <= right && top <= bottom) 
      {
         
         for (int i = left; i <= right; i++)                   // Верхний ряд
         {
            if (field[top][i] == 0)
            {
               spiral_Order.push_front(Coords{ top, i });
            }
         }
         top++;
        
         for (int i = top; i <= bottom; i++)                   // Правый столбец
         {
            if (field[i][right] == 0)
            {
               spiral_Order.push_front(Coords{ i, right });
            }
         }
         right--;
         
         if (top <= bottom)                                    // Нижний ряд (если ещё не обошли)
         {
            for (int i = right; i >= left; i--) 
            {
               if (field[bottom][i] == 0)
               {
                  spiral_Order.push_front(Coords{ bottom, i });
               }
            }
            bottom--;
         }
         
         if (left <= right)                                    // Левый столбец (если ещё не обошли)
         {
            for (int i = bottom; i >= top; i--) 
            {
               if (field[i][left] == 0)
               {
                  spiral_Order.push_front(Coords{ i, left });
               }
            }
            left++;
         }
      }
   }
}