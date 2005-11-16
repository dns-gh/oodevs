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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Population.inl $
// $Author: Sbo $
// $Modtime: 21/07/05 17:05 $
// $Revision: 24 $
// $Workfile: Population.inl $
//
// *****************************************************************************

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: Population::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Population::GetId() const
{
	return nId_;
}

//-----------------------------------------------------------------------------
// Name: Population::GetName
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const std::string& Population::GetName() const
{
	return strName_;
}

} // end namespace TEST
