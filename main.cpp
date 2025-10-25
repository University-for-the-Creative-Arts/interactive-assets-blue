#include <raylib.h>
#include <math.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
 
    #define INVENTORY_SLOTS_LINES 10
 
    typedef struct Player { //structure of the Player
        Vector2 position; //pos
        Vector2 size;  // size
        Vector2 startPos; //start position
        int Health; //health
    } Player;
    
    typedef struct Block { //The Water Blocks 
        Vector2 position;
        Vector2 size;      
    }Block; 

    // Structure of the walls and the trees/bushes required.
    
    typedef struct Item{ //structure of items

        //lilypad
        Vector2 Lposition;
        Vector2 Lsize;
        bool LActive;
        bool Lcollected;

        //eye
        Vector2 Eposition;
        Vector2 Esize;
        bool EActive;
        bool ECollected;

        //eggs 
        Vector2 Egposition;
        Vector2 Egsize;
        bool EgActive;
        bool EgCollected;
    }Item;
    
    typedef struct pot{ //structure of the pot
        Vector2 position;
        Vector2 size;        
    }Pot;
    
    // initalising the screen
    const int screenWidth = 1300; 
    const int screenHeight = 800;
    const int windowScale = 5;
    int Soundplayed = 0;
    
    bool pause = false; //allow pausing 
    bool gameOver = false; //game over so that if you die, you loose the game
    bool win = false; // to be able to win
    bool gameEnd = false; //to be able to end the game
    bool inventoryVisible = false; //so that i can turn on and off the inventory
    bool colls = false;
    
    static Player player = {0};
    static Block block = {0};
    static Item item = {0};
    static Pot pot = {0};

    void InitGame(void); //function to initalise
    void UpdateGame(void); //function to make movement
    void UpdateDrawFrame(void); //movement + drawing
    void DrawGame(void); //function to draw on screen
    void DrawTime(float time); //function to make timer
    void Collision(int positioningx, int positioningy, int sizingx, int sizingy, int damapot, int hit);
    void ItemCollision(int positioningx, int positioningy, int sizingx, int sizingy, int which);

    Camera2D camera = {0}; //creates a camera.
    

