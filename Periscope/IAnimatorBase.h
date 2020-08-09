#ifndef IANIMATORBASE_H
#define IANIMATORBASE_H

class IAnimatorBase {
    public:
        virtual void processCommand(char commandBuffer[], unsigned char commandLength) {}
        virtual void update(unsigned long currentTime) {}

        virtual void stop() {}
};

#endif
