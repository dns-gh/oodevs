// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConnectionCallback_ABC_h_
#define __ConnectionCallback_ABC_h_

#include <string>

namespace tools
{
// =============================================================================
/** @class  ConnectionCallback_ABC
    @brief  ConnectionCallback_ABC
*/
// Created: AGE 2007-09-06
// =============================================================================
class ConnectionCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConnectionCallback_ABC() {}
    virtual ~ConnectionCallback_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint ) = 0;
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error ) = 0;
    //@}
};

}

#endif // __ConnectionCallback_ABC_h_
