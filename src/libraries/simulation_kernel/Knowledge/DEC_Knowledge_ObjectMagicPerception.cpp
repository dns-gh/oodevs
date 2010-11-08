// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectMagicPerception.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMagicPerception::DEC_Knowledge_ObjectMagicPerception
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectMagicPerception::DEC_Knowledge_ObjectMagicPerception( const DEC_Knowledge_Object& knowledge, const PHY_PerceptionLevel& perceptionLevel )
    : knowledge_      ( knowledge )
    , perceptionLevel_( perceptionLevel )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMagicPerception::~DEC_Knowledge_ObjectMagicPerception
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectMagicPerception::~DEC_Knowledge_ObjectMagicPerception()
{
}