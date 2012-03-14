// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef PROCESS_H__
#define PROCESS_H__
#include "../Process_ABC.h"
#include <memory>

namespace process
{

class Api_ABC;
class Handle;

// =============================================================================
/** @class  Process
    @brief  Win32 process class definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Process : public process::Process_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    Process( const Api_ABC& api, int identifier );
    Process( const Api_ABC& api, int identifier, std::auto_ptr< Handle > handle );
    virtual ~Process();
    //@}

    //! @name Operations
    //@{
    virtual int GetPid() const;
    virtual const std::string& GetName() const;
    virtual bool Join( int msTimeout );
    virtual bool Kill( int msTimeout );
    //@}

private:
    //! @name Private operations
    //@{
    void FetchName();
    //@}

private:
    //! @name Member data
    //@{
    const Api_ABC& api_;
    int pid_;
    std::auto_ptr< Handle > handle_;
    std::string name_;
    //@}
};

}

#endif