int main(void)  //main thing
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitGame();   
    InitAudioDevice(); //Required for audio
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(screenWidth, screenHeight, "Game!");
    //--------------------------------------------------------------------------------------
    
    //loading textures in
    Texture2D Background = LoadTexture("Game_Things/game_background.png"); //background
    //Texture2D Witch = LoadTexture("Game_Things/witch.png"); //player
    Texture2D Lilypad = LoadTexture("Game_Things/lily_pad.png");
    Texture2D Bigpad = LoadTexture("Game_Things/Big_Lilypad.png");
    Texture2D Pocket = LoadTexture("Game_Things/Pocket.png");
    Texture2D witch = LoadTexture("Game_Things/Witch_Right.png");
    Texture2D Eyes = LoadTexture("Game_Things/Eye.png");
    Texture2D Eggs = LoadTexture("Game_Things/Egg.png");
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;
    int playgame = 0;

    Rectangle frameRec = {0.0f,0.0f, (float)witch.width/4,(float)witch.height};
    // finish 

    // The music
    Sound InvButton = LoadSound("Audio/Inventorybutton.wav");
    
    
    // colour!! 
    Color Mybrown = (Color){101,53,15, 255};
    Color DarkBrown = (Color){46, 21, 3, 255};

    camera.zoom = 2.0f;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateGame(); //runs the update game function 
        camera.target = (Vector2){ player.position.x, player.position.y};  //the camera targets the player position in a specific spot. 
        framesCounter ++;

        if (IsKeyDown(KEY_RIGHT)){
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)witch.width/4;
        }

        // The collision 
        Collision(pot.position.x, pot.position.y, pot.size.x, pot.size.y, 2, 0);
        //water
        Collision(block.position.x - block.size.x,block.position.y - block.size.y, block.size.x, block.size.y, 1,  1); //bottom
        Collision(2250,1600,150, 300, 1, 1); //2
        Collision(2400,1350, 150, 250, 1, 1); //3
        Collision(2550, 1170, 150, 190, 1, 1); //4
        Collision(2670,950,150,150, 1, 1); //5
        Collision(2770,840, 100, 150, 1, 1); //6
        Collision(2770,490, 50, 200, 1, 1); //7
        Collision(2670, 290,110, 150, 1, 1); //8

        //house
        Collision(1760,1160, 180, 50, 0, 0); //counter
        Collision(1940,1160,40,40,0,0); //trash can and door
        Collision(1930,1070,110, 50,0,0); //tub
        Collision(2100,1080,25,90, 0,0); //the toilet/sink
        Collision(2080,1200,40,100, 0,0); //bed
        Collision(1760,1250,50,90,0,0); //pot
        if (item.LActive == true){
            ItemCollision(item.Lposition.x, item.Lposition.y, item.Lsize.x, item.Lsize.y, 1);
        }
        if (item.EActive == true){
            ItemCollision(item.Eposition.x, item.Eposition.y, item.Esize.x, item.Esize.y, 2);
        }
        if (item.EgActive == true){
            ItemCollision(item.Egposition.x, item.Egposition.y, item.Egsize.x, item.Egsize.y,3);
        }
        // End of collisions

            
            if (gameOver == false){  
            
            BeginDrawing();  

                if (playgame == 0){


                    DrawRectangle(0,0, screenWidth, screenHeight, BROWN);

                    DrawText("Witch Game", 0, 200, 50, WHITE);

                    DrawText("Your job is is simple! You are immortal however your newly adopted cat is not. You must find the required items to make her immortal along with you!", 0,200, 25, WHITE);

                    DrawText("Items Required: Lilypad, Bones, Eggs, An Eye, Salt, Plant Roots", 0, 0 , 25, WHITE);

                    if (IsKeyPressed(KEY_SPACE)){

                        playgame = 1;
                    }
                }

            
                BeginMode2D(camera);
                
                ClearBackground(RAYWHITE);
                
                // The drawing section
                
                DrawRectangle(pot.position.x, pot.position.y, pot.size.x, pot.size.y, GRAY);
            
                DrawRectangle(block.position.x - block.size.x, block.position.y - block.size.y,block.size.x,block.size.y, BLUE );  

                DrawTexture(Background, 0,0, WHITE);
             
                //DrawTexture(witch, player.position.x, player.position.y, WHITE);
                DrawTextureRec(witch, frameRec, player.position, WHITE);

                
                if (item.LActive) {
                    DrawTexture(Lilypad, item.Lposition.x, item.Lposition.y, WHITE);
                }
                if (item.EActive){
                    DrawTexture(Eyes, item.Eposition.x, item.Eposition.y, WHITE);
                }
                if (item.EgActive){
                    DrawTexture(Eggs, item.Egposition.x, item.Egposition.y, WHITE);
                }
                
                EndMode2D();

                Collision(block.position.x - block.size.x,block.position.y - block.size.y, block.size.x, block.size.y, 1,  1); //bottom

                DrawTexture(Pocket, 50, 25, WHITE);

                DrawText("Inventory", 50, 120, 20, BLACK);
                
                DrawText(TextFormat("Health: %i", player.Health), 1100, 10, 30, BLACK);
                
                float resetTime = 0;
                int currentTime = (GetTime() - resetTime);
                if (gameOver)
                {resetTime = GetTime();}
                
                DrawTime(currentTime);
                
                //if(!pause) current += 1.f/60;
                
                if (IsKeyPressed('I'))
                {
                  inventoryVisible = !inventoryVisible;  
                  pause = !pause;
                  Soundplayed = 1;
                  
                }
                if (inventoryVisible) {
                    
                    if (Soundplayed == 1)
                    {
                        PlaySound(InvButton);
                        Soundplayed = 0;
                    }
                    
                    DrawRectangle(50, 50, 1200, 400, DarkBrown);
                    DrawText("Inventory", GetScreenWidth()/2 - MeasureText("Inventory", 40)/2, GetScreenHeight()/8 - 10, 40, WHITE);
                    
                    for (int j = 0; j < INVENTORY_SLOTS_LINES; j++){
                        
                        DrawRectangle(105 + 110*j, (screenHeight/2) - 250,100,100, Mybrown);
                        
                        for (int i = 0; i < INVENTORY_SLOTS_LINES; i++){
                            DrawRectangle(105 + 110*i, (screenHeight/2) - 100,100,100, Mybrown);
                        
                        }
                    }
                if (item.Lcollected){
                    DrawTexture(Bigpad, 105, (screenHeight/2) - 250, WHITE);
                }
                }                        
                if (pause && !inventoryVisible) {             
                    DrawText("The game has been paused", GetScreenWidth()/4 - MeasureText("The game has been paused", 10)/2 + 110, GetScreenHeight()/16,40,BLACK);
                    
                }

                
                
            EndDrawing();   
            
            if (player.Health == 0){
                
                gameOver = !gameOver;
            }
            }
            
    
            else if (gameOver == true){    
                
                if (win == true){
                    BeginDrawing();
                    ClearBackground(RAYWHITE);

                    DrawText("You won!", 600, 20, 30, BLACK);

                    DrawText("Play again? [Y]/[N]", 500, 400, 30, BLACK);

                    if (IsKeyPressed('Y')){
                        InitGame();
                        camera.zoom = 2.0f;
                        gameOver = !gameOver;
                        win = false;

                    }

                    else if (IsKeyPressed('N')){
                        UnloadTexture(Background);
                        UnloadTexture(witch);
                        UnloadSound(InvButton);
                        UnloadTexture(Eggs);
                        UnloadTexture(Eyes);
                        //UnloadTexture(LilyPad);
                        CloseWindow();
                        
                    }
                    EndDrawing();
                }
                else {       
                    BeginDrawing();
               
                    ClearBackground(RAYWHITE);
               
                    DrawText("You lost!", 600, 20, 30, BLACK);
               
                    DrawText("Play Again? [Y]/[N]", 500, 400, 30, BLACK);
               
                    if (IsKeyPressed('Y')){
                        InitGame();
                        camera.zoom = 2.0f;
                        gameOver = !gameOver;              
                    }
                    else if (IsKeyPressed('N')){
                   
                        UnloadTexture(Background);
                        UnloadTexture(witch);
                        UnloadSound(InvButton);
                        UnloadTexture(Eggs);
                        UnloadTexture(Eyes);
                        CloseWindow();
                    }
                EndDrawing();            
            }
        }
        
        
    }
    
    CloseWindow();
    

    return 0;
}

