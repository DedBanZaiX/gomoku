#pragma once
#include <iostream>
#include "..\alg_lab_1\List.h"
#define FIELD_SIZE 10

namespace asd_mvv
{
   enum CellState
   {
      EMPTY = 0,     // ������ ����
      PLAYER = 1,    // ���� ������
      COMPUTER = 2   // ���� ����������
   };

   struct Coords
   {
      int row;
      int col;
   };

   struct bestValue
   {
      Coords coords{ -1, -1};
      int value;
      bool operator<(const bestValue& other) const 
      {
         return value < other.value;
      }
      bool operator>(const bestValue& other) const 
      {
         return value > other.value;
      }
      bool operator==(const bestValue& other) const 
      {
         return value == other.value;
      }
      bool operator!=(const bestValue& other) const
      {
         return value != other.value;
      }
   };

   class Gomoku
   {
   public:
      int field[FIELD_SIZE][FIELD_SIZE];
      asd_mvv::List<Coords> occupied_Coords;
      asd_mvv::List<Coords> spiral_Order;

      Gomoku() : field{ 0 } {}                  // ����������� ��� ������������� ����
      void set_Piece(int i, int j, int player); // ����� ��� ��������� ����� �� ����
      void print_Field() const;                 // ����� ��� ����������� ���� �� ������                   
      int Evaluation() const;                   // ��������� �������
      // int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer);    // �������� �������
      // int findBestMove();                       // ������� ��� ���������� ������� ����

      bestValue minimax(int depth, int start_depth,
         int alpha, int beta, bool isMaximizingPlayer);    // �������� �������
      void findBestMove();                       // ������� ��� ���������� ������� ����

      void generateSpiralOrder(int fieldSize);

   private:
                                                // ������� ��� ������ ���������� ����� (������, ������� ��� ���������)
      static int evaluate_Line(const int* line, int size, int player, bool fork = false);
                                                // ������� ��� ������ ������� ����������� ������
      static int Evaluation_Function(const int field[FIELD_SIZE][FIELD_SIZE],
         int this_player, int enemy_player);
                                                // ������� ��� ������ ����������
      static int point(int streak, bool prev_empty, bool post_empty);
                                                // ���������� �������� ������ ��������� ������ ������
      bool checkSurroundingPieces(int row, int col);

   };

}
