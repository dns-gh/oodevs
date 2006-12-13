// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Mission.h"
#include "FragOrder.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
Mission::Mission( xml::xistream& xis )
{
    std::string name, mrt;
    xis >> attribute( "name", name )
        >> attribute( "id", (int&)id_ )
        >> optional() >> attribute( "mrt-dia-behavior", mrt );
    name_ = name.c_str();
    automat_ = !mrt.empty();
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
Mission::Mission( const Mission& mission )
    : name_   ( mission.name_ )
    , id_     ( mission.id_ )
    , automat_( mission.automat_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    DeleteAll();
}


// -----------------------------------------------------------------------------
// Name: Mission::GetId
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long Mission::GetId() const
{
    return id_;
}
 
// -----------------------------------------------------------------------------
// Name: Mission::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QString Mission::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Mission::IsAutomat
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool Mission::IsAutomat() const
{
    return automat_;
}
