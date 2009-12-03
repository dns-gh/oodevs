// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IntelligenceKarmas.h"
#include "clients_kernel/Karma.h"

// -----------------------------------------------------------------------------
// Name: IntelligenceKarmas constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
IntelligenceKarmas::IntelligenceKarmas()
{
    Register( "friend" , kernel::Karma::friend_ );
    Register( "enemy"  , kernel::Karma::enemy_ );
    Register( "neutral", kernel::Karma::neutral_ );
    Register( "unknown", kernel::Karma::unknown_ );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceKarmas destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
IntelligenceKarmas::~IntelligenceKarmas()
{
     // NOTHING
}
