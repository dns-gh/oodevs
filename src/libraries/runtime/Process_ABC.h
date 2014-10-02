// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef PROCESS_ABC_H
#define PROCESS_ABC_H

#include <boost/noncopyable.hpp>
#include <string>

namespace runtime
{
// =============================================================================
/** @class  Process_ABC
    @brief  Process_ABC interface definition
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Process_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
    //@{
             Process_ABC() {}
    virtual ~Process_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual int GetPid() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual bool IsAlive() const = 0;
    virtual bool Join( int msTimeout ) = 0;
    virtual bool Kill() = 0;
    //@}
};
}

#endif
