// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "CompatibilityHelper.h"

namespace protocol
{

// Human location enumeration was extended but existing required field
// have to remap new values to old ones to preserve compatibility.
sword::EnumHumanLocation RemapHumanLocation( sword::EnumHumanLocation location )
{
    if( location == sword::funeral )
        return sword::medical;
    return location;
}

}  // namespace protocol

