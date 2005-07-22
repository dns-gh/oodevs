// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Automat.cpp $
// $Author: Sbo $
// $Modtime: 24/06/05 10:19 $
// $Revision: 6 $
// $Workfile: Automat.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "Automat.h"
#include "Pawn.h"

#include "TIC/Entities/Automat.h"
#include "TIC/Entities/Pawn.h"
#include "TIC/Entities/Team.h"
#include "TIC/Entities/KnowledgeGroup.h"
#include "TIC/Entities/EntityManager.h"

namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Automat::Automat
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Automat::Automat()
: pAutomat_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: Automat::Automat
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Automat::Automat( unsigned int nIdx )
{
    pAutomat_ = TIC::Automat::GetAutomatAt( nIdx );
    assert( pAutomat_ );
}

//-----------------------------------------------------------------------------
// Name: Automat::~Automat
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Automat::~Automat()
{
}

//-----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Automat::GetId() const
{
    assert( pAutomat_ );
    return pAutomat_->GetId();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetTeam
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const std::string& Automat::GetTeam() const
{
    assert( pAutomat_ );
    return pAutomat_->GetTeam().GetName();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetKnowledgeGroup
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Automat::GetKnowledgeGroup() const
{
    assert( pAutomat_ );
    return pAutomat_->GetKnowledgeGroup().GetId();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPc
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const Pawn Automat::GetPc() const
{
    return Pawn( pAutomat_->GetPc() );
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPawnCount
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Automat::GetPawnCount() const
{
    assert( pAutomat_ );
    return pAutomat_->GetPawnCount();
}

//-----------------------------------------------------------------------------
// Name: Automat::GetPawnAt
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const Pawn Automat::GetPawnAt( unsigned int nIdx ) const
{
    const TIC::Pawn* pPawn = pAutomat_->GetPawnAt( nIdx );
    assert( pPawn );
    return Pawn( *pPawn );
}

} // namespace TICExport