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

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: Object::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
Object* Object::Find( T_EntityId nId )
{
	CIT_ObjectMap it;
	it = objects_.find( nId );
	if ( it != objects_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: Object::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Object::Register( Object& object )
{
    bool bOut = objects_.insert( std::make_pair( object.GetId(), &object ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: Object::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Object::Unregister( Object& object )
{
    int nOut = objects_.erase( object.GetId() );
    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: Object::GetTeam
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const Team& Object::GetTeam() const
{
    assert( pTeam_ );
	return *pTeam_;
}

//-----------------------------------------------------------------------------
// Name: Object::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Object::GetId() const
{
	return nId_;
}


} // end namespace TEST
