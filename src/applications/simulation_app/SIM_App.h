// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 MASA Group
//
// *****************************************************************************

#ifndef __SIM_App_h_
#define __SIM_App_h_

#include "tools/Application.h"

class MIL_Config;
class MT_FileLogger;

// =============================================================================
/** @class  SIM_App
    @brief  Application facade
*/
// Created: NLD 2002-08-07
// =============================================================================
class SIM_App : public tools::Application
{
public:
     SIM_App( int maxConnections );
    ~SIM_App();

private:
    virtual void Initialize();
    virtual bool Update();

private:
    const int maxConnections_;
    std::unique_ptr< MIL_Config > config_;
    std::unique_ptr< MT_FileLogger > logger_;
    std::unique_ptr< boost::thread > dispatcher_;
};

#endif // __SIM_App_h_
