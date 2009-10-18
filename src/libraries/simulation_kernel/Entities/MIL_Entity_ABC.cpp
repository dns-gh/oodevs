// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Entity_ABC.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC constructor
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
MIL_Entity_ABC::MIL_Entity_ABC( const std::string& name )
    : strName_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC constructor
// Created: RDS 2008-05-05
// -----------------------------------------------------------------------------
MIL_Entity_ABC::MIL_Entity_ABC( xml::xistream& xis )
    : strName_( xml::attribute< std::string >( xis, "name", "" ) )
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC destructor
// Created: RDS 2008-04-25
// -----------------------------------------------------------------------------
MIL_Entity_ABC::~MIL_Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::GetName
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
const std::string& MIL_Entity_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::WriteODB
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
void MIL_Entity_ABC::WriteODB( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", strName_ );
}
