/*

    Zelda Navi's Quest

    Copyright (C) 2013-2014  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#ifndef __PROJECTILEHELPER_H__
#define __PROJECTILEHELPER_H__

#include "../types/Projectile.h"

#include "../link/Link.h"

class ProjectileHelper {
    public :
        static ProjectileHelper* getInstance();

        void addProjectile(TypeProjectile type, int x, int y, Direction dir);
        void addProjectile(TypeProjectile type, int x, int y, double dx, double dy, BoundingBox* box = 0);
        void addProjectile(TypeProjectile type, int x, int y, Link* link);

    private :
        ProjectileHelper();
        ~ProjectileHelper();

        static ProjectileHelper instance;

};

#endif  // ProjectileHelper.h
