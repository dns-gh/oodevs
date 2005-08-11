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

#include "Mission_Pawn_ABC.h"

namespace TEST {

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: SBO 2005-08-09
//-----------------------------------------------------------------------------
template < class Mission >
struct sMissionPawnCreator
{
    static Mission_Pawn_ABC& Create( Pawn& target, uint nExecutionTick )
    {
        return *new Mission( target, nExecutionTick );
    }
};

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Register
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
template< typename Mission >
inline
void Mission_Pawn_Type::Register( const std::string& strName )
{
    Mission_Pawn_Type* pType = new Mission_Pawn_Type( strName, &sMissionPawnCreator< Mission >::Create );
    bool bOut = missions_.insert( std::make_pair( strName, pType ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Instanciate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Pawn_ABC& Mission_Pawn_Type::Instanciate( Pawn& target, uint nExecutionTick /* = 0 */ ) const
{
    return missionAllocator_( target, nExecutionTick );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::CreateMission
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Pawn_ABC& Mission_Pawn_Type::CreateMission( const std::string& strName, Pawn& target, uint nExecutionTick /*= 0*/ )
{
    Mission_Pawn_Type* pType = missions_.find( strName )->second;
    assert( pType );
    return pType->Instanciate( target, nExecutionTick );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::CreateMission
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
Mission_Pawn_ABC& Mission_Pawn_Type::CreateMission( const Mission_Pawn_Type& type, Pawn& target, uint nExecutionTick /*= 0*/ )
{
    return type.Instanciate( target, nExecutionTick );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const Mission_Pawn_Type* Mission_Pawn_Type::Find( const std::string& strName )
{
    CIT_MissionPawnTypeMap it = missions_.find( strName );
    if( it != missions_.end() )
        return it->second;
    return 0;
}

} // end namespace TEST
