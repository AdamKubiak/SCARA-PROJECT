#include "sequencehandler.h"

SequenceHandler::SequenceHandler()
{

}

void SequenceHandler::addSeqPosition(float j1, float j2, float j3)
{
    this->positions.push_back(std::make_tuple(j1,j2,j3));
}

void SequenceHandler::clearSequence()
{
    this->positions.clear();
}

int SequenceHandler::getSize()
{
    return this->positions.size();
}

std::vector<std::tuple<float, float,float>> SequenceHandler::getList()
{
    return this->positions;
}



