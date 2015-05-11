#ifndef _OOTIMER_H
#define _OOTIMER_H

#include "EngineObject.h"
#include "tools.h"

class OOTimer : public EngineObject
{
public:
    OOTimer( unsigned long interval )
        : startTime_( 0 )
        , interval_( interval )
        , expired_( false )
    {
        // NOTHING
    }

    void Start()
    {
        startTime_ = tools::GetCurrentTime();
        expired_ = false;
    }

    void Update()
    {
        if( expired_ )
            return;
        unsigned long elapsed = tools::GetCurrentTime() - startTime_;
        if( elapsed >= interval_ )
            expired_ = true;
    }

    bool IsExpired() const
    {
        return expired_;
    }

private:
    unsigned long startTime_;
    unsigned long interval_;
    bool expired_;
};

#endif // OOTimer.h