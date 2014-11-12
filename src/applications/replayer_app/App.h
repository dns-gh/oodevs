// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 MASA Group
//
// *****************************************************************************

#ifndef __App_h_
#define __App_h_

#include "tools/Application.h"

class MT_FileLogger;

namespace dispatcher
{
    class Config;
    class Replayer;
}

// =============================================================================
/** @class  App
    @brief  My first application
*/
// Created: AGE 2007-04-10
// =============================================================================
class App : public tools::Application
{
public:
     App( bool replayLog );
    ~App();

private:
    virtual void Initialize();
    virtual bool Update();

private:
    std::unique_ptr< MT_FileLogger > logger_;
    std::unique_ptr< dispatcher::Config > config_;
    std::unique_ptr< dispatcher::Replayer > replayer_;
    bool test_;
};

#endif // __App_h_
