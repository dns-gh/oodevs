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
#include "Entities/Team.h"
#include "Entities/Pawn.h"

namespace TEST
{

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
// Name: Automat::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Automat::GetId() const
{
	return nId_;
}


} // end namespace TEST
