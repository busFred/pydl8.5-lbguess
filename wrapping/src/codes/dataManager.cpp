//
// Created by Gael Aglin on 2019-12-23.
//

#include "dataManager.h"
#include <cmath>
#include <algorithm>


DataManager::DataManager(int* supports, int ntransactions, int nattributes, int nclasses, int *data, int *target, int *warm):supports(supports), ntransactions(ntransactions), nattributes(nattributes), nclasses(nclasses) {
    nWords = (int)ceil((float)ntransactions/M);
    b = new bitset<M> *[nattributes];
    c = new bitset<M> *[nclasses];
    w = new bitset<M> *[1];

    for (int i = 0; i < nattributes; i++){
        bitset<M> * attrCov = new bitset<M>[nWords];
        for (int j = 0; j < nWords; ++j) {
            int currentindex = -1;
            int* start = data + (ntransactions*i) + (M*j);
            int* end = nullptr;
            if (j != nWords - 1)
                end = data + (ntransactions*i) + (M*j) + M;
            else
                end = data + (ntransactions*i) + ntransactions;
            int dist = 0;

            auto itr = find(start, end, 1);
            while (itr != end && start < end) {
                dist = distance(start, itr);
                currentindex += 1 + dist;
                attrCov[nWords-(j+1)].set(currentindex);
                start += (dist + 1);
                itr = find(start, end, 1);
            }
        }
        b[i] = attrCov;
        //cout << "attr : " << i << " word = " << attrCov->to_string() << endl;
    }


    if (target){
        for (int i = 0; i < nclasses; i++){
            bitset<M> * classCov = new bitset<M>[nWords];
            for (int j = 0; j < nWords; ++j) {
                int currentindex = -1;
                int* start = target + (M*j);
                int* end = nullptr;
                if (j != nWords - 1)
                    end = target + (M*j) + M;
                else
                    end = target + ntransactions;
                int dist = 0;

                auto itr = find(start, end, i);
                while (itr != end && start < end) {
                    dist = distance(start, itr);
                    currentindex += 1 + dist;
                    classCov[nWords-(j+1)].set(currentindex);
                    start += (dist + 1);
                    itr = find(start, end, i);
                }
            }
            c[i] = classCov;
        }
    }
    else
        c = nullptr;

    if (warm){
        bitset<M> * classCov = new bitset<M>[nWords];
        for (int j = 0; j < nWords; ++j) {
            int currentindex = -1;
            int* start = warm + (M*j);
            int* end = nullptr;
            if (j != nWords - 1)
                end = warm + (M*j) + M;
            else
                end = warm + ntransactions;
            int dist = 0;

            auto itr = find(start, end, 1);
            while (itr != end && start < end) {
                dist = distance(start, itr);
                currentindex += 1 + dist;
                classCov[nWords-(j+1)].set(currentindex);
                start += (dist + 1);
                itr = find(start, end, 1);
            }
        }
        w[0] = classCov;
    }
    else
        w = nullptr;

    ::nattributes = nattributes;
    ::nclasses = nclasses;
}

bitset<M>* DataManager::getAttributeCover(int attr) {
    return b[attr];
}

bitset<M>* DataManager::getClassCover(int clas) {
    return c[clas];
}

bitset<M>* DataManager::getWarmCover() {
    if (w == nullptr) {
        return nullptr;
    }
    return w[0];
}
