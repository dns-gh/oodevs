// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CreateSession_h_
#define __CreateSession_h_

#include "ConfigurationManipulator.h"

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  CreateSession
    @brief  CreateSession
*/
// Created: SBO 2008-02-25
// =============================================================================
class CreateSession
{

public:
    //! @name Constructors/Destructor
    //@{
             CreateSession( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
    virtual ~CreateSession();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SetOption( const std::string& path, const T& value );
    void SetDefaultValues();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateSession( const CreateSession& );            //!< Copy constructor
    CreateSession& operator=( const CreateSession& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::string GetSessionXml( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ConfigurationManipulator > setter_;
    std::string session_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: CreateSession::SetOption
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
template< typename T >
void CreateSession::SetOption( const std::string& path, const T& value )
{
    setter_->SetValue( path, value );
}

}

#endif // __CreateSession_h_
