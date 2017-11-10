#include "AStar.h"

#include "../../engine/util/geometry/BoundingBox.h"

#include "cstdlib"

#include "../MainController.h"

#include "../game/scene/Scene.h"

AStar AStar::instance=AStar();

AStar::AStar() {
}

AStar::~AStar() {
}

AStar* AStar::getInstance() {
    return &instance;
}

pair<int, int> AStar::resolvePath(Collisionable* object, int dstX, int dstY, Direction direction, int lvl) {

    l_node opened;
    l_node closed;

    BoundingBox* bb = object->getBoundingBox();
    BoundingBox box(bb->getX(), bb->getY(), bb->getW(), bb->getH());
    int x = box.getX();
    int y = box.getY();
    int dx = box.getW() / 2;
    int dy = box.getH();

    BoundingBox dst(dstX - 8, dstY - 16, 16, 16);

    // create and add the start point to the opened list
    pair<int, int> coords(x, y);
    node start;
    start.cost_g = 0;
    start.cost_h = simpleDistance(x + dx, y + dy, dstX, dstY);
    start.cost_f = start.cost_g + start.cost_h;

    opened[coords] = start;

    pair <int, int> current = coords;
    int count = 0;

    node nd = start;
    bool begin = !object->isIdle();
    while (count < lvl && !opened.empty()) {

        // 1 -> retrieve best node from opened list
        current = retrieveBestNode(&opened);
        box.setX(current.first);
        box.setY(current.second);
        nd = opened[current];
        if (nd.cost_h <= 8*2-1 || box.intersect(&dst)) {
            break;
        }

        // 2 -> switch the best node to the closed list
        closed[current] = nd;
        opened.erase(current);

        if (++count >= lvl) {
            //return pair<int, int> (0, 0);
            break;
        }

        // 3 -> add new nodes from the current
        addNodes(&opened, &closed, current, nd, dx, dy, dstX, dstY, object, &box, direction, begin);
        begin = false;
    }

    // 4 -> find the way from the result

    int nx = x;
    int ny = y;

    while (current != coords) {

        nx = current.first;
        ny = current.second;

        current = nd.parent;
        nd = closed[current];
    }

    pair<int, int> result;


    if (nx < x) result.first = -1;
    else if (nx > x) result.first = 1;
    if (ny < y) result.second = -1;
    else if (ny > y) result.second = 1;

    return result;
}

int AStar::simpleDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

pair <int, int> AStar::retrieveBestNode(l_node* list) {

    pair <int, int> node = list->begin()->first;
    int cost = list->begin()->second.cost_f;

    for (l_node::iterator i = list->begin(); i!=list->end(); i++) {
        if (i->second.cost_f< cost){
            cost = i->second.cost_f;
            node = i->first;
        }
    }
    return node;
}

void AStar::addNodes(l_node* opened, l_node* closed, pair <int, int> current, node nd,
                     int dx, int dy, int dstX, int dstY, Collisionable* object,
                     BoundingBox* box, Direction direction, bool begin) {

    node tmp;

    Scene* scene = MainController::getInstance()->getGameController()->getSceneController()->getScene();

    int x0 = current.first;
    int y0 = current.second;

    int firstX = x0;
    int firstY = y0;
    int lastX;
    int lastY;

    int mod = firstX % 8;
    if (mod != 0) {
        firstX -= mod;
        lastX = firstX + 8;
    } else {
        firstX -= 8;
        lastX = firstX + 16;
    }

    mod = firstY % 8;
    if (mod != 0) {
        firstY -= mod;
        lastY = firstY + 8;
    } else {
        firstY -= 8;
        lastY = firstY + 16;
    }

    for (int j = firstY; j <= lastY; j += 8) {
        for (int i = firstX; i <= lastX; i += 8) {

            if (i == x0 && j == y0) {
                continue;
            }

            box->setX(i);
            box->setY(j);
            if (!scene->checkCollisions(box, object, true, true, false)) {
                continue;
            }

            int surcost = 0;
            if (!scene->checkCollisions(box, object, false, true, true)) {
                surcost += 999; // <- should never want to take this path
            }

            pair<int, int> it(i, j);

            if (closed->find(it) == closed->end()) {

                //int surcost = 0;
                if (begin) {
                    if ( (j == firstY && direction == S)
                        || (j == lastY && direction == N)
                        || (i == firstX && direction == E)
                        || (i == lastX && direction == W)) {
                        surcost += 32;
                    }
                }


                tmp.cost_g = nd.cost_g + (simpleDistance(i, j, x0, y0) / 2);
                tmp.cost_h = simpleDistance(i + dx, j + dy, dstX, dstY) + surcost;
                tmp.cost_f = tmp.cost_g + tmp.cost_h;
                tmp.parent = current;

                if (opened->find(it) != opened->end()) {
                    if (tmp.cost_f < (*opened)[it].cost_f) {
                        (*opened)[it] = tmp;
                    }
                } else {

                    (*opened)[pair<int, int> (i, j)] = tmp;

                }
            }
        }
    }

}
