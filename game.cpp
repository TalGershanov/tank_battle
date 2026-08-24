#include "game.h"

using std ::tolower;
//check

int Game::showInstructions()//Want to maybe change the total format of the instruction, addin the symbols's meaning
{
    cout << "WELCOME TO TANKMANIA!" << endl
        << "Here's everything you need to know before diving into combat : " << endl
        << "Destroy your opponent by:" << endl
        << "-Hitting their tank with a shell" << endl
        << "-Luring them into a mine" << endl
        << "Survive to win. If both tanks are destroyed in the same moment � it�s a tie!" << endl
        << "Each tank is controlled by two keys  one per track (left/right), allowing smooth directional movement:" << endl
        << "Movement is continuous in the last chosen direction until:" << endl
        << "-A wall (#) is hit" << endl
        << "-A boundary is reached" << endl
        << "-You press the Stay key" << endl << "Case-insensitive input  both lowercase and uppercase are accepted!" << endl
        << "Firing:" << endl
        << "-Press the fire key to shoot a shell(*)." << endl
        << "-After shooting, there's a cooldown of 5 game cycles before you can shoot again." << endl
        << "If two shells collide, both explode." << endl
        << "Shells can destroy walls:" << "1st hit = weakened" << endl
        << "2nd hit = wall disappears" << endl
        << "Mine:(@)" << endl
        << "-Stepping on a mine = instant death" << endl
        << "-If only the cannon touches a mine, no effect" << endl
        << "-If only the cannon touches a mine, no effect" << endl
        << "Tanks cannot rotate toward walls directly in front of their cannon." << endl
        << "If a shell hits a wall:" << endl
        << "1st hit = weakened (H)" << endl
        << "2nd hit = destroyed"
        << endl << "Tunnel Edges" << endl
        << "Going beyond the edge of the screen warps you to the other side" << endl
        << "Applies to tanks and shells"
        << endl
        << "Pause Menu:" << endl
        << "Press ESC at any time:" <<
        "ESC again = continue" << endl
        << "Pressing X or x in pause mode ends the current game returning to the main menu "
        << endl
        << " Good luck, commanders! Let the best tank win!" << endl
        << "To hide the instructions and present the players's keys, press v";
    int ans = 1;
    char hide = _getch();
    if (hide == 'v' || hide == 'V') {
        clrscr();
        ans+= printKeys();
    }
    return ans;

}
bool Game::printKeys()//Presents the keys after clearing the screen, should i print the keys parallel to the game board ?
{
    gotoxy(0, 1);
    cout << "KEYS: PLAYER 1\n";
    //gotoxy(20 + BOARD_WIDTH, 2);
    cout << "-------------------------\n";
    //gotoxy(20 + BOARD_WIDTH, 3);
    cout << "E - RIGHT track forward\n";
    //gotoxy(20 + BOARD_WIDTH, 4);
    cout << "D - RIGHT track backward\n";
    //gotoxy(20 + BOARD_WIDTH, 5);
    cout << "Q - LEFT track forward\n";
    //gotoxy(20 + BOARD_WIDTH, 6);
    cout << "A - LEFT track backwards\n";
    //gotoxy(20 + BOARD_WIDTH, 7);
    cout << "S - STAY\n";
    //gotoxy(20 + BOARD_WIDTH, 8);
    cout << "W - Shoot\n";
    cout << "Lives: 1\n";

    gotoxy(20+BOARD_WIDTH+14,1 );
    cout << "KEYS: PLAYER 2";
    gotoxy(20 + BOARD_WIDTH+14, 2);
    cout << "-------------------------";
    gotoxy(20 + BOARD_WIDTH+14, 3);
    cout << "O - RIGHT track forward";
    gotoxy(20 + BOARD_WIDTH+14, 4);
    cout << "L - RIGHT track backward";
    gotoxy(20 + BOARD_WIDTH+14, 5);
    cout << "U - LEFT track forward";
    gotoxy(20 + BOARD_WIDTH+14, 6);
    cout << "J - LEFT track backwards";
    gotoxy(20 + BOARD_WIDTH+14, 7);
    cout << "K - STAY";
    gotoxy(20 + BOARD_WIDTH+14, 8);
    cout << "I - Shoot";
    gotoxy(20 + BOARD_WIDTH + 14, 9);
    cout << "Lives: 1";
    gotoxy(30, 19);
    cout << "To start the game, press 1.";
    char key = _getch();
    if (key)
    {
        gotoxy(30, 20);
        cout << "                             ";
        return true;
    }

return false;

}

