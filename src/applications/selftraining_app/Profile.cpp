// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Profile.h"

#include <xeumeuleu/xml.h> 

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::Profile( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides )
    : name_( xml::attribute< std::string >( xis, "name" ) )
    , supervisor_ ( false ) 
{
    xis >> xml::optional() >> xml::attribute( "supervision", supervisor_ ); 
    ReadSides( xis, sides  ) ; 
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::~Profile( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadSides
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void Profile::ReadSides( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides )
{
    static int ro(READ), rw(READ|WRITE) ;  
    xis >> xml::optional() >> xml::start( "rights")
            >> xml::start( "readonly")
                >> xml::list("side", *this, &Profile::ReadSide, sides, ro ) 
            >> xml::end()
            >> xml::optional() >> xml::start( "readwrite")
                >> xml::list("side", *this, &Profile::ReadSide, sides, rw ) 
            >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadSide
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void Profile::ReadSide( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides, int rights )
{
    unsigned int id ; 
    xis >> xml::attribute("id", id ) ; 
    Side* side = sides.Find( id ) ; 
    sides_[side] |=  rights   ; 
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
QString Profile::GetLogin() const
{
    return name_.c_str(); 
}

// -----------------------------------------------------------------------------
// Name: Profile::IsSupervision
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
bool Profile::IsSupervision() const
{
    return supervisor_ ; 
}

// -----------------------------------------------------------------------------
// Name: Profile::GetRights
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
const int Profile::GetRights( Side& side ) const
{
    CIT_SideRightsList it = sides_.find( &side ) ; 
    if ( it != sides_.end() ) 
        return (*it).second ; 
    else
        return 0 ; 
}

