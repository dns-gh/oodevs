// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProcessObserver_ABC_h_
#define __ProcessObserver_ABC_h_

namespace frontend
{

// =============================================================================
/** @class  ProcessObserver_ABC
    @brief  ProcessObserver_ABC
*/
// Created: SBO 2010-11-10
// =============================================================================
class ProcessObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ProcessObserver_ABC() {}
    virtual ~ProcessObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyStopped() = 0;
    virtual void NotifyError( const std::string& error, std::string commanderEndpoint = "" ) = 0;
    //@}
};

}

#endif // __ProcessObserver_ABC_h_
