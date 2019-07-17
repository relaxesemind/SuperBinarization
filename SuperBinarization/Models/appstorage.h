#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"


class AppStorage : SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage() = default;

public:

};

#endif // APPSTORAGE_H
