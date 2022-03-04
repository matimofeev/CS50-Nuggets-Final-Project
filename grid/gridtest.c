//Matthew Timofeev 2022
//Testing file for grid module


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"



int main(const int argc, char* argv[])
{
  grid_t* grid;
  //test reading grid from invalid file (does not exist)
  //gives error
  printf("Reading grid from cats.txt (nonexistent)...\n");
  grid = grid_read("cats.txt");
  if(grid == NULL){
      fprintf(stderr,"Error reading grid\n");
  }  

  //read a small grid file (valid, and assume right format) into a grid structure
  printf("Reading grid from visdemo.txt...\n");
  grid = grid_read("visdemo.txt");
  if(grid == NULL){
    fprintf(stderr,"Error reading grid\n");
  }

  //print grid dimensions
  printf("Grid dimenstions are %d rows by %d cols\n", 
      grid_getNumberRows(grid), grid_getNumberCols(grid));

  //now make the spectator's view of the grid
  //first, make set of all locations in grid
  //(do not pass in gold or other player symbols for now)

  printf("Maxing set of locations...\n");
  set_t* allLocations = set_new();
  allLocations = grid_displaySpectator(grid, NULL,NULL);

  //print the set to a string
  printf("Printing the view to string...\n");
  char* printString = grid_print(grid, allLocations);
  printf("Spectator sees the following: \n%s\n",printString);
  set_delete(allLocations,NULL);

  
  //now test if locations are open or not
  //test with invalid locations <0 or > num columns*num rows -1 (not open)
  printf("Test negative locations and locations > grid size...\n"
          "Should give false...\n");

  if (!grid_isOpen(grid, -14)){
    fprintf(stderr,"-14 does not correspond to open location.\n");
  }
  if (!grid_isOpen(grid, grid_getNumberCols(grid)*grid_getNumberRows(grid))){
    fprintf(stderr,"%d does not correspond to open location.\n", 
      grid_getNumberCols(grid)*grid_getNumberRows(grid));
  }

  

  //location conversion also gives error on these invalid locations
  printf("Testing location convert on invalid locations. Should give errors...\n");
  if (grid_locationConvert(grid, -14)==NULL){
    fprintf(stderr,"-14 is invalid location.\n");
  }
  if (grid_locationConvert(grid,grid_getNumberCols(grid)*grid_getNumberRows(grid))==NULL){
    fprintf(stderr,"%d is invalid location.\n", grid_getNumberCols(grid)*grid_getNumberRows(grid));
  }
  
  //Test with valid locations
  printf("Testing with valid locations from 0 to grid size...\n");
  for(int i =0; i< grid_getNumberCols(grid)*grid_getNumberRows(grid); i++){
    int* coordinates = grid_locationConvert(grid,i);
    if(grid_isOpen(grid, i)){
      printf("The point at row %d , col %d is open\n", coordinates[0], coordinates[1]);
    }
    else{
      printf("The point at row %d , col %d is not open\n", coordinates[0], coordinates[1]);
    }
  }
  


  //Now test with player locations and gold symbols
  //Will not display symbols in not-open locations
  //Should not see player "C"

  set_t* playerLoc = set_new();
  set_insert(playerLoc,"42","A");
  set_insert(playerLoc,"58", "B");
  set_insert(playerLoc,"181", "C");
  set_insert(playerLoc, "193","D");

  counters_t* gold = counters_new();
  counters_add(gold,30);
  counters_add(gold,75);
  counters_add(gold,100);
  counters_add(gold,206);
  counters_add(gold,146);

  printf("Maxing set of locations, players and gold on grid...\n");
  allLocations = set_new();
  allLocations = grid_displaySpectator(grid, playerLoc,gold);

  //print the set to a string
  printf("Printing the view to string...\n");
  printString = grid_print(grid, allLocations);
  printf("Spectator sees the following (should not see \"C\"): \n%s\n",printString);
  set_delete(allLocations,NULL);


  //testing the visiblity feature

  set_t* visible = set_new();
  visible = grid_isVisible(grid,82,playerLoc,gold);
  printf("Printing the view to string...\n");
  printString = grid_print(grid, visible);
  printf("Player D sees the following: \n%s\n",printString);
  visible = grid_updateView(grid,198,visible,playerLoc,gold);
  printString = grid_print(grid, visible);
  printf("Player D sees the following: \n%s\n",printString);

  
  
  // char* printstring = grid_print(grid, visible);
  // printf("%s", printstring);
  // set_delete(visible, NULL);
  // mem_free(printstring);
  
    return 0;
}