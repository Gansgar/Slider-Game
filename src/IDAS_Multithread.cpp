#include <atomic>
#include "IDAS_Multithread.h"

//
// Created by Georg Friedrich on 15/05/16.
//
std::atomic_int IDAS_Multithread::moves(0);
std::atomic_int IDAS_Multithread::currentID(0);
std::atomic_int IDAS_Multithread::lastFinished(-1);
std::future<int> IDAS_Multithread::ret ;