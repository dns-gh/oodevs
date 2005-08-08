// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: PawnKnowledge::GetId
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
inline
T_EntityId PawnKnowledge::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PawnKnowledge::GetRealPawn
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
inline
const Pawn* PawnKnowledge::GetRealPawn() const
{
    return pRealPawn_;
}

} // end namespace TEST