#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


typedef enum {
    MENU,          
    PLAY_COMPUTER, 
    PLAY_FRIEND    
} GameState;

void DrawHangman(int livesRemaining)
{
    DrawLineEx((Vector2){100, 450}, (Vector2){300, 450}, 5.0f, BLACK);  
    DrawLineEx((Vector2){200, 450}, (Vector2){200, 150}, 5.0f, BLACK);  
    DrawLineEx((Vector2){200, 150}, (Vector2){300, 150}, 5.0f, BLACK);                                                                  
    DrawLineEx((Vector2){300, 150}, (Vector2){300, 200}, 5.0f, BLACK); 

    if (livesRemaining <= 5) DrawCircleLines(300, 230, 30, BLACK);  
    if (livesRemaining <= 4) DrawLine(300, 260, 300, 340, BLACK);  
    if (livesRemaining <= 3) DrawLine(300, 280, 260, 320, BLACK);  
    if (livesRemaining <= 2) DrawLine(300, 280, 340, 320, BLACK);  
    if (livesRemaining <= 1) DrawLine(300, 340, 260, 400, BLACK);  
    if (livesRemaining <= 0) DrawLine(300, 340, 340, 400, BLACK); 
}


int main(void)
{
    InitWindow(900, 600, "Adam Asmaca");
    SetTargetFPS(60);
    
    SetExitKey(KEY_NULL); //herhangi bir tusla ekranın kapanmamasını sağliyor asıl amacımızda esc ile oyundan cıkmasın diye
    
    srand(time(NULL)); //rastgele sayı üretmek icin bu fonksiyonu cagırmak zorundayiz
    
    GameState gameState = MENU;
    
    Rectangle btnComputer = {300, 220, 250, 60};
    Rectangle btnFriend = {300, 300, 250, 60};
    Rectangle btnExit = {300, 380, 250, 60};
    
    char *p4[] = {"IMGE","AZOT","ETIK","MAYA","KAPI","OYUN","ISIK","DERS","YAZI","KAYA"};
    char *p4Clue[] = {"HAYAL","GAZ","AHLAK","HAMUR","GIRIS","EGLENCE","AYDINLIK","OKUL","METIN","TAS"};

    char *p5[] = {"RAKIM","LODOS","SOMUT","KITAP","DENIZ","SINAV","YAZAR","BILGI","TAHTA","SIYAH"};
    char *p5Clue[] = {"YUKSEKLIK","RUZGAR","VARLIK","OKUMA","MAVI","OKUL","KALEM","OGRENME","AGAC","RENK"};

    char *p6[] = {"VICDAN","SILUET","BOZKIR","AHESTE","KAMERA","OYUNCU","YAZICI","BARDAK","TAKSIT","SANDIK"};
    char *p6Clue[] = {"ICSES","GOLGE","COGRAFYA","YAVAS","CEKIM","ROL","CIKTI","ICECEK","ODEME","KUTU"};

    char *p7[] = {"LABIRENT","KARGASA","IHTIMAL","SENARYO","CELISKI","DUSUNCE","BAGINTI","ONCULUK","CIKARIM","OZNELCI"};
    char *p7Clue[] = {"CIKMAZ","KAOS","OLASILIK","KURGU","TUTARSIZLIK","FIKIR","ILISKI","LIDERLIK","SONUC","YORUMCU"};

    char **pLevels[] = {p4,p5,p6,p7};
    char **pClues[]  = {p4Clue,p5Clue,p6Clue,p7Clue};

    
    int level = 0;
    int livesRemaining = 6;
    int usedLetters[26] = {0};
    char *chosenWord = NULL;
    char *chosenClue = NULL;
    char displayWord[32];
    int wordLength = 0;
    int wordFinished = 0;
    
    static char writtenWord[50] = {0};
    static char writtenClue[100] = {0};
    static int wordLengthFriend = 0;
    static char displayWordFriend[50] = {0};
    static int livesRemainingFriend = 6;
    static int usedLettersFriend[26] = {0};
    static int wordFinishedFriend = 0;
    static int enteringWord = 1;
    static int enteringClue = 0;
    
    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        
        if (gameState == MENU)
        {    
            DrawText("Adam Asmaca", 300, 120, 40,BLACK);
            DrawText("Oyununa Hosgeldiniz!", 230,160,40,BLACK);
            Color c1 = CheckCollisionPointRec(mouse, btnComputer) ? RED : LIGHTGRAY;
            Color c2 = CheckCollisionPointRec(mouse, btnFriend) ? RED : LIGHTGRAY;
            Color c3 = CheckCollisionPointRec(mouse, btnExit) ? RED : LIGHTGRAY;
            
            
            DrawRectangleRec(btnComputer, c1);
            DrawRectangleRec(btnFriend, c2);
            DrawRectangleRec(btnExit, c3);
            
            DrawText("Bilgisayara karsi OYNA", 308, 240, 20, BLACK);
            DrawText("Arkadasinla OYNA",335, 320, 20, BLACK);
            DrawText("Cikis", 405, 400, 20,BLACK);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, btnComputer))
                {
                    level = 0;
                    livesRemaining = 6;
                    memset(usedLetters, 0, sizeof(usedLetters));
                    chosenWord = NULL;
                    gameState = PLAY_COMPUTER;
                }//if satir:107
                else if(CheckCollisionPointRec(mouse, btnFriend))
                {
                    livesRemainingFriend = 6;
                    memset(usedLettersFriend, 0, sizeof(usedLettersFriend));
                    wordFinishedFriend = 0;
                    writtenWord[0] = '\0';
                    writtenClue[0] = '\0';
                    gameState = PLAY_FRIEND;
                    
                }//else if satir:115
                else if(CheckCollisionPointRec(mouse, btnExit))
                {
                    break;
                  
                }//else if sati:125
            }//if satir:105

        }//if satir:88
        else if(gameState == PLAY_COMPUTER)
        {
            if (livesRemaining<=0)
            {
                ClearBackground(RAYWHITE);
                DrawText("OYUN BITTI !", 340, 240, 30, RED);
                DrawText(TextFormat("Bilemediginiz kelime: %s", chosenWord), 310, 280, 20, BLACK);
                DrawText("Menuye donmek icin ESC ye basiniz", 280, 320, 20, LIGHTGRAY);
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    chosenWord = NULL;
                    gameState = MENU;
                }//if satir:141
                EndDrawing();
                continue;
            }//if satir 135
            if (chosenWord == NULL)
            {
                int r = rand() % 10;
                chosenWord = pLevels[level][r];
                chosenClue = pClues[level][r];
                wordLength = strlen(chosenWord);
                for (int i=0;i<wordLength;i++) displayWord[i] = '_';
                displayWord[wordLength] = '\0';
                livesRemaining = 6;
                memset(usedLetters, 0, sizeof(usedLetters));
                wordFinished = 0;
            }//if satir:149
            DrawText(TextFormat("LEVEL %d", level + 1), 360, 10, 30, BLACK);
            DrawText(TextFormat("Ipucu: %s",chosenClue),550, 90, 20, DARKGRAY);
            DrawText(TextFormat("Kalan can: %d",livesRemaining), 10, 10, 20, RED);
            
            DrawHangman(livesRemaining);
            DrawText(displayWord, 550, 420, 35, ORANGE);
            
            int key = GetKeyPressed();
            
            if (key >= 'A' && key <= 'Z')
            {
                int index = key - 'A';
                if (!usedLetters[index])
                {
                    usedLetters[index] = 1;
                    int found = 0;
                    for (int j=0;j<wordLength;j++)
                    
                        if(chosenWord[j] == key)
                        {
                            displayWord[j] = key;
                            found = 1;
                        }//if satir:179
                    if (!found) livesRemaining--;
                    
                }//if satir:173
            }//if satir:170
            
            wordFinished = 1;
            for (int k=0;k<wordLength;k++)
            {
                if (displayWord[k] == '_') wordFinished = 0;
            }//for satir:190
            if (wordFinished)
            {
                level++;
                chosenWord = NULL;
                if (level>=4)
                    gameState = MENU;
            }//if satir:194
            
            if (IsKeyPressed(KEY_ESCAPE))
            {
                chosenWord = NULL;
                gameState = MENU;
            }//if satir:202
        }//else if satir:133
        else if (gameState == PLAY_FRIEND)
        {
            if (wordFinishedFriend)
            {  
                ClearBackground(RAYWHITE);
                DrawText("BRAVO!! Dogru bildiniz",285,240,30,GREEN);
                DrawText("Menuye donmek icin ESC ye basiniz",277,300,20,BLACK);
                DrawText("Bir daha oynamak icin 'R' ye basiniz",277,360,20,BLACK);
                    
                if(IsKeyPressed(KEY_ESCAPE))
                {
                    enteringWord = 1;
                    enteringClue = 0;
                    wordLengthFriend = 0;
                    livesRemainingFriend = 6;
                    memset(usedLettersFriend, 0, sizeof(usedLettersFriend));
                    wordFinishedFriend = 0;
                    writtenWord[0] = '\0';
                    writtenClue[0] = '\0';
                    displayWordFriend[0] = '\0';
                    gameState = MENU;
                }//if satir:217
                    
                else if(IsKeyPressed(KEY_R))
                {
                    enteringWord = 1;
                    enteringClue = 0;
                    wordLengthFriend = 0;
                    livesRemainingFriend = 6;
                    memset(usedLettersFriend, 0, sizeof(usedLettersFriend));
                    wordFinishedFriend = 0;
                    writtenWord[0] = '\0';
                    writtenClue[0] = '\0';
                    displayWordFriend[0] = '\0';
                }//else if satir:231
                EndDrawing();
                continue;
                
            }
     
            
            if (livesRemainingFriend <= 0)
            {
                ClearBackground(RAYWHITE);
                DrawText("OYUN BITTI !",340,240,30,RED);
                DrawText(TextFormat("Arkadasinin kelimesi: %s",writtenWord),310,300,20,DARKGRAY);
                DrawText("Menuye gitmek icin ESC basiniz", 290,360,20,DARKGRAY);
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    enteringWord = 1;
                    enteringClue = 0;
                    livesRemainingFriend = 6;
                    memset(usedLettersFriend, 0, sizeof(usedLettersFriend));
                    wordFinishedFriend = 0;
                    writtenWord[0] = '\0';
                    writtenClue[0] = '\0';
                    gameState = MENU;
                }//if satir:255
                EndDrawing();
                continue;
            }//if satir:249
            
            if (enteringWord)
            {
                DrawText("Kelimenizi yazin ve ENTER'a basiniz",270,200,20,BLACK);
                
                int key = GetCharPressed();
                
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    int len = strlen(writtenWord);
                    if(len > 0) writtenWord[len - 1] = '\0'; //==> son karakteri sil
                }//if satir:276
                
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    writtenWord[0] = '\0';
                    enteringWord = 0;
                    gameState = MENU;
                }//if satir:282
                
                if (IsKeyPressed(KEY_ENTER))
                {
                        
                    enteringWord = 0;
                    enteringClue = 1;
                    wordLengthFriend = strlen(writtenWord);
                    for (int i=0;i<wordLengthFriend;i++)
                    {    
                        displayWordFriend[i] = '_';
                        displayWordFriend[wordLengthFriend] = '\0';
                    }//for satir:295
                }//if satir:289
                while (key>0)
                {

                    if(((key >= 65 && key <= 90) || (key >=97 && key <= 122)) && strlen(writtenWord) < 50)
                    {
                        int len = strlen(writtenWord);
                        writtenWord[len] = (char)toupper(key);
                        writtenWord[len+1] = '\0';
                    }//else if satir:304
                    key = GetCharPressed();
                }//while satir:301
                DrawText(writtenWord, 380, 240, 30, BLACK);
            }//if satir:270
            
            else if (enteringClue)
            {
                DrawText("Kelimeniz icin tek kelimelik bir ipucu giriniz ve ENTER'a basiniz",130,200,20,BLACK);
                int key = GetCharPressed();
                
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    int len = strlen(writtenClue);
                    if (len > 0) writtenClue[len-1] = '\0';
                }//if satir:320
                
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    writtenClue[0] = '\0';
                    enteringWord = 0;
                    enteringClue = 0;
                    gameState = MENU;
                }//if satir:326
                if (IsKeyPressed(KEY_ENTER)) 
                {    
                    enteringClue = 0;
                }//if satir:333
                
                while (key > 0)
                {

                    if(((key >= 65 && key <= 90) || (key >=97 && key <= 122)) && strlen(writtenClue) < 100)
                    {
                        int len = strlen(writtenClue);
                        writtenClue[len] = (char)toupper(key);
                        writtenClue[len+1] = '\0';
                    }//if satir:341
                    key = GetCharPressed();
                }//while satir:338
                DrawText(writtenClue, 370, 240, 30,BLACK);
                if (IsKeyPressed(KEY_ENTER))
                    enteringClue = 0;
            }//else if satir:315
            else
            {
                DrawText(TextFormat("Ipucu: %s",writtenClue), 550,90,20, DARKGRAY);
                DrawText(TextFormat("Kalan can: %d", livesRemainingFriend),10,10,20,RED);
                DrawHangman(livesRemainingFriend);
                DrawText(displayWordFriend, 550, 420, 35, ORANGE);
                
                int key = GetKeyPressed();
                key = toupper(key);
                if (key >= 'A' && key <= 'Z')
                {
                    int index = key - 'A';
                    if (!usedLettersFriend[index])
                    {
                        usedLettersFriend[index] = 1;
                        int found = 0;
                        for (int i=0;i<wordLengthFriend;i++)
                            if (writtenWord[i] == key)
                            {
                                displayWordFriend[i] = key;
                                found = 1;
                            }//if satir:370
                        if (!found) livesRemainingFriend--;
                    }//if satir:365
                }//if satir:362
                
                wordFinishedFriend = 1;
                for (int i=0;i<wordLengthFriend;i++)
                {
                    if (displayWordFriend[i] != writtenWord[i])
                        wordFinishedFriend = 0;
                }//for satir:380
                
            }//else satir:353
            
        }//else if satir:208
        EndDrawing();
    }//while satir:81
    CloseWindow();  
    
    return 0;
}//int main(void)






























































































































































