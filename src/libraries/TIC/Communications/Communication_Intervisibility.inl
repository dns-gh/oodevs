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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_Intervisibility.inl $
// $Author: Sbo $
// $Modtime: 20/07/05 18:03 $
// $Revision: 7 $
// $Workfile: Communication_Intervisibility.inl $
//
// *****************************************************************************

#include "Communications/MessageReport.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::ResetStatus
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
bool Communication_Intervisibility::ResetStatus()
{
    for( IT_EntityIdMap it = subjects_.begin(); it != subjects_.end(); ++it )
        it->second = false;
    // return false => do not delete communication
    return false;
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::IsSameSubject
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
bool Communication_Intervisibility::IsSameSubject( T_EntityId nSubjectId ) const
{
    return subjects_.find( nSubjectId ) != subjects_.end() ? true : false;
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::GetArity
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
inline
uint Communication_Intervisibility::GetArity() const
{
    uint nCount = 0;
    for( CIT_EntityIdMap it = subjects_.begin(); it != subjects_.end(); ++it )
        if( it->second == true )
            ++nCount;
    return nCount;
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::GetSubject
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
inline
const std::string Communication_Intervisibility::GetSubject() const
{
    std::stringstream ss;
    uint              nCount = 0;
    for( CIT_EntityIdMap it = subjects_.begin(); it != subjects_.end(); ++it )
        if( it->second == true )
        {
            if( nCount++ > 0 )
                ss << ',';
            ss << it->first;
        }
    return ss.str();
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::GetMessage
// Created: SBO 2005-07-01
//-----------------------------------------------------------------------------
inline
const std::string& Communication_Intervisibility::GetMessage() const
{
    return MessageReport::notDefined.GetName();
}

} // end namespace TIC