void InitGame(void){ //initalises the game
    player.position = (Vector2){1900, 1300};
    player.size = (Vector2){40, 40}; 
    block.position = (Vector2){2250, 2210};
    block.size = (Vector2){490,310};
    player.Health = 100;
    pot.position = (Vector2){1780,1330};
    pot.size = (Vector2){60,60};

    //lilypad
    item.Lposition = (Vector2){2940, 900};
    item.Lsize = (Vector2){40,40};
    item.LActive = true;
    item.Lcollected = false;

    // win condition
    win = false;

    //Eyes
    item.EActive = true;
    item.ECollected = false;
    item.Eposition = (Vector2){340, 2890};
    item.Esize = (Vector2){40,40};

    //eggs
    item.EgActive = true;
    item.EgCollected = false;
    item.Egposition = (Vector2){2570, 350};
    item.Egsize = (Vector2){40,40};
    

    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f}; //to allow the camera to be offset by 2.0 float
    camera.rotation = 0.0f; //the rotation of the camera
    camera.zoom = 1.0f; //zoom of the camera.
}

void UpdateGame(void){ //updates movement 

    player.startPos = player.position;
    
    if (!gameOver)
    {
        if (IsKeyPressed('P')) {
           pause = !pause;
           }
    
        if (!pause){
            
            if (IsKeyDown(KEY_LEFT)) 
                player.position.x -= 3;
            if (IsKeyDown(KEY_RIGHT)) 
                player.position.x += 3;
            if (IsKeyDown(KEY_UP))
                player.position.y -= 3;
            if (IsKeyDown(KEY_DOWN))
                player.position.y += 3;                           
        }
        
    }
    
}

void DrawTime(float time){  //so that i can make a timer run, it grabs the time from intilitialising
    
    int seconds = (int) time % 60; //makes seconds, that is the time modulus of 60, to find the remainder
    
    int minutes = (int) time /60; //makes minutes, that is the time divided by 60 so it finds the whole number
    
    DrawText(TextFormat("Timer: %i : %i", minutes, seconds), 600, 10, 20, BLACK); //displays texts, minutes:seconds, the position, size, colour.

}

