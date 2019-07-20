#ifndef MANAGERSLOCATOR_H
#define MANAGERSLOCATOR_H

#include "Abstract/singletoneclass.h"
#include "colorgenerator.h"


class ManagersLocator : public SingletonClass<ManagersLocator>
{
    friend class SingletonClass<ManagersLocator>;
    ManagersLocator() = default;
public:

    ColorGenerator<> colorGenerator;
};

#endif // MANAGERSLOCATOR_H
