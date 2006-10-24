// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-26 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "Tools/Location.h"

namespace TEST {

// -----------------------------------------------------------------------------
// Name: GenObject::GetLocation
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
inline
const Location& GenObject::GetLocation() const
{
    assert( pLocation_ );
    return *pLocation_;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetType
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType GenObject::GetType() const
{
    return eType_;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetObstacleSubType
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenSousTypeObstacle GenObject::GetObstacleSubType() const
{
    return eObstacleSubType_;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetUrgency
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenUrgence GenObject::GetUrgency() const
{
    return eUrgency_;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetPriority
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenPriorite GenObject::GetPriority() const
{
    return ePriority_;
}

} // end namespace TEST