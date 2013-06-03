// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_Agent_ABC.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 11:12 $
// $Revision: 2 $
// $Workfile: MIL_Agent_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Agent_ABC.h"
#include <xeumeuleu/xml.hpp>

MIL_IDManager MIL_Agent_ABC::idManager_;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Agent_ABC )

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( xml::xistream& xis )
    : MIL_Entity_ABC( xis )
    , nID_          ( idManager_.GetId( xis.attribute< unsigned long >( "id" ), true ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( const std::string& name )
    : MIL_Entity_ABC ( name )
    , nID_           ( idManager_.GetId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::~MIL_Agent_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::GetID
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
unsigned int MIL_Agent_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::operator==
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool MIL_Agent_ABC::operator==( const MIL_Agent_ABC& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::operator!=
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool MIL_Agent_ABC::operator!=( const MIL_Agent_ABC& rhs ) const
{
    return nID_ != rhs.nID_;
}