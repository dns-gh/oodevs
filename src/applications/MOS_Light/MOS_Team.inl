// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Team.inl $
// $Author: Nld $
// $Modtime: 14/02/05 15:43 $
// $Revision: 2 $
// $Workfile: MOS_Team.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Team::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint MOS_Team::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetName
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const std::string& MOS_Team::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetColor
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const GFX_Color& MOS_Team::GetColor() const
{
    return color_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::GetObjectKnowledges
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const MOS_Team::T_ObjectKnowledgeMap& MOS_Team::GetObjectKnowledges() const
{
    return objectKnowledges_;    
}
