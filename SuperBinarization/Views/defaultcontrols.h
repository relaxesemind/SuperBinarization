#ifndef DEFAULTCONTROLS_H
#define DEFAULTCONTROLS_H


#include <QString>
#include "Abstract/singletoneclass.h"
#include "Common/magic.h"


class DefaultControls : public SingletonClass<DefaultControls>
{
    friend class SingletonClass<DefaultControls>;

private:
    DefaultControls() = default;

public:
    template<class Action> void showConfirmDialog(const QString& title, Action okAction);
                           void showMessage(const QString& title, const QString& description);
};



#endif // DEFAULTCONTROLS_H
