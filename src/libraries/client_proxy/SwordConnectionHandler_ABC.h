// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordConnectionHandler_ABC_h_
#define __SwordConnectionHandler_ABC_h_

// =============================================================================
/** @class  SwordConnectionHandler_ABC
    @brief  SwordConnectionHandler_ABC
*/
// Created: SEB 2010-10-12
// =============================================================================
class SwordConnectionHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordConnectionHandler_ABC() {}
    virtual ~SwordConnectionHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnConnectionSucceeded( const std::string& endpoint ) = 0;
    virtual void OnConnectionFailed( const std::string& endpoint, const std::string& reason ) = 0;
    virtual void OnConnectionError( const std::string& endpoint, const std::string& reason ) = 0;
    virtual void OnConnectionWarning( const std::string& endpoint, const std::string& reason ) = 0;
    virtual void OnAuthenticationSucceeded( const std::string& profile ) = 0;
    virtual void OnAuthenticationFailed( const std::string& profile, const std::string& reason ) = 0;
    //@}
};

#endif // __SwordConnectionHandler_ABC_h_
