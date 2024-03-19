#include "metaverse.h"
#include "display.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <vector>


/*
 * <count_neighbors>
 *
 * <Count the number of occupied neighbors for a given location.>
 *
 * input: <A const metaverse_t reference called board, an int called row, an int called column>
 * output: <The function will return the number of occupied neighbors in the current generation
 * of the Metaverse described by Metaverse referred to by the Metaverse constant reference at the 
 * location specified by the Row and Column number parameters.>
 */
int count_neighbors(const metaverse_t &board, int row, int column) {
    
    int count = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((!(i == 0 && j == 0)) && i+row != -1 && j + column!=-1  && i+row!=board.size() && j + column != board.size()) {
                if (board[i + row][j + column]) {
                    count++;
                }
            }
        }
    }
  return count;
}


/*
 * <occupied_in_next_tick>
 *
 * <This function will return true if a given location in
 * the Metaverse is occupied in the next generation.>
 *
 * input: <a bool called currently_occupied, an int called neighbor_count>
 * output: <The function will return true if the (un)occupied location with
 * the specified number of occupied neighbors will be occupied in the next generation.>
 */
bool occupied_in_next_tick(bool currently_occupied, int neighbor_count) {
    if (currently_occupied) {
        if (neighbor_count==2 || neighbor_count==3) {
            return true;
       }
    }
    if (!currently_occupied) {
        if (neighbor_count==3) {
            return true;
        }
    }
  return false;
}

/*
 * <tick>
 *
 * <Build a new Metaverse one tick in the future from a given Metaverse.>
 *
 * input: <a metaverse_t reference called board>
 * output: <The function will return a new Metaverse (as a metaverse_t) that
 * describes the next generation of the Metaverse described by the Metaverse reference parameter.>
 */
metaverse_t tick(const metaverse_t &board) {
    metaverse_t new_tick(board.size(), std::vector<bool>(board.size()));
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.size(); j++) {
            new_tick[i][j] = occupied_in_next_tick(board[i][j], count_neighbors(board, i, j));
        }
    }
  return new_tick;
}
/*
 * <relize_metaverse>
 *
 * <This function will resize a Metaverse according to a given size>
 *
 * input: <an int of rows, A metaverse_t reference called board >
 * output: <The function will resize the given metaverse_t parameter according
 * to the specified size. The function always returns true>
 */
bool resize_metaverse(int rows, metaverse_t &board) {
    board.resize(rows,std::vector<bool>(rows));

  return true;
}

/*
 * <citizenship_row_to_metaverse_row>
 *
 * <This function will take a string of characters read from 
 * a row of the Universe File and update the initial citizenship of Metaverse accordingly.>
 *
 * input: < a const reference to a std::string called input_row, an int called row,
 * A metaverse_t reference called board>
 * output: <The function will return true and update the given,Metaverse according to
 * the Row string as long as the initial occupancy length matches the Metaverse size.
 * It will return false, and not update the Metaverse, if the Row string does not have a valid length.>
 */
bool citizenship_row_to_metaverse_row(const std::string &input_row, int row,
                                      metaverse_t &board) {
    if (input_row.length() != board.size()) {
        return false;
    }
    for (int i = 0; i < board.size(); i++) {
        if (input_row[i] == '0') {
            board[row][i] = false;
        }
        else {
            board[row][i] = true;
        }
    }
    return true;
}
/*
 * <read_metaverse_configuration_line_from_file>
 *
 * < If the read was successful, the Size reference will be updated with the size of the Metaverse 
 * and the Generation reference will be updated with the number of generations to model (both as specified in the Universe File).>
 *
 * input: <Universe file reference,an int the size number reference, and an int generation number reference>
 * output: <The function returns true or false depending on whether a configuration 
 * could be read from the Universe File using the given reference>
 */
bool read_metaverse_configuration_line_from_file(std::ifstream &metaverse_file,
                                                 int &size, int &generations) {
    if (!(metaverse_file >> size)) {
        return false;
   }
    metaverse_file.ignore(1);
    if (!(metaverse_file >> generations)) {
        return false;
    }
  return true;
}
bool initialize_metaverse_from_file(std::ifstream &metaverse_file,
                                    metaverse_t &metaverse,
                                    int &generations) {
  int size{};
  int actual_row{};

  generations = 0;

  if (!read_metaverse_configuration_line_from_file(metaverse_file, size,
                                                   generations)) {
    return false;
  }

  if (!resize_metaverse(size, metaverse)) {
    return false;
  }

  metaverse_file >> std::ws;
  while (!metaverse_file.eof()) {
    std::string line{};
    if (!std::getline(metaverse_file, line)) {
      return false;
    }
    if (!citizenship_row_to_metaverse_row(line, actual_row, metaverse)) {
      return false;
    }
    actual_row++;
    metaverse_file >> std::ws;
  }
  return actual_row == size;
}

/*
 * <model_metaverse>
 *
 * <The function prints out the state of the Metaverse between generations>
 *
 * input: <a const reference to a metaverse_t, and an int the number of generations>
 * output: <This function does not return a value.>
 */
void model_metaverse(const metaverse_t &starting_metaverse, int generations) {
    metaverse_t  meta = starting_metaverse;
    for (int i = 0; i < generations; i++) {
        display_metaverse(std::cout, meta);
        meta = tick(meta);
       
    }
}
