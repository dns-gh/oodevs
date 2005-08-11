// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "Mission_Automat_ABC.h"

namespace TEST {

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: SBO 2005-08-09
//-----------------------------------------------------------------------------
template < class Mission >
struct sMissionAutomatCreator
{
    static Mission_Automat_ABC& Create( Automat& target, uint nExecutionTick )
    {
        return *new Mission( target, nExecutionTick );
    }
};

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Register
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
template< typename Mission >
inline
void Mission_Automat_Type::Register( const std::string& strName )
{
    Mission_Automat_Type* pType = new Mission_Automat_Type( strName, &sMissionAutomatCreator< Mission >::Create );
    bool bOut = missions_.insert( std::make_pair( strName, pType ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Instanciate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Automat_ABC& Mission_Automat_Type::Instanciate( Automat& target, uint nExecutionTick /* = 0 */ ) const
{
    return missionAllocator_( target, nExecutionTick );
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::CreateMission
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
Mission_Automat_ABC& Mission_Automat_Type::CreateMission( const std::string& strName, Automat& target, uint nExecutionTick /*= 0*/ )
{
    Mission_Automat_Type* pType = missions_.find( strName )->second;
    assert( pType );
    return pType->Instanciate( target, nExecutionTick );
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Find
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
const Mission_Automat_Type* Mission_Automat_Type::Find( const std::string& strName )
{
    CIT_MissionAutomatTypeMap it = missions_.find( strName );
    if( it != missions_.end() )
        return it->second;
    return 0;
}

} // end namespace TEST
