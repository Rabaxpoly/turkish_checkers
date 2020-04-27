#include <stdio.h>

char board[9][9];              //global array


void read(FILE *file){
    char temp[300];
    if ((file = fopen ("dama.txt", "r+")) == NULL) {
        printf("file not opened.");
    }else {
        for (int i = 0; i < 270; ++i) {
            temp[i]=fgetc(file);
            if( feof(file) ) {
                break ;
            }
        }

        int i=0,j=8,k=0;
        while (j>-1) {
            if (temp[i] != ' '&&temp[i] != '\n') {
                board[j][k]=temp[i];
                k++;
                i++;
                if(k==9){
                    k=0;
                    j--;
                }
            }i++;
        }
    }
    fclose(file);
}


void reset(){
    char row0 [9]={'+','A','B','C','D','E','F','G','H'};   //i0
    char row1 [9]={'1','_','_','_','_','_','_','_','_'};   //i1
    char row2 [9]={'2','w','w','w','w','w','w','w','w'};   //i2
    char row3 [9]={'3','w','w','w','w','w','w','w','w'};   //i3
    char row4 [9]={'4','_','_','_','_','_','_','_','_'};   //i4
    char row5 [9]={'5','_','_','_','_','_','_','_','_'};   //i5
    char row6 [9]={'6','b','b','b','b','b','b','b','b'};   //i6
    char row7 [9]={'7','b','b','b','b','b','b','b','b'};   //i7
    char row8 [9]={'8','_','_','_','_','_','_','_','_'};   //i8
               //  j0 ,j1 ,j2 ,j3 ,j4 ,j5 ,j6 ,j7 ,j8
    for (int i = 0; i <9 ; ++i) {
        board[0][i]=row0[i];
        board[1][i]=row1[i];
        board[2][i]=row2[i];
        board[3][i]=row3[i];
        board[4][i]=row4[i];
        board[5][i]=row5[i];
        board[6][i]=row6[i];
        board[7][i]=row7[i];
        board[8][i]=row8[i];
    }
}


void firstWrite(FILE *file){
    file = fopen ("dama.txt", "w+");
    for (int i = 8; i > -1; --i) {
        for (int j = 0; j < 9; ++j) {
            fprintf(file,"%c", board[i][j]);
            if(board[i][j]=='W'){                                       //for WW
                fprintf(file,"%c",'W');
                fprintf(file," ");
            }else if(board[i][j]=='B'){                                 //for BB
                if(i==0){
                    fprintf(file,"  ");
                } else{
                    fprintf(file,"%c",'B');
                    fprintf(file," ");
                }
            }else{
                fprintf(file,"  ");
            }
        }
        fprintf(file,"\n");
    }fclose(file);
}

void write(FILE *file,int x,int y,char value){
    file = fopen("dama.txt", "r+");
    fseek(file, ((8-x)*29)+(3*y), SEEK_SET);
    fprintf(file, "%c",value);
    fclose(file);
}
void saveTxt(FILE *save,int x1,int y1,int x2,int y2){
    char c1,c2;
    c1=y1+'0'+48;
    c2=y2+'0'+48;
    save = fopen("save.txt", "a+");              //record file
    if(board[0][0]=='+') {
        fprintf(save, "w:%c%d%c%d\n",c1,x1,c2,x2);
    }
    if(board[0][0]=='-') {
        fprintf(save, "b:%c%d%c%d\n",c1,x1,c2,x2);
    }
    fclose(save);
}
void resetTxt(FILE *save){
    save = fopen("save.txt", "w+");
    fclose(save);
}
void showTxt(FILE *save){       //Shown when you pick up where you left off
    int temp [100][8];
    save = fopen("save.txt", "r+");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j <8 ; ++j) {
            temp[i][j]=fgetc(save);
            if( feof(save) ) {
                break ;
            }
       }
    }

    printf("Moves in the last record\n");
    printf("|   |   |   |\n");
    printf("v   v   v   v \n");
    for (int i = 0; i <100 ; ++i) {
        for (int j = 0; j <8 ; ++j) {
            printf("%c",temp[i][j]);
            if(temp[i][j]==-1){
                i=100;
                j=8;
            }
        }
    }
    printf("<-->\n");
    fclose(save);
}


