// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "MIL_Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC constructor
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
MIL_Entity_ABC::MIL_Entity_ABC( const std::string& name, unsigned int id )
    : strName_( name )
    , id_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC constructor
// Created: RDS 2008-05-05
// -----------------------------------------------------------------------------
MIL_Entity_ABC::MIL_Entity_ABC( xml::xistream& xis, unsigned int id )
    : strName_( xis.attribute< std::string >( "name", "" ) )
    , id_( id )
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC destructor
// Created: RDS 2008-04-25
// -----------------------------------------------------------------------------
MIL_Entity_ABC::~MIL_Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::GetName
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
const std::string& MIL_Entity_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::SetName
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void MIL_Entity_ABC::SetName( const std::string& name )
{
    strName_ = name;
}

unsigned int MIL_Entity_ABC::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::WriteODB
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
void MIL_Entity_ABC::WriteODB( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", strName_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::serialize
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_Entity_ABC::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< tools::RoleContainer >( *this )
        & const_cast< std::string& >( strName_ )
        & const_cast< unsigned int& >( id_ );
}

template void MIL_Entity_ABC::serialize( MIL_CheckPointInArchive&, const unsigned int );
template void MIL_Entity_ABC::serialize( MIL_CheckPointOutArchive&, const unsigned int );
