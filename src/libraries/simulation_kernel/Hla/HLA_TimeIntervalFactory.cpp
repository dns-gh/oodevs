// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-13 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeIntervalFactory.cpp $
// $Author: Age $
// $Modtime: 7/12/04 14:41 $
// $Revision: 2 $
// $Workfile: HLA_TimeIntervalFactory.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_TimeIntervalFactory.h"
#include "HLA_TimeInterval.h"
#include <hla/Deserializer.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_TimeIntervalFactory constructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeIntervalFactory::HLA_TimeIntervalFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeIntervalFactory destructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeIntervalFactory::~HLA_TimeIntervalFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeIntervalFactory::CreateEpsilon
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
TimeInterval_ABC& HLA_TimeIntervalFactory::CreateEpsilon()
{
    HLA_TimeInterval& interval = *new HLA_TimeInterval();
    return interval.SetEpsilon();
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeIntervalFactory::Decode
// Created: AGE 2004-12-07
// -----------------------------------------------------------------------------
TimeInterval_ABC* HLA_TimeIntervalFactory::Decode( Deserializer& deserializer )
{
    double rTime = 0;
    deserializer >> rTime;
    return new HLA_TimeInterval( rTime );
}