void stampMove(FILE *file,int x1,int y1 , int x2,int y2){        //stamp moves with double char (WW-BB)
    file = fopen("dama.txt", "r+");
    if(board[x1][y1]=='W'||board[x1][y1]=='w'){
        fseek(file, ((8-x1)*29)+(3*y1), SEEK_SET);
        fprintf(file, "%c",'_');
        fseek(file, ((8-x1)*29)+(3*y1)+1, SEEK_SET);
        fprintf(file, "%c",' ');
        fseek(file, ((8-x2)*29)+(3*y2), SEEK_SET);
        fprintf(file, "%c",'W');
        fseek(file, ((8-x2)*29)+(3*y2)+1, SEEK_SET);
        fprintf(file, "%c",'W');
    }
    if(board[x1][y1]=='B'||board[x1][y1]=='b'){
        fseek(file, ((8-x1)*29)+(3*y1), SEEK_SET);
        fprintf(file, "%c",'_');
        fseek(file, ((8-x1)*29)+(3*y1)+1, SEEK_SET);
        fprintf(file, "%c",' ');
        fseek(file, ((8-x2)*29)+(3*y2), SEEK_SET);
        fprintf(file, "%c",'B');
        fseek(file, ((8-x2)*29)+(3*y2)+1, SEEK_SET);
        fprintf(file, "%c",'B');
    }
    fclose(file);
}