void Game :: handleInput(Player &player1, Player &player2, std::vector<Shell>& shells ) {



    if (isKeyPressed('Z')) {
        player1.changeTankFlag();
    }
    Tank& activeTank1 = player1.getActiveTank();
    TrackStatus curLeftStatusTank1 = activeTank1.getLeftTrackStatus();
    TrackStatus curRightStatusTank1 = activeTank1.getRightTrackStatus();
    activeTank1.setLeftTrackStatus(TrackStatus::STAY);
    activeTank1.setRightTrackStatus(TrackStatus::STAY);
    bool user1keyPressed = false;

    if (isKeyPressed('E')) {
        activeTank1.setRightTrackStatus(TrackStatus::FORWARD);
        user1keyPressed = true;
    }
    if (isKeyPressed('D')) {
        activeTank1.setRightTrackStatus(TrackStatus::BACKWARD);
        user1keyPressed = true;
    }
    if (isKeyPressed('Q')) {
        activeTank1.setLeftTrackStatus(TrackStatus::FORWARD);
        user1keyPressed = true;
    }
    if (isKeyPressed('A')) {
        activeTank1.setLeftTrackStatus(TrackStatus::BACKWARD);
        user1keyPressed = true;
    }
    if (isKeyPressed('S')) {
        activeTank1.setRightTrackStatus(TrackStatus::STAY);
        activeTank1.setLeftTrackStatus(TrackStatus::STAY);
        user1keyPressed = true;
    }
    if (isKeyPressed('W')){
        activeTank1.setRightTrackStatus(curLeftStatusTank1);
        activeTank1.setLeftTrackStatus(curRightStatusTank1);
        user1keyPressed = true;
        if (activeTank1.canShoot()){
            shells.emplace_back((activeTank1.getCannonPosition()).NextPointIs(activeTank1.getDirection()), activeTank1.getDirection());
            activeTank1.resetCooldown();
        }
    }
    if (user1keyPressed == false){
        activeTank1.setLeftTrackStatus(curLeftStatusTank1);
        activeTank1.setRightTrackStatus(curRightStatusTank1);
    }



    if (isKeyPressed('M')) {
        player2.changeTankFlag();
    }
    Tank& activeTank2 = player2.getActiveTank();
    TrackStatus curLeftStatusTank2 = activeTank2.getLeftTrackStatus();
    TrackStatus curRightStatusTank2 = activeTank2.getRightTrackStatus();
    activeTank2.setLeftTrackStatus(TrackStatus::STAY);
    activeTank2.setRightTrackStatus(TrackStatus::STAY);
    bool user2keyPressed = false;


    if (isKeyPressed('O')) {
        activeTank2.setRightTrackStatus(TrackStatus::FORWARD);
        user2keyPressed = true;
    }
    if (isKeyPressed('L')) {
        activeTank2.setRightTrackStatus(TrackStatus::BACKWARD);
        user2keyPressed = true;
    }
    if (isKeyPressed('U')) {
        activeTank2.setLeftTrackStatus(TrackStatus::FORWARD);
        user2keyPressed = true;
    }
    if (isKeyPressed('J')) {
        activeTank2.setLeftTrackStatus(TrackStatus::BACKWARD);
        user2keyPressed = true;
    }
    if (isKeyPressed('K')) {
        activeTank2.setRightTrackStatus(TrackStatus::STAY);
        activeTank2.setLeftTrackStatus(TrackStatus::STAY);
        user2keyPressed = true;
    }
    if (isKeyPressed('I')){
        activeTank2.setRightTrackStatus(curLeftStatusTank2);
        activeTank2.setLeftTrackStatus(curRightStatusTank2);
        user2keyPressed = true;
        if (activeTank2.canShoot()){
            shells.emplace_back((activeTank2.getCannonPosition()).NextPointIs(activeTank2.getDirection()), activeTank2.getDirection());
            activeTank2.resetCooldown();
        }
    }

    if (user2keyPressed == false){
        activeTank2.setLeftTrackStatus(curLeftStatusTank2);
        activeTank2.setRightTrackStatus(curRightStatusTank2);
    }

}
//shell vector help from chat GPT
class Shell;
class Wall;
class Mine;

