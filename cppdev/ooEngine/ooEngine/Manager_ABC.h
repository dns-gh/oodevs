#ifndef _MANAGER_ABC_H_
#define _MANAGER_ABC_H_

class Manager_ABC
{
public:
    virtual ~Manager_ABC(){};
    virtual bool Update() = 0;
};

#endif // Manager_ABC.h