int isAround(int x,int y){          //is there any able to capture pieces or stamps
    if(board[0][0]=='+'){
        if(board[x][y]=='w') {
            if (board[x + 2][y] == '_') {
                if (board[x + 1][y] == 'b' || board[x + 1][y] == 'B') {
                    return 1;
                }
            }
            if (board[x][y + 2] == '_') {
                if (board[x][y + 1] == 'b' || board[x][y + 1] == 'B') {
                    return 1;
                }
            }
            if (board[x][y - 2] == '_') {
                if (board[x][y - 1] == 'b' || board[x][y - 1] == 'B') {
                    return 1;
                }
            }
        }else if(board[x][y]=='W'){
            for (int i = x+1; i < 9; ++i) {
                if(board[i][y]=='b'||board[i][y]=='B'){
                    if(board[i+1][y]=='_'){
                        return 1;
                    }else if(board[i+1][y]=='b'||board[i+1][y]=='B'){
                        break;
                    }
                }if(board[i][y]=='w'||board[i][y]=='W'){
                    break;
                }
            }
            for (int i = x-1; i > 0; --i) {
                if(board[i][y]=='b'||board[i][y]=='B'){
                    if(board[i-1][y]=='_'){
                        return 1;
                    }else if(board[i-1][y]=='b'||board[i-1][y]=='B'){
                        break;
                    }
                }if(board[i][y]=='w'||board[i][y]=='W'){
                    break;
                }
            }
            for (int i = y+1; i < 9; ++i) {
                if(board[x][i]=='b'||board[x][i]=='B'){
                    if(board[x][i+1]=='_'){
                        return 1;
                    }else if(board[x][i+1]=='b'||board[x][i+1]=='B'){
                        break;
                    }
                }if(board[x][i]=='w'||board[x][i]=='W'){
                    break;
                }
            }
            for (int i = y-1; i >0; --i) {
                if(board[x][i]=='b'||board[x][i]=='B'){
                    if(board[x][i-1]=='_'){
                        return 1;
                    }else if(board[x][i-1]=='b'||board[x][i-1]=='B'){
                        break;
                    }
                }if(board[x][i]=='w'||board[x][i]=='W'){
                    break;
                }
            }

        }
    }else if(board[0][0]=='-'){
        if(board[x][y]=='b') {
            if (board[x - 2][y] == '_') {
                if (board[x - 1][y] == 'w' || board[x - 1][y] == 'W') {
                    return 1;
                }
            }
            if (board[x][y + 2] == '_') {
                if (board[x][y + 1] == 'w' || board[x][y + 1] == 'W') {
                    return 1;
                }
            }
            if (board[x][y - 2] == '_') {
                if (board[x][y - 1] == 'w' || board[x][y - 1] == 'W') {
                    return 1;
                }
            }
        }else if(board[x][y]=='B'){
            for (int i = x+1; i < 9; ++i) {
                if(board[i][y]=='w'||board[i][y]=='W'){
                    if(board[i+1][y]=='_'){
                        return 1;
                    }else if(board[i+1][y]=='w'||board[i+1][y]=='W'){
                        break;
                    }
                }if(board[i][y]=='b'||board[i][y]=='B'){
                    break;
                }
            }
            for (int i = x-1; i > 0; --i) {
                if(board[i][y]=='w'||board[i][y]=='W'){
                    if(board[i-1][y]=='_'){
                        return 1;
                    }else if(board[i-1][y]=='w'||board[i-1][y]=='W'){
                        break;
                    }
                }if(board[i][y]=='b'||board[i][y]=='B'){
                    break;
                }
            }
            for (int i = y+1; i < 9; ++i) {
                if(board[x][i]=='w'||board[x][i]=='W'){
                    if(board[x][i+1]=='_'){
                        return 1;
                    }else if(board[x][i+1]=='w'||board[x][i+1]=='W'){
                        break;
                    }
                }if(board[x][i]=='b'||board[x][i]=='B'){
                    break;
                }
            }
            for (int i = y-1; i >0; --i) {
                if(board[x][i]=='w'||board[x][i]=='W'){
                    if(board[x][i-1]=='_'){
                        return 1;
                    }else if(board[x][i-1]=='w'||board[x][i-1]=='W'){
                        break;
                    }
                }if(board[x][i]=='b'||board[x][i]=='B'){
                    break;
                }
            }
        }
    }else{
        printf("Turn value could not be read");
    }

    return 0;
}


