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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeFactory.cpp $
// $Author: Age $
// $Modtime: 17/11/04 11:07 $
// $Revision: 2 $
// $Workfile: HLA_TimeFactory.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "hla/Deserializer.h"
#include "HLA_TimeFactory.h"
#include "HLA_Time.h"

// -----------------------------------------------------------------------------
// Name: HLA_TimeFactory constructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeFactory::HLA_TimeFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeFactory destructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeFactory::~HLA_TimeFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeFactory::CreateInitial
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
Time_ABC& HLA_TimeFactory::CreateInitial()
{
    return *new HLA_Time();    
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeFactory::Decode
// Created: AGE 2004-10-14
// -----------------------------------------------------------------------------
Time_ABC* HLA_TimeFactory::Decode( Deserializer& deserializer )
{
    double rTime = 0;
    deserializer >> rTime;
    return new HLA_Time( rTime );
}