void Game::run(){
    char pickedOption;
    cout << "(1) Start a new game" << endl
        << "(8) Present instructions and key" << endl
        << "(9) EXIT" << endl;
    pickedOption = getchar();
    int wasPrinted = 0;
    switch (pickedOption)
    {
    case '1':
        clrscr();
        break;
    case '8':
        wasPrinted = showInstructions();
        if (wasPrinted==1)
            return;
        break;
    case '9':
        return; break;

    }

    Board board;
    board.display();

    std:: vector<Point> tankPositionsPlayer1 = {Point(TANK1_INIT_POS_X, TANK1_INIT_POS_Y),Point(TANK2_INIT_POS_X, TANK2_INIT_POS_Y) };
    Player player1(tankPositionsPlayer1, TANK1_FLAG,TANK_SYMBOL_PLAYER_1, board);

    std:: vector<Point> tankPositionsPlayer2 = {Point(TANK1_INIT_POS_X+3, TANK1_INIT_POS_Y+3),Point(TANK2_INIT_POS_X+3, TANK2_INIT_POS_Y+3) };
    Player player2(tankPositionsPlayer2,TANK1_FLAG,TANK_SYMBOL_PLAYER_2,board);

    std::vector<Shell> shells;
    Mine minesArray[NUM_OF_MINES];
    for (int i = 0; i < NUM_OF_MINES; ++i) {
        minesArray[i] = Mine(Point(MINE_POS_X[i], MINE_POS_Y[i]));
    }


    std::vector<Wall> walls = {
        Wall(Point(10, 10)),
        Wall(Point(10, 11)),
        Wall(Point(10, 12)),
        Wall(Point(10, 13)),

        Wall(Point(14, 16)),
        Wall(Point(14, 17)),
        Wall(Point(14, 18)),
        Wall(Point(14, 19)),
        Wall(Point(14, 20)),
        Wall(Point(14, 21)),

        Wall(Point(5, 7)),
        Wall(Point(6, 7)),
        Wall(Point(7, 7)),

        Wall(Point(2, MAX_COL-7)),
        Wall(Point(2, MAX_COL-8)),
        Wall(Point(2, MAX_COL-9)),
        Wall(Point(2, MAX_COL-10)),
        Wall(Point(2, MAX_COL-11)),
        Wall(Point(2, MAX_COL-12)),

        Wall(Point(10, 32)),
        Wall(Point(10, 33)),
        Wall(Point(10, 34)),
        Wall(Point(10, 35)),


        Wall(Point(12, 37)),
        Wall(Point(13, 37)),
        Wall(Point(14, 37)),
        Wall(Point(15, 37)),

        Wall(Point(MIN_ROW+1, MAX_COL-3)),
        Wall(Point(MIN_ROW+2, MAX_COL-3)),
        Wall(Point(MIN_ROW+3, MAX_COL-3)),
        Wall(Point(MIN_ROW+4, MAX_COL-3)),
        Wall(Point(MIN_ROW+5, MAX_COL-3)),
        Wall(Point(MIN_ROW+6, MAX_COL-3)),

        Wall(Point(MAX_ROW-1, MIN_COL+3)),
        Wall(Point(MAX_ROW-2, MIN_COL+3)),
        Wall(Point(MAX_ROW-3, MIN_COL+3)),
        Wall(Point(MAX_ROW-4, MIN_COL+3)),
        Wall(Point(MAX_ROW-5, MIN_COL+3)),
        Wall(Point(MAX_ROW-6, MIN_COL+3)),

    };

    while (true) {
        if (_kbhit()) {
            char keyPressed = _getch();
            if (isKeyPressed(27)) {
                gotoxy(20, 20);
                cout << "Game paused. Press ESC to resume or X to return to the main menu.";


                while (isKeyPressed(27)) {
                    Sleep(5);
                }

                while (true) {
                    if (isKeyPressed(27)) {
                        gotoxy(20, 20);
                        cout << "                                                                 ";
                        break;
                    }
                    if (isKeyPressed('x') || isKeyPressed('X')) {
                        clrscr();
                        cout << "Goodby comarads!!! and always remember: the one in the tank wins!!!";
                            Sleep(500);
                        return;
                    }
                    Sleep(5);
                }
            }
            handleInput(player1,player2, shells);
        }

        // Tank movement
        player1.moveTanks(board);
        player2.moveTanks(board);

        // Shells Movements + delete non-active shells
        for (auto& shell:shells) {
            shell.move(board);
        }
        shells.erase(
            std::remove_if(shells.begin(), shells.end(), [](const Shell& s) { return !s.isActive(); }),
            shells.end());

        // Tank and shells Collisions Detection
        for (size_t i = 0; i < shells.size(); i++) {
            // player 1
            for (Tank &t1 : player1.getTankVector()) {
                if (shells[i].getPosition() == t1.getPosition()) {
                    t1.markAsHit();
                    t1.removeDeadTank(board);
                    player1.changeTankFlag();
                    shells[i].deactivate();
                }
                if (shells[i].getPosition() == t1.getCannonPosition()) {
                    t1.removeCannon(board);
                    shells[i].deactivate();
                }
            }


            // player 2
            for (Tank &t2 : player2.getTankVector()) {
                // shells;
                if (shells[i].getPosition() == t2.getPosition()) {
                    t2.markAsHit();
                    t2.removeDeadTank(board);
                    player2.changeTankFlag();
                    shells[i].deactivate();
                }
                if (shells[i].getPosition() == t2.getCannonPosition()) {
                    t2.removeCannon(board);
                    shells[i].deactivate();
                }

            }


            // Shells collisions with other shells
            for (size_t j = i+1; j < shells.size(); j++) {
                if (shells[i].getPosition() == shells[j].getPosition()) {
                    shells[i].deactivate();
                    shells[j].deactivate();
                }
            }

            // collisions in Walls
            for (size_t j = 0; j < walls.size(); j++) {
                if (shells[i].getPosition() == walls[j].getPosition()) {
                    shells[i].deactivate();
                    walls[j].increaseHits();
                }
            }
        }



        // collisions with Mine
        for (int t = 0; t < NUM_OF_MINES; ++t) {

            //player1
            if (!(minesArray[t].isDestroyed())) {
            for (Tank &t1 : player1.getTankVector()) {
                if ((minesArray[t].getPosition() == t1.getPosition())) {
                    t1.markAsHit();
                    t1.removeDeadTank(board);
                    player1.changeTankFlag();
                    minesArray[t].setDestroyed();
                }
                if (minesArray[t].getPosition() == t1.getCannonPosition()) {
                    minesArray[t].markAsInvisible();
                }
            }}
            //player2
            for (Tank &t2 : player2.getTankVector()) {
                if (!(minesArray[t].isDestroyed())) {
                    if ((minesArray[t].getPosition() == t2.getPosition())) {
                        t2.markAsHit();
                        t2.removeDeadTank(board);
                        player2.changeTankFlag();
                        minesArray[t].setDestroyed();
                    }
                    if (minesArray[t].getPosition() == t2.getCannonPosition()) {
                        minesArray[t].markAsInvisible();
                    }

                    for (auto &shell: shells) {
                        if (shell.getPosition() == minesArray[t].getPosition())
                            minesArray[t].markAsInvisible();

                    }
                }}
                if (!(minesArray[t].isDestroyed())) {
            minesArray[t].draw();
            minesArray[t].markAsVisible();
                }


            }



        // remove destroyed walls + draw walls
        walls.erase(
            std::remove_if(walls.begin(), walls.end(), [](const Wall& w) { return w.isDestroyed(); }),
            walls.end());

        for (auto& wall:walls) {
            wall.draw(board);
        }


        if (player1.allTanksDead() && player2.allTanksDead())
        {
             // it's a tie end game

            if (wasPrinted==2)
            {
                gotoxy(41 + BOARD_WIDTH, 9);
                cout << "0";
                gotoxy(7, 10);//Player 1 life update
                cout << "0";
            }
            gotoxy(30, 20);
             cout << "Its a tie!";
             return;
        }
        else if (player1.allTanksDead())
        {
            //      tank 2 won end game

            if (wasPrinted ==2)
            {
            gotoxy(7, 9);//Player 1 life update
            cout << "0";
             }
            gotoxy(30, 20);
            cout << "Congratulations!!! player 2 won the battle.";
            return;

        }
        else if (player2.allTanksDead())
        {

            if (wasPrinted ==2) {
                gotoxy(41 + BOARD_WIDTH, 9);//Player 2 life update
                cout << "0";
            }
            gotoxy(30, 20);
            cout << "Congratulations!!! player 1 won the battle.";
            return;
         }



        // shell logic here
        player1.cooldownTickAllTanks();
        player2.cooldownTickAllTanks();

        Sleep(500);

    }
}
