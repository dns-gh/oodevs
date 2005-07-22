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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_CR.inl $
// $Author: Sbo $
// $Modtime: 20/07/05 18:03 $
// $Revision: 7 $
// $Workfile: Communication_CR.inl $
//
// *****************************************************************************

#include "Communications/MessageReport.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Communication_CR::ResetStatus
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
bool Communication_CR::ResetStatus()
{
    // return true => communication will be deleted
    return true;
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::IsSameSubject
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
bool Communication_CR::IsSameSubject( T_EntityId nSubjectId ) const
{
    return nSubjectId_ == nSubjectId;
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::GetSubject
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
inline
const std::string Communication_CR::GetSubject() const
{
    std::stringstream ss;
    ss << nSubjectId_;
    return ss.str();
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::GetMessage
// Created: SBO 2005-07-01
//-----------------------------------------------------------------------------
inline
const std::string& Communication_CR::GetMessage() const
{
    return MessageReport::Find( nSubjectId_ ).GetName();
}

} // end namespace TIC