// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MissionType.h"
#include "OrderParameter.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
MissionType::MissionType( xml::xistream& xis, bool context )
    : OrderType( xis )
{
    std::string mrt;
    xis >> optional() >> attribute( "mrt-dia-behavior", mrt )
        >> list( "parameter", *this, &MissionType::ReadParameter );
    automat_ = !mrt.empty();
    if( context )
        AddContextParameters();
}

// -----------------------------------------------------------------------------
// Name: MissionType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
MissionType::~MissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionType::IsAutomat
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool MissionType::IsAutomat() const
{
    return automat_;
}

// -----------------------------------------------------------------------------
// Name: MissionType::ReadParameter
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void MissionType::ReadParameter( xml::xistream& xis )
{
    OrderParameter* param = new OrderParameter( xis );
    Register( Count(), *param );
}

// -----------------------------------------------------------------------------
// Name: MissionType::AddContextParameters
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void MissionType::AddContextParameters()
{
    Register( Count(), *new OrderParameter( tools::translate( "MissionType", "Limits" ), "limits", !automat_, true ) );
    Register( Count(), *new OrderParameter( tools::translate( "MissionType", "Limas" ) , "limalist", true, true ) );
    Register( Count(), *new OrderParameter( tools::translate( "MissionType", "Dangerous Direction" ), "dangerousdirection", !automat_, true ) );
}
