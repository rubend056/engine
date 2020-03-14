#ifndef Component_h
#define Component_h

class Component {
    public:
        bool enabled = true;
        virtual int max_num() = 0;
};

#endif