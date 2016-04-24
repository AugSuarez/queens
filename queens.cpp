#include <iostream>
#include <stdexcept>
#include <vector>
//C. Augusto Suarez 4/24/2016, rel: A program that puts as many queens as there are rows or columns in the chess board, in a way that none of them treaten eachother
const int BOARD_SIZE=8;//change according to number of queens you wish to fit
// the higher the number of BOARD_SIZE, the more times an exception will be thrown given the random nature of the first queens position
std::string queenArray[BOARD_SIZE][BOARD_SIZE];
std::vector<int> safeVector;
constexpr int safeVectorStaticSize=(BOARD_SIZE*BOARD_SIZE);

void fillSafeVector(){
	for (int i = 0; i < (BOARD_SIZE*BOARD_SIZE); ++i)
		safeVector.push_back(i);
}

void printSafeVector(){
	std::cout << std::endl;
	for (int i = 0; i < safeVector.size(); ++i)
		std::cout << safeVector[i] << " ";
}

template<typename T>
void fillArray(T &myArray){
	for (int y = 0; y < BOARD_SIZE; ++y){
		for (int x = 0; x < BOARD_SIZE; ++x)
			myArray[y][x]="  ";
	}
}

template<typename T>
void printArray(T &myArray){
	for (int y = 0; y < BOARD_SIZE; ++y){
		std::cout << std::endl;
		for (int x = 0; x < BOARD_SIZE; ++x)
			std::cout << "[" << myArray[y][x] << "] ";
	}
}

inline int convertToX(int queenPosition){
	return (queenPosition%BOARD_SIZE);
}

inline int convertToY(int queenPosition){
	return ((queenPosition-(queenPosition%BOARD_SIZE))/BOARD_SIZE);
}

bool isUnderAttack(int queenPosition2, int queenPosition1){
	bool status, isVert, isHor, isDiag, isSame;
	int queen1x=convertToX(queenPosition1);
	int queen1y=convertToY(queenPosition1);
	int queen2x=convertToX(queenPosition2);
	int queen2y=convertToY(queenPosition2);

	if (queen2y==queen1y)//horizontal attack eg. if both queens are in same row, then true
		isHor=true;

	if(queen2x==queen1x)//vertical attack eg. if both queens are in same column, then true
		isVert=true;

	if(queen2x==queen1x && queen2y==queen1y)//is the position already taken by another queen?
		isSame=true;

	for (int x = 1; x <= BOARD_SIZE; ++x){//diagonal attack
		if((queen2y+x==queen1y && queen2x+x==queen1x) || (queen2y+x==queen1y && queen2x-x==queen1x) || (queen2y-x==queen1y && queen2x+x==queen1x) || (queen2y-x==queen1y && queen2x-x==queen1x))
				isDiag=true;
	}

	if(isSame==false && isVert==false && isHor==false && isDiag==false)
		status=false;

	else
		status=true;

	return status;
}

int safePosition(std::vector<int> &safeVector, int attackingQueenPosition){//will only receive values larger than one because of "position of queen" fx
	srand(time(NULL));

	for(int j=0; j<safeVector.size(); j++){
		for (int i = 0; i < safeVectorStaticSize; ++i){
			if(i==safeVector[j] && isUnderAttack(i,attackingQueenPosition)==true){
			//std::cout << "i(Number deleted from safeVector):" << i << " j(index of deleted #):" << j << std::endl;
			safeVector.erase(safeVector.begin()+j);
			}
		}
	}
	int newQueenPosition;

	if(safeVector.size()>0)
		newQueenPosition=safeVector[rand()%safeVector.size()];

	else 
		throw std::runtime_error("Next position not possible!");

	printSafeVector();
	return newQueenPosition;
}

int positionOfQueen(int x){
	srand(time(NULL));
	int position;

	if (x==1){
		position=safeVector[rand()%safeVector.size()];//uses attackingQueenPosition as 1 just to pass an int, it will still be changed later
		printSafeVector();
		for(int j=0; j<safeVector.size(); j++){
			for (int i = 0; i < safeVectorStaticSize; ++i){
				if(i==safeVector[j] && isUnderAttack(i,position)==true)
					safeVector.erase(safeVector.begin()+j);
			}
		}
	}
	else
		position=safePosition(safeVector, positionOfQueen(x-1));

	queenArray[convertToY(position)][convertToX(position)]=std::to_string(x);

	return position;
}

int main(){//----------------------------------------------------------START-MAIN------------------------------------------------------------
	fillArray(queenArray);
	fillSafeVector();
	try{
	positionOfQueen(BOARD_SIZE);
	}catch(std::runtime_error queenError){
		std::cerr << "\nRuntime Error: " << queenError.what();
	}
	printArray(queenArray);
	return 0;
}