// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "Mission_Population_ABC.h"

namespace TEST {

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: SBO 2005-08-09
//-----------------------------------------------------------------------------
template < class Mission >
struct sMissionPopulationCreator
{
    static Mission_Population_ABC& Create( Population& target )
    {
        return *new Mission( target );
    }
};

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::Register
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
template< typename Mission >
inline
void Mission_Population_Type::Register( const std::string& strName )
{
    Mission_Population_Type* pType = new Mission_Population_Type( strName, &sMissionPopulationCreator< Mission >::Create );
    bool bOut = missions_.insert( std::make_pair( strName, pType ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::Instanciate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Population_ABC& Mission_Population_Type::Instanciate( Population& target ) const
{
    return missionAllocator_( target );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::CreateMission
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Population_ABC& Mission_Population_Type::CreateMission( const std::string& strName, Population& target )
{
    Mission_Population_Type* pType = missions_.find( strName )->second;
    assert( pType );
    return pType->Instanciate( target );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::CreateMission
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
Mission_Population_ABC& Mission_Population_Type::CreateMission( const Mission_Population_Type& type, Population& target )
{
    return type.Instanciate( target );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const Mission_Population_Type* Mission_Population_Type::Find( const std::string& strName )
{
    CIT_MissionPopulationTypeMap it = missions_.find( strName );
    if( it != missions_.end() )
        return it->second;
    return 0;
}

} // end namespace TEST
