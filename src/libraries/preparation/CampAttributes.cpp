// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CampAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: CampAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::CampAttributes( Controllers& controllers )
    : tc2_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CampAttributes::~CampAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::SetTC2
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
void CampAttributes::SetTC2( const Agent_ABC& tc2 )
{
    tc2_ = &tc2;
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CampAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
                .Display( tools::translate( "Object", "TC2:" ), tc2_ );
}

// -----------------------------------------------------------------------------
// Name: CampAttributes::Serialize
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CampAttributes::Serialize( xml::xostream& xos ) const
{
    xos << start( "AttributsSpecifiques" )
            << content( "TC2", long( tc2_->GetId() ) )
        << end();
}
