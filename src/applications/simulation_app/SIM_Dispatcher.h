// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SIM_Dispatcher_h_
#define __SIM_Dispatcher_h_

#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#undef Yield // $$$$ AGE 2005-03-02: ....
#include "tools/thread/Thread.h"
#include "dispatcher/Dispatcher.h"

class SIM_Config;

// =============================================================================
/** @class  SIM_Dispatcher
    @brief  SIM_Dispatcher
*/
// Created: NLD 2006-10-04
// =============================================================================
class SIM_Dispatcher : public tools::thread::Thread
{
public:
    //! @name Constructors/Destructor
    //@{
             SIM_Dispatcher( SIM_Config& config );
    virtual ~SIM_Dispatcher();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SIM_Dispatcher( const SIM_Dispatcher& );            //!< Copy constructor
    SIM_Dispatcher& operator=( const SIM_Dispatcher& ); //!< Assignement operator
    //@}

    //! @name Thread Main Loop
    //@{
    virtual void Run();
    virtual void OnUnexpected( Win32Exception& e );
    virtual void OnUnexpected( std::exception& e );
    virtual void OnUnexpected();
    //@}

private:
    dispatcher::Dispatcher dispatcher_;
};


#endif // __SIM_Dispatcher_h_