void Collision(int positioningx, int positioningy, int sizingx, int sizingy, int damapot, int hit){

    Rectangle boxA = {player.position.x, player.position.y, player.size.x, player.size.y};
    Rectangle boxB = {positioningx, positioningy, sizingx, sizingy};

    if (CheckCollisionRecs(boxA, boxB)){
            if (player.startPos.x + player.size.x <= positioningx){ //player-right, wall-left
                player.position.x = positioningx - player.size.x;
                    switch(damapot){
                        case 1:
                            player.position.x = player.position.x - 40;
                            player.Health -= 10;
                            break;
                        case 2:
                            if (item.LActive == false && item.EActive == false && item.EgActive == false){
                                gameOver = !gameOver;
                                win = !win;
                            }
                            break;
                    }
                    switch(hit){
                        case 1:
                        {
                            Sound HitWater = LoadSound("Audio/Waterhit.wav");
                            PlaySound(HitWater);
                            break;}
                        case 2:{
                            Sound HitBush = LoadSound("Audio/Bushhit.wav");
                            PlaySound(HitBush);
                            break;
                        }

                    }
                    
        }
            if (player.startPos.x >= positioningx + sizingx){ //player-left, wall-right
                player.position.x = positioningx + sizingx;
                switch(damapot){
                        case 1:
                            player.position.x = player.position.x + 40;
                            player.Health -= 10;
                            break;
                        case 2:
                            if (item.LActive == false && item.EActive == false && item.EgActive == false){
                                gameOver = !gameOver;
                                win = !win;
                            }
                            break;
                    }
                    switch(hit){
                        case 1:
                        {
                            Sound HitWater = LoadSound("Audio/Waterhit.wav");
                            PlaySound(HitWater);
                            break;}
                        case 2:{
                            Sound HitBush = LoadSound("Audio/Bushhit.wav");
                            PlaySound(HitBush);
                            break;
                        }

                    }
        }
            if (player.startPos.y + player.size.y <= positioningy){ //player-bottom, wall-top THIS ONE WORKS
                player.position.y = positioningy - player.size.y; //the player position becomes the block's position minus the player's 
                switch(damapot){
                        case 1:
                            player.position.y = player.position.y;
                            player.Health -= 10;
                            break;
                        case 2:
                            if (item.LActive == false && item.EActive == false && item.EgActive == false){
                                gameOver = !gameOver;
                                win = !win;
                            }
                            break;
                    }
                switch(hit){
                        case 1:
                        {
                            Sound HitWater = LoadSound("Audio/Waterhit.wav");
                            PlaySound(HitWater);
                            break;}
                        case 2:{
                            Sound HitBush = LoadSound("Audio/Bushhit.wav");
                            PlaySound(HitBush);
                            break;
                        }

                    }
        } 
            if (player.startPos.y >= positioningy + sizingy){ //player-top, wall-bottom
                player.position.y = positioningy + sizingy;
                switch(damapot){
                        case 1:
                            player.position.y = player.position.y;
                            player.Health -= 10;
                            break;
                        case 2:
                            if (item.LActive == false && item.EActive == false && item.EgActive == false){
                                gameOver = !gameOver;
                                win = !win;
                            }
                            break;
                    }
                    switch(hit){
                        case 1:
                        {
                            Sound HitWater = LoadSound("Audio/Waterhit.wav");
                            PlaySound(HitWater);
                            break;}
                        case 2:{
                            Sound HitBush = LoadSound("Audio/Bushhit.wav");
                            PlaySound(HitBush);
                            break;
                        }

                    }
        }
    }
}

void ItemCollision(int positioningx, int positioningy, int sizingx, int sizingy, int which){
    
    Rectangle boxA = {player.position.x, player.position.y, player.size.x, player.size.y};
    Rectangle boxB = {positioningx, positioningy, sizingx, sizingy};
    
    if (CheckCollisionRecs(boxA, boxB)){  
        Sound Pickup = LoadSound("Audio/Pickup.wav");
            if (player.startPos.x + player.size.x <= positioningx){ //player-right, wall-left
                switch(which){
                    case 1:
                        item.LActive = false;
                        item.Lcollected = true;
                        PlaySound(Pickup);
                        break;
                    case 2:{
                        item.EActive = false;
                        item.ECollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                    case 3:{
                        item.EgActive = false;
                        item.EgCollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                }
        }
            if (player.startPos.x >= positioningx + sizingx){ //player-left, wall-right
               switch(which){
                    case 1:
                        item.LActive = false;
                        item.Lcollected = true;
                        PlaySound(Pickup);
                        break;
                    case 2:{
                        item.EActive = false;
                        item.ECollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                    case 3:{
                        item.EgActive = false;
                        item.EgCollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                }
        }
            if (player.startPos.y + sizingy <= positioningy){ //player-bottom, wall-top THIS ONE WORKS
               switch(which){
                    case 1:
                        item.LActive = false;
                        item.Lcollected = true;
                        PlaySound(Pickup);
                        break;
                    case 2:{
                        item.EActive = false;
                        item.ECollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                    case 3:{
                        item.EgActive = false;
                        item.EgCollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                }
        } 
            if (player.startPos.y >= positioningy + sizingy){ //player-top, wall-bottom
               switch(which){
                    case 1:
                        item.LActive = false;
                        item.Lcollected = true;
                        PlaySound(Pickup);
                        break;
                    case 2:{
                        item.EActive = false;
                        item.ECollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                    case 3:{
                        item.EgActive = false;
                        item.EgCollected = true;
                        PlaySound(Pickup);
                        break;
                    }
                }
        }   
    }
}