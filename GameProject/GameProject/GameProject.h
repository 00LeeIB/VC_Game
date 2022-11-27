#pragma once

#include "resource.h"



class ME_Status {
public:
    //½ºÅİ
    int Level;
    int HP;
    int TotalHP;
    int HPregen;

    int Power;
    int AttackSpeed;
    int Speed;
    
    int Range;
    int Defense;

    int Exp;
    int SumExp;
};

class ENEMY_Status {
public:
    int Level;
    int HP;
    int TotalHP;
    int Power;
    int Speed;
    wchar_t HPstr[10];
};

bool collisionEllipseCheck(RECT rc, RECT rc2) {
    float rc_R = (rc.right - rc.left) / 2;
    float rc2_R = (rc2.right - rc2.left) / 2;

    float rX = (rc.left + (rc.right - rc.left) / 2) - (rc2.left + (rc2.right - rc2.left) / 2);
    float rY = (rc.top + (rc.bottom - rc.top) / 2) - (rc2.top + (rc2.bottom - rc2.top) / 2);

    float length = (rc_R + rc2_R) * (rc_R + rc2_R);

    if (length >= (rX * rX + rY * rY)) {
        return true;
    }
    return false;


}

