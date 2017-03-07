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
            qValueRows ->push_back(crRand+rand()%10);
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
        
        agentY = rand()% gridHeight;
        agentX = rand()% gridWidth;
        
        agentStartX = agentX;
        agentStartY = agentY;
        
        goalY = rand()% gridHeight;
        goalX= rand()% gridWidth;
    }
    qValueGrid->at(goalY)[goalX]=100;
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
  //  srand(time(NULL));
    
    double best= -500.1;
    int choice;
    double explore= .2;
    
        //check up value
        if(agentY != gridHeight-1){
            if(best<qValueGrid->at(agentY+1)[agentX]){
            
            //if best, reassign best and make best choice
            best=qValueGrid->at(agentY+1)[agentX];
            choice = 1;
            }
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
    
    if(crRand<explore)
        {
                char valid='n';
                while(valid =='n'){
                    choice= rand()% 4 + 1;
                    
                    //Make sure location is not near upper wall
                    if(choice == 1 && agentY < gridHeight -1){
                        valid = 'y';
                        return choice;
                    }
                    
                    //Make sure location is not near lower wall
                    else if(choice == 2 && agentY != 0){
                        valid = 'y';
                        return choice;
                    }
                    
                    //Make sure location is not near right wall
                    else if(choice == 3 && agentX < gridWidth - 1){
                        valid = 'y';
                        return choice;
                    }
                    
                    //Make sure location is not near left wall
                    else if(choice == 4 && agentX != 0){
                        valid = 'y';
                        return choice;
                    }
                    
                    else{
                        valid='n';
                    }
                    
                }
        }
    return choice;
    
};

void act(int choice){
    
    //cout<<"here 2"<<endl;
    
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
};

double react(int choice, vector<vector<double>>* qValueGrid){
    double reward;
    double learner = .1;
    double discount = .9;
   // cout<<"here 3"<<endl;
    
    // check to see if agent has found goal
    if(agentX == goalX && agentY == goalY){
        reward = 100;
        
        qValueGrid->at(agentY)[agentX] = learner*( reward + discount*qValueGrid->at(agentY)[agentX]-qValueGrid->at(agentY-1)[agentX]);
        
        //move agent back to starting position
        agentX = agentStartX;
        agentY = agentStartY;
        
    }
    
    else {
        reward = -.01;
    }
    
    // update previous location grid table
    if (choice==1){
        qValueGrid->at(agentY-1)[agentX] = qValueGrid->at(agentY-1)[agentX] + learner*( reward + discount*qValueGrid->at(agentY)[agentX]-qValueGrid->at(agentY-1)[agentX]);
    }
    
    else if (choice==2){
        qValueGrid->at(agentY+1)[agentX] = qValueGrid->at(agentY+1)[agentX] + learner*( reward + discount*qValueGrid->at(agentY)[agentX]-qValueGrid->at(agentY+1)[agentX]);
    }
    
    else if (choice==3){
        qValueGrid->at(agentY)[agentX-1] = qValueGrid->at(agentY)[agentX-1] + learner*( reward + discount*qValueGrid->at(agentY)[agentX]-qValueGrid->at(agentY)[agentX-1]);
    }
    
    else if (choice==4){
        qValueGrid->at(agentY)[agentX+1] = qValueGrid->at(agentY)[agentX+1] + learner*( reward + discount*qValueGrid->at(agentY)[agentX]-qValueGrid->at(agentY)[agentX+1]);
    }
    
    return reward;
};



int main() {
    
    vector<vector<double>> qValueGrid;
    vector<double> qValueRows;
    vector<int> iter;
    
    int choice=0;
    int iterations = 0;
    int cycles = 100;
    double reward = -.01;
    
    createGrid(&qValueGrid, &qValueRows);
    printGrid(&qValueGrid);
    
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            cout<<" " <<qValueGrid.at(i)[j]<<" ";
        }
        cout<<endl<<endl;
    }
    
    for(int i=0;i<cycles;i++)
    {
        while(reward==-.01)
        {
            choice = decide(&qValueGrid);
            act(choice);
            reward = react(choice, &qValueGrid);
            //printGrid(&qValueGrid);
            iterations+=1;
            
            for(int i=gridHeight-1;i>-1;i--){
                for(int j=0;j<gridWidth;j++){
                    cout<<" " <<qValueGrid.at(i)[j]<<" ";
                }
                cout<<endl;
            }
            
        }
        iter.push_back(iterations);
        iterations = 0;
        reward = -.01;
        cout<<"Agent has been reset.................//////////........../////////////////"<<endl;
    }

    
    for(int j = 0; j<cycles;j++){
        cout<<iter.at(j)<<endl;
    }
    
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
                cout<<" " <<qValueGrid.at(i)[j]<<" ";
            }
        cout<<endl;
    }

    
    
    cout<<"Agent: ("<<agentX<<","<<agentY<<")"<<endl;
    cout<<"Goal: ("<<goalX<<","<<goalY<<")"<<endl;
    
        return 0;
}

