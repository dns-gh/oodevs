// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Lima.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, E_FuncLimaType nFuncType, IdManager& idManager )
    : TacticalLine_ABC( tools::ToString( nFuncType ), idManager )
    , controller_     ( controller )
    , nFuncType_      ( nFuncType )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Lima::Lima( kernel::Controller& controller, xml::xistream& xis, IdManager& idManager )
    : TacticalLine_ABC( xis, idManager )
    , controller_( controller )
{
    int type = 0;
    xis >> xml::attribute( "type", type );
    nFuncType_ = E_FuncLimaType( type );
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}
 
// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    DestroyExtensions();
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::Select( ActionController& actions ) const
{
    actions.Select( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *this, *(kernel::TacticalLine_ABC*)this, point );
}

// -----------------------------------------------------------------------------
// Name: Lima::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Lima::Activate( ActionController& actions ) const
{
    actions.Activate( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::SerializeAttributes
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void Lima::SerializeAttributes( xml::xostream& xos ) const
{
    TacticalLine_ABC::SerializeAttributes( xos );
    xos << attribute( "type", int( nFuncType_ ) );
}

// -----------------------------------------------------------------------------
// Name: Lima::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Lima::IsLimit() const
{
    return false;
}
