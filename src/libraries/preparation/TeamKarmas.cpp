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
#include "TeamKarma.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: TeamKarmas constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarmas::TeamKarmas()
{
    default_ = new TeamKarma( tools::translate( "TeamKarmas", "Friend" ), "friend" );
    Register( "friend" , *default_ );
    Register( "neutral", *new TeamKarma( tools::translate( "TeamKarmas", "Neutral" ), "neutral" ) );
    Register( "enemy"  , *new TeamKarma( tools::translate( "TeamKarmas", "Enemy" ), "enemy" ) );

}

// -----------------------------------------------------------------------------
// Name: TeamKarmas destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarmas::~TeamKarmas()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamKarmas::GetDefault
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
TeamKarma& TeamKarmas::GetDefault() const
{
    return *default_;
}
