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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Team.inl $
// $Author: Sbo $
// $Modtime: 1/06/05 11:18 $
// $Revision: 3 $
// $Workfile: Team.inl $
//
// *****************************************************************************

namespace TEST {

//-----------------------------------------------------------------------------
// Name: Team::GetName
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const std::string& Team::GetName() const
{
	return strName_;
}

//-----------------------------------------------------------------------------
// Name: Team::GetRelationWith
// Created: SBO 2005-05-13
//-----------------------------------------------------------------------------
inline
Team::E_Diplomacy Team::GetRelationWith( const Team& otherTeam ) const
{
    if( &otherTeam == this )
        return eFriend;

    CIT_TeamRelationMap itRelation = relations_.find( &otherTeam );
    if( itRelation == relations_.end() )
        return eUnknown;
    else
        return itRelation->second;
}

//-----------------------------------------------------------------------------
// Name: Team::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Team::GetId() const
{
	return nId_;
}

} // end namespace TEST
