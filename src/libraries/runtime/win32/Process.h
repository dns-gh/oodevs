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
#include <runtime/Process_ABC.h>
#include <boost/shared_ptr.hpp>

namespace runtime
{
    struct Api_ABC;

    typedef boost::shared_ptr< void > Handle;
    Handle MakeHandle( const Api_ABC& api, void* value );

// =============================================================================
/** @class  Process
    @brief  Win32 runtime class definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Process : public runtime::Process_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    Process( const Api_ABC& api, int identifier );
    Process( const Api_ABC& api, int identifier, Handle handle );
    virtual ~Process();
    //@}

    //! @name Operations
    //@{
    virtual int GetPid() const;
    virtual const std::string& GetName() const;
    virtual bool IsAlive() const;
    virtual bool Join( int msTimeout );
    virtual bool Kill();
    //@}

private:
    //! @name Member data
    //@{
    const Api_ABC& api_;
    int pid_;
    Handle handle_;
    const std::string name_;
    //@}
};
}

#endif
