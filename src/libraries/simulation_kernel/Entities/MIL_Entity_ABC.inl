// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_Entity_ABC::serialize
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
template< typename Archive > 
void MIL_Entity_ABC::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< MT_RoleContainer >( *this )
            & strName_ ; 
}