int main() {
    char inputNull,input1, input2, input3, input4,lastX='a',lastY='1';
    int x1,x2,y1,y2,count,isAtk,reAtck=0;
    FILE *file;
    FILE *save;
    file = fopen ("dama.txt", "r+");
    fclose(file);
    save = fopen ("save.txt", "r+");
    fclose(save);

    printf("\n\t   CHECKERS   \n ------------------------------------");
    printf("\n\n 1 -> New Game \n 2 -> Continue game \n 3 -> End Game \n");


    while(1){                                                       //Main menu
        int ch=0;
        char c;                                 //bug fixed when entered another type from integer
        scanf_s("%c",&c);
        ch=(int)c-48;
        if(ch==1){
            reset();
            resetTxt(save);
            firstWrite(file);
            break;
        }else if(ch==2){
            read(file);
            firstWrite(file);
            showTxt(save);
            break;
        }else if(ch==3){
            printf("Game is closing...\n");
            return 1;
        }else{
            printf("Choose one(1-2-3)\n");
        }
    }


    while(1){                                                            //Game loop
        count=0; isAtk=0;
        for (int i = 1; i <9 ; ++i) {
            for (int j = 1; j <9 ; ++j) {
                if(board[i][j]=='w'||board[i][j]=='W'){
                    count++;                                   //to check if there are attackable enemies
                    if(1==isAround(i,j)){                      //and to check the end of the game.
                        isAtk=1; i=9; j=9;
                    }
                }
            }
        }if (count == 0) {
            printf("Black won the match.\n");
            break;
        }

        if(board[0][0]=='+'){                                                            //TURN WHITE
            while(1){
                if(reAtck==1){
                    printf("The turn is still on the white team.\n ex:(->b3)\n");
                    printf("%c%c->", lastX, lastY);
                    scanf("%c%c%c", &inputNull, &input3, &input4);
                    //first variable automatically takes null value in while due to compiler error.
                    input1=lastX;
                    input2=lastY;
                }else{
                    printf("Team White please enter your command.\n ex:(a2b3)\n");
                    scanf("%c%c%c%c%c", &inputNull, &input1, &input2, &input3, &input4);
                    //first variable automatically takes null value due to compiler error.
                    //we can still assign values to variables by entering 4 characters.
                }
                y1= (int)input1 - 96;  //char to integer by ASCII code
                y2= (int)input3 - 96;
                x1= (int)input2 - 48;
                x2= (int)input4 - 48;
                if(x1>0&&9>x1&&y1>0&&9>y1){
                    if(x2>0&&9>x2&&y2>0&&9>y2) {
                        break;
                    } else{
                        printf("Point of destination not on board.Re-enter between these values(1-8)(a-h)\n");
                    }
                }else{
                    printf("Initial value not on board.Re-enter between these values(1-8)(a-h)\n");
                }
            }

            if(board[x2][y2]=='_'){                         //-----------Basic Piece Move--------
                if(board[x1][y1]=='w') {
                    if(0==isAtk) {                          //If a player is able to make a capture, there is no option
                        if ((x2 - x1) == 1 && (y2 - y1) == 0) {
                            if (x2 == 8) {                                         //vertical move
                                stampMove(file,x1,y1,x2,y2);                                      //transform to stamp
                            } else {
                                write(file, x2, y2, 'w');
                                write(file, x1, y1, '_');
                            }
                            saveTxt(save,x1,y1,x2,y2);
                            write(file, 0, 0, '-');
                            read(file);
                        } else if ((x2 - x1) == 0 && ((y2 - y1) == 1 || (y2 - y1) == -1)) {             //horizontal move
                            write(file, x2, y2, 'w');
                            write(file, x1, y1, '_');
                            saveTxt(save,x1,y1,x2,y2);
                            write(file, 0, 0, '-');
                            read(file);
                        }else{
                            printf("Wrong no-attack move.\n");
                        }
                    }else {
                        if ((x2 - x1) == 2 && (y2 - y1) == 0 &&
                            (board[(x1 + 1)][y1] == 'b' || board[(x1 + 1)][y1] == 'B')) {
                            write(file, (x1 + 1), y1, '_');                              // straight capture
                            if (x2 == 8) {
                                stampMove(file,x1,y1,x2,y2);                    //transform to stamp
                            } else {
                                write(file, x1, y1, '_');
                                write(file, x2, y2, 'w');
                            }
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '-');
                            }
                            read(file);
                        } else if ((x2 - x1) == 0 && (y2 - y1) == 2 &&
                                   (board[x1][(y1 + 1)] == 'b' || board[x1][(y1 + 1)] == 'B')) {
                            write(file, x2, y2, 'w');
                            write(file, x1, y1, '_');
                            write(file, x1, (y1 + 1), '_');                                 //right capture
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '-');
                            }
                            read(file);
                        } else if ((x2 - x1) == 0 && (y1 - y2) == 2 &&
                                   (board[x1][(y2 + 1)] == 'b' || board[x1][(y2 + 1)] == 'B')) {
                            write(file, x2, y2, 'w');
                            write(file, x1, y1, '_');
                            write(file, x1, (y1 - 1), '_');                                    //left capture
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '-');
                            }
                            read(file);
                        } else {
                            printf("U have to attack.Enter again\n");
                        }
                    }
                }else if(board[x1][y1]=='W'){                              //-----Stamp Move---------------------------
                    int temp=0,between=0;
                    if(0==isAtk) {
                        if(y2 == y1) {
                            if (x2 > x1) {                                              //Up movement
                                for (int i = (x1+1); i <x2 ; ++i) {
                                    if(board[i][y1]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(x2-x1-1)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '-');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }
                            } else if (x2 < x1) {                                   //Down movement
                                for (int i = (x1-1); i >x2 ; --i) {
                                    if(board[i][y1]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(x1-1-x2)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '-');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way.\n");
                                }

                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else if(x2 == x1) {
                            if (y1 > y2) {                                      //Move to the left
                                for (int i = (y1-1); i >y2 ; --i) {
                                    if(board[x1][i]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(y1-1-y2)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '-');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }

                            } else if (y2 > y1) {                           //Move to the right
                                for (int i = (y1+1); i <y2 ; ++i) {
                                    if(board[x1][i]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(y2-1-y1)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '-');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }

                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else {
                            printf("Your stamp cannot go in this direction.\n");
                        }
                    }else{                                      //if you have to attack
                        if(y2 == y1) {
                            if (x2 > x1) {                                            //capture the top
                                for (int i = (x1+1); i <x2 ; ++i) {
                                    if(board[i][y1]!='w'&&board[i][y1]!='W'){
                                        if(board[i][y1]=='b'||board[i][y1]=='B'){
                                            between++;
                                            temp=i;          //data of the information to be deleted
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, temp, y1, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '-');
                                    }
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            } else if (x2 < x1) {                                   //capture the bottom
                                for (int i = (x1-1); i >x2 ; --i) {
                                    if(board[i][y1]!='w'&&board[i][y1]!='W'){
                                        if(board[i][y1]=='b'||board[i][y1]=='B'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, temp, y1, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '-');
                                    }
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else if(x2 == x1){
                            if (y1 > y2) {                                          //capture the left
                                for (int i = (y1-1); i >y2 ; --i) {
                                    if(board[x1][i]!='w'&&board[x1][i]!='W'){
                                        if(board[x1][i]=='b'||board[x1][i]=='B'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, x1, temp, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '-');
                                    }
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            } else if (y2 > y1) {                               //capture the right
                                for (int i = (y1+1); i <y2 ; ++i) {
                                    if(board[x1][i]!='w'&&board[x1][i]!='W'){
                                        if(board[x1][i]=='b'||board[x1][i]=='B'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, x1, temp, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '-');
                                    }
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            } else {
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else{
                            printf("Your stamp cannot go in this direction.\n");
                        }
                    }
                }else{
                    printf("Your pieces or your stamps not found.\n");
                }
            }else {
                printf("The tile to go is not empty.\n");
            }
        }

        count=0; isAtk=0;
        for (int i = 1; i <9 ; ++i) {
            for (int j = 1; j <9 ; ++j) {
                if(board[i][j]=='b'||board[i][j]=='B'){
                    count++;
                    if(1==isAround(i,j)){
                        isAtk=1; i=9; j=9;
                    }
                }
            }
        }if (count == 0) {
            printf("White won the match.\n");
            break;
        }
        if(board[0][0]=='-'){                                                            //TURN BLACK
            while(1){
                if(reAtck==1){
                    printf("The turn is still on the black team.\n ex:(->b3)\n");
                    printf("%c%c->", lastX, lastY);
                    scanf("%c%c%c", &inputNull, &input3, &input4);
                    //first variable automatically takes null value in while due to compiler error.
                    input1=lastX;
                    input2=lastY;
                }else{
                    printf("Team Black please enter your command.\n ex:(a2b3)\n");
                    scanf("%c%c%c%c%c", &inputNull, &input1, &input2, &input3, &input4);
                    //first variable automatically takes null value due to compiler error.
                    //we can still assign values to variables by entering 4 characters.
                }
                y1= (int)input1 - 96;  //char to integer by ASCII code
                y2= (int)input3 - 96;
                x1= (int)input2 - 48;
                x2= (int)input4 - 48;
                if(x1>0&&9>x1&&y1>0&&9>y1){
                    if(x2>0&&9>x2&&y2>0&&9>y2) {
                        break;
                    } else{
                        printf("Point of destination not on board.Re-enter between these values(1-8)(a-h)\n");
                    }
                }else{
                    printf("Initial value not on board.Re-enter between these values(1-8)(a-h)\n");
                }
            }
            if(board[x2][y2]=='_'){                        //-----------Basic Piece Move--------
                if(board[x1][y1]=='b'){
                    if(0==isAtk) {                          //If a player is able to make a capture, there is no option
                        if ((x2 - x1) == -1 &&
                            (y2 - y1) == 0) {                                             //vertical move
                            if (x2 == 1) {
                                stampMove(file,x1,y1,x2,y2);                               //transform to stamp
                            } else {
                                write(file, x2, y2, 'b');
                                write(file, x1, y1, '_');
                            }
                            saveTxt(save,x1,y1,x2,y2);
                            write(file, 0, 0, '+');
                            read(file);
                        } else if ((x2 - x1) == 0 &&
                                   ((y2 - y1) == 1 || (y2 - y1) == -1)) {                //horizontal move
                            write(file, x2, y2, 'b');
                            write(file, x1, y1, '_');
                            saveTxt(save,x1,y1,x2,y2);
                            write(file, 0, 0, '+');
                            read(file);
                        }else{
                            printf("Wrong no-attack move.\n");
                        }
                    }else {
                        if ((x1 - x2) == 2 && (y2 - y1) == 0 &&
                            (board[(x2 + 1)][y1] == 'w' || board[(x2 + 1)][y1] == 'W')) {
                            write(file, (x1 - 1), y1, '_');                                //straight capture
                            if (x2==1){
                                stampMove(file,x1,y1,x2,y2);
                            }else{
                                write(file, x2, y2, 'b');
                                write(file, x1, y1, '_');
                            }
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '+');
                            }
                            read(file);
                        } else if ((x2 - x1) == 0 && (y2 - y1) == 2 &&
                                   (board[x1][(y1 + 1)] == 'w' || board[x1][(y1 + 1)] == 'W')) {
                            write(file, x2, y2, 'b');
                            write(file, x1, y1, '_');
                            write(file, x1, (y1 + 1),
                                  '_');                                      //left capture from black side
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '+');
                            }
                            read(file);
                            read(file);
                        } else if ((x2 - x1) == 0 && (y1 - y2) == 2 &&
                                   (board[x1][(y2 + 1)] == 'w' || board[x1][(y2 + 1)] == 'W')) {
                            write(file, x1, y1, '_');
                            write(file, x1, (y1 - 1), '_');                  //right capture from black side
                            if (x2 == 1) {
                                write(file, x2, y2, 'B');                               //transform to stamp
                            } else {
                                write(file, x2, y2, 'b');
                            }
                            read(file);
                            if(isAround(x2,y2)==1){                  //--attack again
                                reAtck=1;
                                lastX=y2+'0';
                                lastX=lastX+48;
                                lastY=x2+'0';
                                saveTxt(save,x1,y1,x2,y2);
                            }else{
                                reAtck=0;
                                saveTxt(save,x1,y1,x2,y2);
                                write(file, 0, 0, '+');
                            }
                            read(file);
                        } else {
                            printf("U have to attack.Enter again\n");
                        }
                    }
                }else if(board[x1][y1]=='B'){                                        //-----Stamp Move-------
                    int temp=0,between=0;
                    if(0==isAtk) {
                        if(y2 == y1) {
                            if (x2 > x1) {                                              //Up movement
                                for (int i = (x1+1); i <x2 ; ++i) {
                                    if(board[i][y1]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(x2-x1-1)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '+');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }
                            } else if (x2 < x1) {                                   //Down movement
                                for (int i = (x1-1); i >x2 ; --i) {
                                    if(board[i][y1]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(x1-1-x2)){
                                    stampMove(file,x1,y1,x2,y2);
                                    write(file, 0, 0, '+');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }

                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else if(x2 == x1) {
                            if (y1 > y2) {                                      //Move to the left
                                for (int i = (y1-1); i >y2 ; --i) {
                                    if(board[x1][i]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(y1-1-y2)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '+');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }

                            } else if (y2 > y1) {                           //Move to the right
                                for (int i = (y1+1); i <y2 ; ++i) {
                                    if(board[x1][i]=='_'){
                                        between++;
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==(y2-1-y1)){
                                    stampMove(file,x1,y1,x2,y2);
                                    saveTxt(save,x1,y1,x2,y2);
                                    write(file, 0, 0, '+');
                                    read(file);
                                }else{
                                    printf("There is your pieces or stamps on move way .\n");
                                }

                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else {
                            printf("Your stamp cannot go in this direction.\n");
                        }
                    }else{                                      //if you have to attack
                        if(y2 == y1) {
                            if (x2 > x1) {                                            //capture the top
                                for (int i = (x1+1); i <x2 ; ++i) {
                                    if(board[i][y1]!='b'&&board[i][y1]!='B'){
                                        if(board[i][y1]=='w'||board[i][y1]=='W'){
                                            between++;
                                            temp=i;          //data of the information to be deleted
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, temp, y1, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '+');
                                    }
                                    read(file);
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            } else if (x2 < x1) {                                   //capture the bottom
                                for (int i = (x1-1); i >x2 ; --i) {
                                    if(board[i][y1]!='b'&&board[i][y1]!='B'){
                                        if(board[i][y1]=='w'||board[i][y1]=='W'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, temp, y1, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '+');
                                    }
                                    read(file);
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }
                            }else{
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else if(x2 == x1){
                            if (y1 > y2) {                                          //capture the left
                                for (int i = (y1-1); i >y2 ; --i) {
                                    if(board[x1][i]!='b'&&board[x1][i]!='B'){
                                        if(board[x1][i]=='w'||board[x1][i]=='W'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, x1, temp, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '+');
                                    }
                                    read(file);
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }

                            } else if (y2 > y1) {                               //capture the right
                                for (int i = (y1+1); i <y2 ; ++i) {
                                    if(board[x1][i]!='b'&&board[x1][i]!='B'){
                                        if(board[x1][i]=='w'||board[x1][i]=='W'){
                                            between++;
                                            temp=i;
                                        }
                                    }else{
                                        between=-1;
                                        break;
                                    }
                                }
                                if(between==1){
                                    write(file, x1, temp, '_');
                                    stampMove(file,x1,y1,x2,y2);
                                    read(file);
                                    if(isAround(x2,y2)==1){                  //--attack again
                                        reAtck=1;
                                        lastX=y2+'0';
                                        lastX=lastX+48;
                                        lastY=x2+'0';
                                        saveTxt(save,x1,y1,x2,y2);
                                    }else{
                                        reAtck=0;
                                        saveTxt(save,x1,y1,x2,y2);
                                        write(file, 0, 0, '+');
                                    }
                                    read(file);
                                    read(file);
                                }else if(between==0){
                                    printf("U have to attack.Enter again.\n");
                                }else if(between==-1){
                                    printf("There is your pieces or stamps on move way .\n");
                                }else{
                                    printf("There are many opponent pieces on move way .\n");
                                }
                            } else {
                                printf("Stamps cannot go to own tile .\n");
                            }
                        }else{
                            printf("Your stamp cannot go in this direction.\n");
                        }
                    }
                }else{
                    printf("Your pieces or your stamps not found.\n");
                }
            }else {
                printf("The tile to go is not empty.\n");
            }
        }
    }
    return 0;
}

