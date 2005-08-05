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
// Name: Team::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
Team* Team::Find( T_EntityId nId )
{
	CIT_TeamMap it;
	it = teams_.find( nId );
	if ( it != teams_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: Team::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Team::Register( Team& team )
{
    bool bOut = teams_.insert( std::make_pair( team.GetId(), &team ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: Team::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Team::Unregister( Team& team )
{
    int nOut = teams_.erase( team.GetId() );
    assert( nOut == 1 );
}


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
