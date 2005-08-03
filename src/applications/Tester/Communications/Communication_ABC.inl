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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_ABC.inl $
// $Author: Sbo $
// $Modtime: 20/07/05 18:17 $
// $Revision: 7 $
// $Workfile: Communication_ABC.inl $
//
// *****************************************************************************

#include "Communications/CommunicationLink.h"
#include "Communications/CommunicationType.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Communication_ABC::GetArity
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
uint Communication_ABC::GetArity() const
{
    return nArity_;
}

//-----------------------------------------------------------------------------
// Name: Communication_ABC::GetType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
const CommunicationType& Communication_ABC::GetType() const
{
    assert( pType_ );
    return *pType_;
}

/*
//-----------------------------------------------------------------------------
// Name: Communication_ABC::GetSource
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
inline
const std::string& Communication_ABC::GetSource() const
{
    return CommunicationManager::GetSource( eSource_ );
}
*/

//-----------------------------------------------------------------------------
// Name: Communication_ABC::IsReport
// Created: SBO 2005-07-01
//-----------------------------------------------------------------------------
inline
bool Communication_ABC::IsReport() const
{
    assert( pType_ );
    return pType_->IsReport();
}

//-----------------------------------------------------------------------------
// Name: Communication_ABC::IsSameType
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
bool Communication_ABC::IsSameType( const CommunicationType& type ) const
{
    assert( pType_ );
    return *pType_ == type;
}

} // end namespace TIC