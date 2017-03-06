//
//  main.cpp
//  Project Beta
//
//  Created by Cooper Richardson on 2/28/17.
//  Copyright © 2017 Cooper Richardson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>

using namespace std;

// Using global variables for all functions
int gridWidth=1;
int gridHeight=1;

// moving agent location
int agentX=0;
int agentY=0;

//starting agent location
int agentStartX=0;
int agentStartY=0;


int goalX=0;
int goalY=0;

int rewardGoal=100;
int rewardNot=-1;


#define crRand (double)rand()/RAND_MAX

void createGrid(vector<vector<double>>* qValueGrid, vector<double>* qValueRows){
    
    srand(time(NULL));
    
    // Input length and width of gridworld from user while width or height greater than 1
    while (gridWidth <=1 && gridHeight <=1){
    cout<<"How wide would you like your grid to be? ";
    cin>>gridWidth;
    cout<< endl;
    cout<<"How tall would you like your grid to be? ";
    cin>>gridHeight;
    cout<< endl<<endl;
        
        if(gridWidth <=1 && gridHeight<=1){
        cout<<"Nice... Why are you trying to break me? This time enter dimensions for a valid grid."<<endl<<endl;
            }
    }
    // Create gridworld given user inputs
    for(int i=0;i<gridHeight;i++){
        for(int j=0;j<gridWidth;j++){
            //qValueRows ->push_back(crRand);
            qValueRows ->push_back(crRand);
        }
        qValueGrid->push_back(*qValueRows);
        qValueRows->clear();
    }
    
    // Create agent and goal locations
    agentY = rand()% gridHeight;
    agentX = rand()% gridWidth;
    
    agentStartX = agentX;
    agentStartY = agentY;
    
    goalY = rand()% gridHeight;
    goalX = rand()% gridWidth;
    
    // If agent and goal are in same location, relocate
    while( agentY == goalY && agentX == goalX ){
        
        // subtract one because matrix starts at 0
        agentY = rand()% gridHeight;
        agentX = rand()% gridWidth;
        
        goalY = rand()% gridHeight;
        goalX= rand()% gridWidth;
    }
};

void printGrid(vector<vector<double>>* pGrid){
    cout<<"Note that the bottom left corner is location (0,0)"<<endl<<endl;
    
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            if(j==agentX && i== agentY){
                cout<<'A'<<" ";
            }
            else if(j==goalX && i == goalY){
            
                cout<<'G'<<" ";
            }
            else{
            cout<<"- ";
            }
        }
        cout<<endl;
    }
};

int decide(vector<vector<double>>* qValueGrid){
    srand(time(NULL));
    int best=-500;
    int choice;
    int learnerValue= .1;
    
    //Make sure location is not near upper wall
    if(agentY != gridHeight){
        
        //check up
        if(best<qValueGrid->at(agentY+1)[agentX]){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY+1)[agentX];
            choice = 1;
        }
    }
    
    //Make sure location is not near lower wall
    if(agentY != 0){
        
        //check down
        if(best<qValueGrid->at(agentY-1)[agentX]){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY-1)[agentX];
            choice = 2;
        }
    }
    
    //Make sure location is not near right wall
    if(agentX != gridWidth){
        
        //check right move
        if(best<qValueGrid->at(agentY)[agentX+1]){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY)[agentX+1];
            choice = 3;
        }
    }
    
    //Make sure location is not near left wall
    if(agentX != 0){
        
        //check left move
        if(best<qValueGrid->at(agentY)[agentX-1]){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY)[agentX-1];
            choice = 4;
        }
    }
    
    if(crRand<learnerValue){
        choice=5;
    }
    
    return choice;
};

int act(int choice){
    
    if(choice==1){
        agentY+=1;
    }
    else if(choice==2){
        agentY-=1;
        
    }
    else if(choice==3){
        agentX+=1;
        
    }
    else if(choice==4){
        agentX-=1;
        
    }
    else if(choice==5){
        char valid='n';
        while(valid =='n'){
            choice= rand()% 4;
            
            //Make sure location is not near upper wall
            if(choice == 1 && agentY < gridHeight){
                agentY+=1;
                valid = 'y';
                }
            
            //Make sure location is not near lower wall
            else if(choice == 2 && agentY != 0){
                agentY-=1;
                valid = 'y';
            }
            
            //Make sure location is not near right wall
            else if(choice == 3 && agentX < gridWidth){
                agentX+=1;
                valid = 'y';
            }
            
            //Make sure location is not near left wall
            else if(choice == 4 && agentX != 0){
                agentX-=1;
                valid = 'y';
            }
            
            else{
            valid='n';
            }
            
        }
    }
    return choice;
};

int react(int choice){
    int reward;
    int learner = .1;
    int discount = .9;
    
    
    // check to see if agent has found goal
    if(agentX == goalX && agentY == goalY){
        reward = 100;
        
        //move agent back to starting position
        agentX = agentStartX;
        agentY = agentStartY;
    }
    
    else{
        reward = -1;
    }
    
    // update previous location grid table
    if (choice==1){
    
    }
    
    else if (choice==2){
        
    }
    
    else if (choice==3){
        
    }
    
    else if (choice==4){
        
    }
    
    return reward;
};



int main() {
    
    vector<vector<double>> qValueGrid;
    vector<double> qValueRows;
    vector<int> iter;
    
    int choiceOne;
    int choiceTwo;
    int iterations = 0;
    int cycles = 30;
    int reward=-1;
    
    createGrid(&qValueGrid, &qValueRows);
    printGrid(&qValueGrid);
    
    
    for(int i=0;i<cycles;i++)
    {
        while(reward==-1)
        {
            choiceOne = decide(&qValueGrid);
            choiceTwo = act(choiceOne);
            reward=react(choiceTwo);
            iterations+=1;
        }
        iter.push_back(iterations);
        iterations = 0;
    }
    
    cout<<"Agent: ("<<agentX<<","<<agentY<<")"<<endl;
    cout<<"Goal: ("<<goalX<<","<<goalY<<")"<<endl;
    
        return 0;
}

