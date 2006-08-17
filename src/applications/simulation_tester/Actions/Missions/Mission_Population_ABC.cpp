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

#include "simulation_tester_pch.h"
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
Mission_Population_ABC::Mission_Population_ABC( const std::string& name, Population& population )
    : Action< Population > ( name, population )
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
    ASN_Tools::CopyID( pTarget_->GetId()                  , asnMsg_.GetAsnMsg().oid_unite_executante );
    ASN_Tools::CopyID( idManager_.GetFreeIdentifier()     , asnMsg_.GetAsnMsg().order_id             );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_ABC::Send( const Scheduler& /*scheduler*/ )
{
    asnMsg_.Send( 56 );
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_ABC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_ABC::Clean()
{
    // NOTHING
}
