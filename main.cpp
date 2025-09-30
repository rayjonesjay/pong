#include <iostream>
#include <raylib.h>
#include <chrono>
#include <thread>

using namespace std;

// the pong game will operate under these dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
int CpuScore = 0;
int PlayerScore = 0;

class Ball
{
	public:
		float x, y;
		int speed_x = 5;
		int speed_y = 5;
		int radius;

	void Draw()
	{
		DrawCircle(x,y,radius,BLACK);
	}

    void Update()
    {
        x+=speed_x;
        y+=speed_y;

        // if the ball touches the upper or lower bounds
        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }

		if(x + radius >= GetScreenWidth()){
			CpuScore++;
			cout << "cpu score is " << CpuScore << endl;
			ResetBallPosition();
		}
        if(x - radius <= 0)
        {
			PlayerScore++;
			cout << "player score is " << PlayerScore << endl;
            ResetBallPosition();
        }
    }

	void ResetBallPosition(){
		x = SCREEN_WIDTH/2;
		y = SCREEN_HEIGHT/2;
		cout << "ball reset done wait 1 seconds";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
};

// paddle will be on right
class Paddle
{

	protected:
	void LimitMovement(){
		if(y <= 0){
			y = 0;
		}

		if(y + height >= GetScreenHeight()){
			y = GetScreenHeight() - height;
		}
	}
	public:
		float x, y, width, height;
		int speed = 5; // default speed is 5(5 pixels on screen)
		Color color = BLACK;

	void Draw(Color color)
	{
		DrawRectangle(x,y,width,height,color);
		// DrawRectangle(SCREEN_WIDTH - 20, SCREEN_HEIGHT/2 - 70/2, 10, 70, BLUE);
	}

	// animate ball movement
	void Update(){
		if(IsKeyDown(KEY_UP))
		{
			y = y - speed;
		}

		if(IsKeyDown(KEY_DOWN))
		{
			y = y + speed;
		}
		LimitMovement();
	}
};


class CpuPaddle: public Paddle{
	public: 
	
	// override the update method used my Paddle class
	// this Update method will be controlled by the computer
	void Update(int ball_y)
	{
		// int upError = GetRandomValue(1,30);
		// int lowError = GetRandomValue(1,30);
		if((y+height/2)> ball_y)
		{
			y-=speed;
		}

		if((y+height/2)<= ball_y){
			y+=speed;
		}
		LimitMovement();
	}
};

Paddle player;
Ball ball;
CpuPaddle cpu;
Color BackgroundColor = Color{203,245,221};

int main()
{
	cout << "ray pong starting..." << endl;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ray pong");
    SetTargetFPS(60);

	// initialize ball
	ball.radius = 20;
	ball.x = SCREEN_WIDTH/2;
	ball.y = SCREEN_HEIGHT/2;

	int DistanceFromEndOfScreen = 20;
	// initialize player
	player.width = 10;
	player.height = 70;
	player.x = SCREEN_WIDTH - DistanceFromEndOfScreen;
	player.y = SCREEN_HEIGHT/2 - player.height/2;
	player.speed = 8;


	// initialize cpu
	cpu.height = 70;
	cpu.width = 10;
	cpu.x = 10;
	cpu.y = SCREEN_HEIGHT/2 - cpu.height/2;
	

	ClearBackground(BackgroundColor);

	while(WindowShouldClose() == false)
	{
		BeginDrawing();

		DrawCircle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,40,WHITE);
        ball.Update();
		player.Update();
		cpu.Update(ball.y);
        
		ClearBackground(BackgroundColor);
        
		ball.Draw();
		player.Draw(BLACK);
		cpu.Draw(BLUE);

		// ball and player collision
		if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
			ball.speed_x *= -1;
			ball.speed_y *= -1;
		}

		// ball and cpu collision
		if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
			ball.speed_x *= -1;
			ball.speed_y *= -1;
		}

		DrawLine(640,0,640,800,BLACK);	
		EndDrawing();	
	}

	// close a window
	CloseWindow();
	
}
