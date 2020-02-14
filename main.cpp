#include <iostream>
#include "console.h"
#include "snake.h"

int main()
{
    Console::GetInstance().Load(50 + 1, 50 + 10, 11);

    Snake snake(50, 50);
    while (snake.IsRunning())
    {
        snake.Input();
        snake.Update();
    }

    Console::GetInstance().SetCursorPos(1, Console::GetInstance().GetRow() + 5);
    system("pause");

    return 0;
}
