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
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "MIL_Agent_ABC.h"
#include "Tools/MIL_IDManager.h"

namespace
{
MIL_IDManager idManager;
}

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Agent_ABC )

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( xml::xistream& xis )
    : MIL_Entity_ABC( xis, idManager.GetId( xis.attribute< unsigned long >( "id" ), true ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( const std::string& name )
    : MIL_Entity_ABC ( name, idManager.GetId() )
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
// Name: MIL_Agent_ABC::operator==
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool MIL_Agent_ABC::operator==( const MIL_Agent_ABC& rhs ) const
{
    return GetID() == rhs.GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::operator!=
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool MIL_Agent_ABC::operator!=( const MIL_Agent_ABC& rhs ) const
{
    return GetID() != rhs.GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::serialize
// Created: SBO 2009-07-01
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_Agent_ABC::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_Entity_ABC >( *this );
}

template void MIL_Agent_ABC::serialize( MIL_CheckPointInArchive&, const unsigned int );
template void MIL_Agent_ABC::serialize( MIL_CheckPointOutArchive&, const unsigned int );
