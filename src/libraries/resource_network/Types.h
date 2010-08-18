// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Resource_Types_h_
#define __Resource_Types_h_

#include <map>
#include <string>
#include <vector>

namespace resource
{
    enum EResourceType
    {
        eResourceTypeWater,
        eResourceTypeGaz,
        eResourceTypeElectricity,
        eNbrResourceType
    };

    struct Consumption
    {
        int amount_;
        bool critical_;
    };

    typedef std::map< EResourceType, Consumption >   T_Consumptions;
    typedef T_Consumptions::iterator                IT_Consumptions;
    typedef T_Consumptions::const_iterator         CIT_Consumptions;
}

#endif // __Resource_Types_h_
