// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Automat.inl $
// $Author: Sbo $
// $Modtime: 21/07/05 17:04 $
// $Revision: 7 $
// $Workfile: Automat.inl $
//
// *****************************************************************************

#include "Entities/KnowledgeGroup.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Automat::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
Automat* Automat::Find( T_EntityId nId )
{
	CIT_AutomatMap it;
	it = automats_.find( nId );
	if ( it != automats_.end() )
		return ( *it ).second;
	return 0;
}


//-----------------------------------------------------------------------------
// Name: Automat::GetAutomatAt
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
const Automat* Automat::GetAutomatAt( uint nIdx )
{
    CIT_AutomatMap it = automats_.begin();
    assert( nIdx < automats_.size() );
    std::advance( it, nIdx );
    return ( *it ).second;
}

//-----------------------------------------------------------------------------
// Name: Automat::GetAutomatCount
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
uint Automat::GetAutomatCount()
{
    return automats_.size();
}

//-----------------------------------------------------------------------------
// Name: Automat::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Automat::Register( Automat& automat )
{
    bool bOut = automats_.insert( std::make_pair( automat.GetId(), &automat ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: Automat::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Automat::Unregister( Automat& automat )
{
    int nOut = automats_.erase( automat.GetId() );
    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: Automat::AddDisaggregation
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
inline
void Automat::AddDisaggregation( T_EntityId nId )
{
    bool bOut = disaggregAutomats_.insert( nId ).second;
    assert( bOut );
}


//-----------------------------------------------------------------------------
// Name: Automat::MustBeDisaggregated
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
inline
bool Automat::MustBeDisaggregated() const
{
    return bConfigDisaggreg_ || pKnowledgeGroup_->MustBeDisaggregated();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetTeam
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const Team& Automat::GetTeam() const
{
    assert( pTeam_ );
	return *pTeam_;
}

//-----------------------------------------------------------------------------
// Name: Automat::GetKnowledgeGroup
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const KnowledgeGroup&  Automat::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
	return *pKnowledgeGroup_;
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPc
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
inline
const Pawn&  Automat::GetPc() const
{
    assert( pPc_ );
	return *pPc_;
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPawnCount
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
uint Automat::GetPawnCount() const
{
    return childPawns_.size();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPawnAt
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
const Pawn* Automat::GetPawnAt( uint nIdx ) const
{
    CIT_PawnSet it = childPawns_.begin();
    assert( nIdx < childPawns_.size() );
    std::advance( it, nIdx );
    return ( *it );
}

//-----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Automat::GetId() const
{
	return nId_;
}


} // end namespace TIC
