#pragma once
#include <iostream>
#include "..\alg_lab_1\List.h"
#define FIELD_SIZE 10

namespace asd_mvv
{
   enum CellState
   {
      EMPTY = 0,     // Пустое поле
      PLAYER = 1,    // Поле игрока
      COMPUTER = 2   // Поле компьютера
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

      Gomoku() : field{ 0 } {}                  // Конструктор для инициализации поля
      void set_Piece(int i, int j, int player); // Метод для установки фишки на поле
      void print_Field() const;                 // Метод для отображения поля на экране                   
      int Evaluation() const;                   // Оценочная функция
      // int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer);    // Минимакс функция
      // int findBestMove();                       // Функция для нахождения лучшего хода

      bestValue minimax(int depth, int start_depth,
         int alpha, int beta, bool isMaximizingPlayer);    // Минимакс функция
      void findBestMove();                       // Функция для нахождения лучшего хода

      void generateSpiralOrder(int fieldSize);

   private:
                                                // Функция для оценки конкретной линии (строка, столбец или диагональ)
      static int evaluate_Line(const int* line, int size, int player, bool fork = false);
                                                // Функция для оценки позиции конкретного игрока
      static int Evaluation_Function(const int field[FIELD_SIZE][FIELD_SIZE],
         int this_player, int enemy_player);
                                                // Функция для оценки комбинации
      static int point(int streak, bool prev_empty, bool post_empty);
                                                // Исключение проверки далеко отстоящих пустых клеток
      bool checkSurroundingPieces(int row, int col);

   };

}
