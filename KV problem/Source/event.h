#pragma once

int E = -1; //все возможные события программы
enum
{
    noEvent       =-1,
    dragNode      = 1,
    addNode       = 2,
    clearScreen   = 3,
    connect       = 4,
    setConnection = 5,
    getAnswer     = 6,
    runTask       = 7
};
