// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ConnectionHandler_ABC_h_
#define __ConnectionHandler_ABC_h_

namespace frontend
{
// =============================================================================
/** @class  ConnectionHandler_ABC
    @brief  ConnectionHandler_ABC
*/
// Created: SBO 2010-10-21
// =============================================================================
class ConnectionHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConnectionHandler_ABC() {}
    virtual ~ConnectionHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnConnectionSucceeded() = 0;
    virtual void OnConnectionFailed( const std::string& reason ) = 0;
    virtual void OnConnectionLost( const std::string& reason ) = 0;
    virtual void OnError( const std::string& message ) = 0;
    //@}
};

}

#endif // __ConnectionHandler_ABC_h_
