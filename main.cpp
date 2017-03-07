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
int gridWidth;
int gridHeight;

// moving agent location
int agentX;
int agentY;

//starting agent location
int agentStartX;
int agentStartY;


int goalX;
int goalY;


#define crRand (double)rand()/RAND_MAX

void createGrid(vector<vector<double>>* qValueGrid, vector<vector<char>>* visual){
    
    srand(time(NULL));
    vector<double> qValueRows;
    vector<char> visualRows;
    
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
            
            qValueRows.push_back(crRand);
            visualRows.push_back('-');
        }
        qValueGrid->push_back(qValueRows);
        visual->push_back(visualRows);
        qValueRows.clear();
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
        
        agentY = rand()% gridHeight;
        agentX = rand()% gridWidth;
        
        agentStartX = agentX;
        agentStartY = agentY;
        
        goalY = rand()% gridHeight;
        goalX= rand()% gridWidth;
    }
    
    visual->at(agentY)[agentX]='A';
    visual->at(goalY)[goalX]='G';
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
    //srand(time(NULL));
    
    double best=-500;
    int choice;
    double explore= .005;
    
    for(int i =0;i<4;i++){
        //check up value
        if(agentY != gridHeight-1){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY+1)[agentX];
            choice = 1;
        }
        
        //check down
        if(agentY != 0){
            if(best<qValueGrid->at(agentY-1)[agentX]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY-1)[agentX];
                choice = 2;
            }
        }
        
        //check right move
        if(agentX != gridWidth-1){
            if(best<qValueGrid->at(agentY)[agentX+1]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY)[agentX+1];
                choice = 3;
            }
        }
        
        //check left move
        if(agentX != 0){
            if(best<qValueGrid->at(agentY)[agentX-1]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY)[agentX-1];
                choice = 4;
            }
        }
    }
    
    if(crRand<explore)
    {
        char valid='n';
        while(valid =='n'){
            choice = rand()% 4 + 1;
            
            //Make sure location is not near upper wall
            if(choice == 1 && agentY < gridHeight - 1){
                valid = 'y';
            }
            
            //Make sure location is not near lower wall
            else if(choice == 2 && agentY != 0){
                valid = 'y';
            }
            
            //Make sure location is not near right wall
            else if(choice == 3 && agentX < gridWidth - 1){
                valid = 'y';
            }
            
            //Make sure location is not near left wall
            else if(choice == 4 && agentX != 0){
                valid = 'y';
            }
            
            else{
                valid='n';
            }
            
        }
    }
    return choice;
    
};

void act(int choice){
    
    
    if(choice==1){
        agentY=agentY+1;
    }
    else if(choice==2){
        agentY=agentY-1;
        
    }
    else if(choice==3){
        agentX=agentX+1;
        
    }
    else if(choice==4){
        agentX=agentX-1;
        
    }
};

double react(int choice, vector<vector<double>>* qValueGrid){
    double reward = -1;
    double learner = .1;
    double discount = .9;
    
    // check to see if agent has found goal
    if(agentX == goalX && agentY == goalY){
        reward = 100;
        qValueGrid->at(agentY)[agentX] =discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX]);
        
    }
    
    // update previous location grid table
    if (choice==1){
        qValueGrid->at(agentY-1)[agentX] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY-1)[agentX]);
        //qValueGrid->at(agentY)[agentX] =discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY-1)[agentX]);
    }
    
    else if (choice==2){
        qValueGrid->at(agentY+1)[agentX] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY+1)[agentX]);
        //qValueGrid->at(agentY)[agentX] =discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY+1)[agentX]);
    }
    
    else if (choice==3){
        qValueGrid->at(agentY)[agentX-1] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX-1]);
        // qValueGrid->at(agentY)[agentX] =discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX-1]);
    }
    
    else if (choice==4){
        qValueGrid->at(agentY)[agentX+1] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX+1]);
        //qValueGrid->at(agentY)[agentX] =discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX+1]);
    }
    if(reward==100){
        //move agent back to starting position if goal was found
        agentX = agentStartX;
        agentY = agentStartY;
    }
    
    return reward;
};

void testD(vector<vector<double>>* qValueGrid){
    
    int highest = 0;
    int maxReward=100;
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            
            if(highest<qValueGrid->at(j)[i]){
                highest=qValueGrid->at(j)[i];
            }
        }
    }

    cout<<"You're max q value is: "<<highest<<endl;
    assert(highest<maxReward);
};

void runProgram(vector<vector<double>>* qValueGrid){
    
    vector<int> iter;
    
    int choice=0;
    int iterations = 0;
    int cycles = 10000;
    double reward = -1;
    
    for(int i=0;i<cycles;i++)
    {
        while(reward==-1)
        {
            choice = decide(qValueGrid);
            act(choice);
            reward = react(choice, qValueGrid);
            iterations+=1;
        }
        iter.push_back(iterations);
        iterations = 0;
        reward = -1;
    }
    for(int j = 0; j<cycles;j++){
        cout<<iter.at(j)<<endl;
    }
};

void printPath(vector<vector<double>>* qValueGrid,vector<vector<char>>* visual){
    agentX=agentStartX;
    agentY=agentStartY;
    int reward=-1;
    
    while(reward==-1)
    {
    
    double best=-500;
    int choice;
    
    for(int i =0;i<4;i++){
        //check up value
        if(agentY != gridHeight-1){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY+1)[agentX];
            choice = 1;
        }
        
        //check down
        if(agentY != 0){
            if(best<qValueGrid->at(agentY-1)[agentX]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY-1)[agentX];
                choice = 2;
            }
        }
        
        //check right move
        if(agentX != gridWidth-1){
            if(best<qValueGrid->at(agentY)[agentX+1]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY)[agentX+1];
                choice = 3;
            }
        }
        
        //check left move
        if(agentX != 0){
            if(best<qValueGrid->at(agentY)[agentX-1]){
                
                //if best, reassign best and make best choice
                best=qValueGrid->at(agentY)[agentX-1];
                choice = 4;
            }
        }
    }
        
        choice = decide(qValueGrid);
        int x;
        int y;
        x=agentX;
        y=agentY;
        act(choice);
        visual->at(y)[x]='*';
        reward = react(choice, qValueGrid);
    }
    
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            cout<<visual->at(j)[i]<<" ";
            
        }
        cout<<endl;
    }

};

int main() {
    
    vector<vector<double>> qValueGrid;
    vector<vector<char>> visual;
    
    createGrid(&qValueGrid, &visual);
    printGrid(&qValueGrid);
    runProgram(&qValueGrid);
    testD(&qValueGrid);
    printPath(&qValueGrid, &visual);
    
    return 0;
}

