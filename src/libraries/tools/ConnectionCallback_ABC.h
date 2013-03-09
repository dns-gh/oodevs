// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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

    // Called upon connection success, with local and remote endpoints address.
    // Note the meaning of local and remote is not the same if are connecting
    // as client or server.
    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote ) = 0;
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error ) = 0;
};

}

#endif // __ConnectionCallback_ABC_h_
