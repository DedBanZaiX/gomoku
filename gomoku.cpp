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
            std::cout << "������ (" << i << ", " << j << ") ��� ������.\n";
         }
      }
      else
      {
         std::cout << "������������ ����������: (" << i << ", " << j << ")\n";
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
               std::cout << "o|";         // ����� ������
            }
            else if (field[i][j] == 2)
            {
               std::cout << "x|";         // ����� ����������
            }
            else
            {
               std::cout << " |";         // ������ ������
            }
         }
         std::cout << "\n";
      }
   }

   int Gomoku::Evaluation() const
   {
      //std::cout << "\n���������� ������ ������� ��� ����������:\n";

      int evalComputer = Evaluation_Function(field, 2, 1);              // ������ ��� ����������

      /*std::cout << "������ ����������: " << evalComputer << "\n";

      std::cout << "���������� ������ ������� ��� ������:\n";*/

      int evalPlayer = Evaluation_Function(field, 1, 2);                // ������ ��� ������

      /*std::cout << "������ ������: " << evalPlayer << "\n";

      std::cout << "����� ������ ������� (��������� - �����): "
         << (evalComputer - evalPlayer) << "\n\n";*/

      return evalComputer - evalPlayer;                                 // ������� ������
   }

   int Gomoku::evaluate_Line(const int* line, int size, int player, bool fork)
   {
      int sum = 0;
      int streak = 0;                                                     // ���-�� ������� ������ ����� (�����)
      bool prev_empty = fork;
      bool post_empty = false;


      for (int i = 0; i < size; ++i)
      {
         if (fork && line[0] != player)                                   // �������� �����, �.�. ���� ����� ������ ������ ������ ����� ������
            return sum;

         if (line[i] == player)
         {
            if (streak == 0)
            {
               prev_empty = (fork || (i > 0 && line[i - 1] == 0));        // ��������� ������ ������ ����� ������
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
               post_empty = (line[i] == 0);                               // ��������� ������ ������ ����� �����
               current_sum = point(streak, prev_empty, post_empty);       // ��������� ������� �����
               if (post_empty && size - i > 1 && !fork )
               {
                  fork_sum = evaluate_Line(&line[i + 1], size - (i + 1), player, post_empty);     // ��������� ������� �����
               }
               if (fork_sum != 0)
               {
                  int multiplier = 15;                                     // ��������� ��� ������ ����� �����
                  if (current_sum + fork_sum < 110)                        // 100
                     multiplier = 50;
                  else if (current_sum + fork_sum < 260)                   // 250
                     multiplier = 30;
                  sum += (current_sum + fork_sum) * multiplier;            // ��������� ������� ����� c ������ �����
               }
               else
                  sum += current_sum;
               streak = 0;
               prev_empty = false;
               post_empty = false;
            }
         }
      }


      // ��������� ����� � ����� �����
      if (streak > 0)
         sum += point(streak, prev_empty, false);                          // ��������� ��������� �����

      return sum;
   }

   int Gomoku::Evaluation_Function(const int field[FIELD_SIZE][FIELD_SIZE], int this_player, int enemy_player)
   {
      int sum = 0;

                                                                        // ��������� �����
      //std::cout << "��������� �����:\n";

      for (int i = 0; i < FIELD_SIZE; ++i)
      {
         int row[FIELD_SIZE]{ 0 };
         memcpy(row, field[i], FIELD_SIZE * sizeof(field[i][0]));        // �������� ������
         
         //std::cout << "������ " << i << ": ";
         //for (int j = 0; j < FIELD_SIZE; ++j)
         //{
         //   std::cout << row[j] << " ";                                // ������� ������
         //}

         int lineSum = evaluate_Line(row, FIELD_SIZE, this_player);      // ��������� ������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
      }

                                                                         // ��������� ��������
      //std::cout << "\n��������� ��������:\n";
      
      for (int j = 0; j < FIELD_SIZE; ++j)
      {
         int column[FIELD_SIZE]{ 0 };
         for (int i = 0; i < FIELD_SIZE; ++i)
         {
            column[i] = field[i][j];                                     // �������� �������
         }

         //std::cout << "������� " << j << ": ";
         //for (int i = 0; i < FIELD_SIZE; ++i)
         //{
         //   std::cout << column[i] << " ";                             // ������� �������
         //}
         
         int lineSum = evaluate_Line(column, FIELD_SIZE, this_player);   // ��������� �������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
      }


                                                                         // ��������� ���������� (����� ���� -> ������ ���)
      //std::cout << "\n��������� ���������� (����� ���� -> ������ ���):\n";
      
      int count{ 0 };
      for (int n = 0; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};

         //std::cout << "��������� " << ++count << ": ";
         
         for (int i = n, j = 0; i < FIELD_SIZE && j < size; ++i, ++j)
         {
            diagonal[j] = field[i][j];
            
            //std::cout << diagonal[j] << " ";                              // ������� ���������
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // ��������� ���������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }
      for (int n = 1; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "��������� " << ++count << ": ";
         
         for (int j = n, i = 0; j < FIELD_SIZE && i < size; ++i, ++j)
         {
            diagonal[i] = field[i][j];
            
            //std::cout << diagonal[i] << " ";                              // ������� ���������
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // ��������� ���������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }

                                                                         // ��������� ���������� (����� ��� -> ������ ����)
      //std::cout << "\n��������� ���������� (����� ��� -> ������ ����):\n";
      count = 0;
      for (int n = 0; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "��������� " << ++count << ": ";
         for (int i = FIELD_SIZE - 1 - n, j = 0; i > 0 && j < FIELD_SIZE - n; --i, ++j)
         {
            diagonal[j] = field[i][j];
            
            //std::cout << diagonal[j] << " ";                              // ������� ���������
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // ��������� ���������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }
      for (int n = 1; n < FIELD_SIZE; ++n)
      {
         int size = FIELD_SIZE - n;
         int* diagonal = new int[size] {0};
         
         //std::cout << "��������� " << ++count << ": ";
         
         for (int i = FIELD_SIZE - 1, j = n; j < FIELD_SIZE && i > 0 + n; --i, ++j)
         {
            diagonal[FIELD_SIZE - i] = field[i][j];
            
            //std::cout << diagonal[FIELD_SIZE - i] << " ";                 // ������� ���������
         }
         int lineSum = evaluate_Line(diagonal, size, this_player);        // ��������� ���������
         sum += lineSum;
         
         //std::cout << "| : " << lineSum << "  | : " << sum << "\n";
         
         delete[] diagonal;
      }

      //std::cout << "����� ����� ������: " << sum << "\n\n";

      return sum;
   }

   int Gomoku::point(int streak, bool prev_empty, bool post_empty)
   {
      if (streak >= 5)
      {
         return 10000000;                                                  // ������
      }
      else if (streak == 4)
      {
         if (prev_empty && post_empty)
            return 10000;                                                // �������� ��������
         else if (prev_empty || post_empty)
            return 5000;                                                // ������������ ��������
         else
            return 0;                                                   // �������� ��������
      }
      else if (streak == 3)
      {
         if (prev_empty && post_empty)
            return 500;                                                 // �������� ������
         else if (prev_empty || post_empty)
            return 100;                                                 // ������������ ������
         else
            return 0;                                                   // �������� ������
      }
      else if (streak == 2)
      {
         if (prev_empty && post_empty)
            return 50;                                                  // �������� ������
         else if (prev_empty || post_empty)
            return 10;                                                  // ������������ ������
         else
            return 0;                                                   // �������� ������
      }
      else if (streak == 1)
      {
         if (prev_empty && post_empty)
            return 2;                                                   // �������� �������
         else if (prev_empty || post_empty)
            return 1;                                                   // ������������ �������
         else
            return 0;                                                   // �������� �������
      }
      else
      {
         return 0;                                                      // ��� �������� ����������
      }
   }

   bool Gomoku::checkSurroundingPieces(int row, int col)    // ���� � ���-� ������� �� ������������� �����
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
            checkSurroundingPieces(cur_pos.row, cur_pos.col))         // �������� �� ������ ������
         {
            field[cur_pos.row][cur_pos.col] = currentPlayer;          // ��������� ��� 

            int score = Evaluation();                                 // ������ �������� ��������� �����
            
            if (depth == 0 || score > 5000000 || score < -5000000)    // ������� ��� �������� ��� ���� �������
            {
               field[cur_pos.row][cur_pos.col] = EMPTY;               // �������� ���
               return bestValue{ cur_pos, score };
            }

            if (isMaximizingPlayer)
            {
               if (score > 5000000 && depth == start_depth)           // ������� �� ������ ����, ���������� ����������
               {
                  field[cur_pos.row][cur_pos.col] = EMPTY;            // �������� ���
                  return bestValue{ cur_pos, score };
               }

               bestValue new_Value = std::max( Best_Value, 
                  Mini_Max(depth - 1, start_depth, alpha, beta, false));
               if (new_Value != Best_Value)
               {
                  Best_Value = new_Value;
                  Best_Value.coords = cur_pos;
               }

               alpha = std::max(alpha, Best_Value.value);              // �������� alpha
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
               beta = std::min(beta, Best_Value.value);                // �������� beta
               
               if (beta <= alpha)
               {
                  field[cur_pos.row][cur_pos.col] = EMPTY;             // �������� ���
                  return Best_Value;                                   // ���������           
               }
            }

            /*std::system("cls");

            std::cout << "\n������� ��������� �������� ����:\n";
            print_Field();*/

            field[cur_pos.row][cur_pos.col] = EMPTY;                   // �������� ���
         }
      }
      return Best_Value;
   }

   Coords Gomoku::findBestMove()
   {
      int depth = 4;
      Coords BestMove = Mini_Max(depth, depth, INT_MIN, INT_MAX, true).coords;

      if (BestMove.col != -1 && BestMove.row != -1)          // ������������� ������ ��� �� ����
      {
         set_Piece(BestMove.row, BestMove.col, COMPUTER);
      }
      else
      {
         std::cout << "������ ���������� ������� ����" << std::endl;
      }
      return BestMove;
   }
   
   void Gomoku::generateSpiralOrder(int fieldSize)
   {
      int left = 0, right = fieldSize - 1;
      int top = 0, bottom = fieldSize - 1;

      while (left <= right && top <= bottom) 
      {
         
         for (int i = left; i <= right; i++)                   // ������� ���
         {
            if (field[top][i] == 0)
            {
               spiral_Order.push_front(Coords{ top, i });
            }
         }
         top++;
        
         for (int i = top; i <= bottom; i++)                   // ������ �������
         {
            if (field[i][right] == 0)
            {
               spiral_Order.push_front(Coords{ i, right });
            }
         }
         right--;
         
         if (top <= bottom)                                    // ������ ��� (���� ��� �� ������)
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
         
         if (left <= right)                                    // ����� ������� (���� ��� �� ������)
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