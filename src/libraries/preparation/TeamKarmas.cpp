// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamKarmas.h"
#include "clients_kernel/Karma.h"

// -----------------------------------------------------------------------------
// Name: TeamKarmas constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarmas::TeamKarmas()
    : default_( kernel::Karma::friend_ )
{
    Register( "friend" , kernel::Karma::friend_ );
    Register( "neutral", kernel::Karma::neutral_ );
    Register( "enemy"  , kernel::Karma::enemy_ );
}

// -----------------------------------------------------------------------------
// Name: TeamKarmas destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarmas::~TeamKarmas()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamKarmas::GetDefault
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
const kernel::Karma& TeamKarmas::GetDefault() const
{
    return default_;
}
