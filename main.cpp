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
int cycles = 100;

// moving agent location
int agentX;
int agentY;

//starting agent location
int agentStartX;
int agentStartY;

int goalX;
int goalY;


#define crRand (double)rand()/RAND_MAX

void createGrid(vector<vector<double>>* qValueGrid){
    
    srand(time(NULL));
    vector<double> qValueRows;
    
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
        }
        qValueGrid->push_back(qValueRows);
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

};

void printGrid(){
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
    
    double best = -500;
    int choice;
    double explore = .001;
    
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
    }
    
    else if (choice==2){
        qValueGrid->at(agentY+1)[agentX] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY+1)[agentX]);
    }
    
    else if (choice==3){
        qValueGrid->at(agentY)[agentX-1] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX-1]);
    }
    
    else if (choice==4){
        qValueGrid->at(agentY)[agentX+1] = discount*(qValueGrid->at(agentY)[agentX]) + learner*(reward - qValueGrid->at(agentY)[agentX+1]);
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
            
            if(highest<qValueGrid->at(j)[i])
            {
                highest=qValueGrid->at(j)[i];
            }
        }
    }

    cout<<"You're max q value is: "<<highest<<endl;
    assert(highest<maxReward);
    cout<<"Congratulations, test D was a success! The highest Q value did not surpass 100."<<endl;
};

void runProgramOnce(vector<vector<double>>* qValueGrid, vector<int>* iter){
    
    int choice=0;
    int iterations = 0;
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
        iter->push_back(iterations);
        iterations = 0;
        reward = -1;
    }

    
    
};

void runProgramThirty(vector<vector<double>>* qValueGrid, vector<int>* iter, vector<vector<int>>* allIterations){
    
    int choice=0;
    int iterations = 0;
    double reward = -1;
    vector<double> qValueRows;
    
    for(int j=0;j<30;j++){
        
        for(int i=0;i<cycles;i++)
        {
            while(reward==-1)
            {
                choice = decide(qValueGrid);
                act(choice);
                reward = react(choice, qValueGrid);
                iterations+=1;
            }
            iter->push_back(iterations);
            iterations = 0;
            reward = -1;
        }
        allIterations->push_back(*iter);
        iter->clear();
        qValueGrid->clear();
        
        // reinitialize qtable while leaving goal and agent locations in the same location
        for(int i=0;i<gridHeight;i++){
            for(int j=0;j<gridWidth;j++){
                
                qValueRows.push_back(crRand);
            }
            qValueGrid->push_back(qValueRows);
            qValueRows.clear();
        }
        
    }
    
  ofstream iterationsFile;
   iterationsFile.open("Iterations_Beta.txt");
    for(int i=0;i<cycles;i++){
        for(int j=0;j<30;j++){
        iterationsFile << allIterations->at(j)[i] <<"\t";
        }
        iterationsFile<< "\n";
      }
  iterationsFile.close();
    
};

void testE(vector<vector<double>>* qValueGrid){
    int xTest;
    int yTest;
    
    xTest = agentX;
    yTest = agentY;
    
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
        act(choice);
        reward = react(choice, qValueGrid);
        printGrid();
    }
    agentX=agentStartX;
    agentY=agentStartY;
    printGrid();
    
    assert(xTest==agentX && yTest==agentY);
    cout<<"Congratulations, test E was a success!"<<endl;

};

void testF(vector<int>* iter){
    double test;
    for(int j = 0; j<cycles;j++){
        cout<<iter->at(j)<< endl;
    }
    test= iter->at(0)/iter->at(99);
    assert(iter->at(0)> 5*iter->at(99));
    cout<<"Congratulations, test F was a success! The final iteration value was reduced by "<<test<<" times its original random search value by using Q-learning."<<endl;

};

int main() {
    
    vector<vector<double>> qValueGrid;
    vector<int> iter;
    vector<vector<int>> allIterations;
    
    createGrid(&qValueGrid);
    printGrid();
    cout<<"Agent Start Location: ("<<agentX<<","<<agentY<<")"<<endl;
    cout<<"Goal Start Location: ("<<goalX<<","<<goalY<<")"<<endl;
    
    runProgramOnce(&qValueGrid,&iter);
    testD(&qValueGrid);
    testE(&qValueGrid);
    testF(&iter);
    
    runProgramThirty(&qValueGrid, &iter,&allIterations);
    
    
    return 0;
}

