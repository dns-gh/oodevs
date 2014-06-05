// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Profile.h"
#include "ProfileVisitor_ABC.h"
#include "protocol/Authentication.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace frontend;

Profile Profile::Invalid;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2010-02-19
// -----------------------------------------------------------------------------
Profile::Profile()
    : valid_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::Profile( xml::xistream& xis )
    : login_( xis.attribute< std::string >( "name" ) )
    , password_( xis.attribute< std::string >( "password" ) )
    , supervisor_( false )
    , valid_( true )
{
    xis >> xml::optional >> xml::attribute( "supervision", supervisor_ );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
QString Profile::GetLogin() const
{
    return login_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Profile::GetPassword
// Created: AHC 2011-05-18
// -----------------------------------------------------------------------------
QString Profile::GetPassword() const
{
    return password_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Profile::IsSupervision
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
bool Profile::IsSupervision() const
{
    return supervisor_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsValid
// Created: SBO 2010-02-19
// -----------------------------------------------------------------------------
bool Profile::IsValid() const
{
    return valid_;
}

namespace
{
    struct ProfileReader
    {
        void ReadProfile( xml::xistream& xis, ProfileVisitor_ABC& visitor )
        {
            Profile profile( xis );
            visitor.Visit( profile );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Profile::VisitProfiles
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void Profile::VisitProfiles( const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, const tools::Path& exercise, ProfileVisitor_ABC& visitor )
{
    const tools::Path exerciseFile = config.GetExerciseFile( exercise );
    tools::Path profilesFile;
    {
        std::unique_ptr< xml::xistream > xis = fileLoader.LoadFile( exerciseFile ) ;
        *xis >> xml::start( "exercise" )
                >> xml::start( "profiles" )
                    >> xml::attribute( "file", profilesFile );
    }
    {
        std::unique_ptr< xml::xistream > xis = fileLoader.LoadFile( config.BuildChildPath( exerciseFile, profilesFile ) );
        ProfileReader reader;
        *xis >> xml::start( "profiles" )
                >> xml::list( "profile", reader, &ProfileReader::ReadProfile, visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void Profile::Send( sword::Profile& message ) const
{
    message.set_login( login_ );
    message.set_password( password_ );
    message.set_supervisor( supervisor_ );
}
