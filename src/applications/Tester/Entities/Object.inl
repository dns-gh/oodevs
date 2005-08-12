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

#include "Team.h"

namespace TEST
{

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
