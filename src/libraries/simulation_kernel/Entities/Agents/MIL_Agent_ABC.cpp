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

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( const std::string& name, xml::xistream& xis, uint nID )
    : MIL_Entity_ABC ( name, xis ) 
    , nID_           ( nID )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( const std::string& name, uint nID )
    : MIL_Entity_ABC ( name ) 
    , nID_           ( nID )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC()
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
// Name: MIL_Agent_ABC::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Agent_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Agent_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << nID_;
}
