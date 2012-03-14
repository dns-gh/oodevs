// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef HANDLE_H__
#define HANDLE_H__
#include <boost/noncopyable.hpp>
#include <windows.h>

namespace process
{

class Api_ABC;

// =============================================================================
/** @class  HandleWrapper
    @brief  Win32 HANDLE wrapper definition
*/
// Created: BAX 2012-03-08
// =============================================================================
class Handle : public boost::noncopyable
{
    public:
    //! @name Constructor/Destructor
    //@{
     Handle( const Api_ABC& api, HANDLE value );
    ~Handle();

    //! @name Public members
    //@{
    HANDLE value_;
    //@}

    private:
    //! @name Private members
    //@{
    const Api_ABC& api_;
    //@}
};

}

#endif