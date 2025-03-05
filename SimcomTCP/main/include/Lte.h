#ifndef LTE_H
#define LTE_H

class Lte
{
private:
public:
    Lte();
    ~Lte();

    bool disconnect();
    bool connect();
    bool connected();
};

#endif // LTE_H