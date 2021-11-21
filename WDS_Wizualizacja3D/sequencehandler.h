#ifndef SEQUENCEHANDLER_H
#define SEQUENCEHANDLER_H
#include <vector>
#include <tuple>
#include <QTime>
#include <QCoreApplication>

class SequenceHandler
{
private:
    std::vector<std::tuple<float, float,float>> positions;
public:
    SequenceHandler();
    void addSeqPosition(float j1,float j2, float j3);
    void clearSequence();
    int getSize();
    std::vector<std::tuple<float, float,float>> getList();
    void delay()
    {
        QTime dieTime= QTime::currentTime().addSecs(4);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 4000);
    }
    int index;
};

#endif // SEQUENCEHANDLER_H
