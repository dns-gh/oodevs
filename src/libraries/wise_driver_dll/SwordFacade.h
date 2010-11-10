// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordFacade_h_
#define __SwordFacade_h_

#include "client_proxy/SwordConnectionHandler_ABC.h"
#include <memory>
#include <wiseinterfacetypes.h>

class CWISEDriver;
class IWISEDriverSettings;
class Model;
class SwordProxy;

// =============================================================================
/** @class  SwordFacade
    @brief  SwordFacade
*/
// Created: SEB 2010-10-12
// =============================================================================
class SwordFacade : public SwordConnectionHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordFacade( IWISEDriverSettings& pSettings, CWISEDriver& driver );
    virtual ~SwordFacade();
    //@}

    //! @name Operations
    //@{
    void Connect( const WISE_HANDLE& database );
    void Disconnect();
    void OnRun();
    void OnEventReceived( const WISE_HANDLE& handle );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SwordFacade( const SwordFacade& );            //!< Copy constructor
    SwordFacade& operator=( const SwordFacade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OnConnectionSucceeded( const std::string& endpoint );
    virtual void OnConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionError( const std::string& endpoint, const std::string& reason );
    virtual void OnAuthenticationSucceeded( const std::string& profile );
    virtual void OnAuthenticationFailed( const std::string& profile, const std::string& reason );
    //@}

private:
    //! @name Member data
    //@{
    CWISEDriver& driver_;
    std::auto_ptr< SwordProxy > proxy_;
    std::auto_ptr< Model > model_;
    //@}
};

#endif // __SwordFacade_h_
