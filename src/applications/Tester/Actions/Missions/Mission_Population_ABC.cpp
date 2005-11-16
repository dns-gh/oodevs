// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Population_ABC.h"
#include "Entities/Population.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"
#include "Actions/Scheduler.h"

using namespace TEST;

IDManager Mission_Population_ABC::idManager_( 145 );

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Population_ABC::Mission_Population_ABC( Population& population )
    : Action< Population > ( population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Population_ABC::~Mission_Population_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC::Serialize
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_ABC::Serialize()
{
    //$$$ TODO
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_ABC::Send( const Scheduler& scheduler )
{
    //$$$ TODO
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_ABC::Clean()
{
    // TODO
}
