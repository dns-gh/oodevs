// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateSession_h_
#define __CreateSession_h_

#include "ConfigurationManipulator.h"
#include <boost/noncopyable.hpp>

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
class CreateSession : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CreateSession( const tools::Path& filePath );
             CreateSession( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session );
    virtual ~CreateSession();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SetOption( const std::string& path, const T& value );
    bool HasOption( const std::string& path );
    bool RemoveOption( const std::string& path );
    template< typename T >
    T GetOption( const std::string& path ) const;
    void SetDefaultValues();
    const tools::Path& GetPath() const;

    void Commit();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ConfigurationManipulator > setter_;
    tools::Path session_;
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

// -----------------------------------------------------------------------------
// Name: CreateSession::GetOption
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
template< typename T >
T CreateSession::GetOption( const std::string& path ) const
{
    return setter_->GetValue<T>( path ) ;
}

}

#endif // __CreateSession_h_
