// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatTemplateElement.h"
#include "AgentsModel.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_gui/EntityType.h"

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents,
                                                const kernel::Automat_ABC& automat )
    : TemplateElement_ABC( automat )
    , agents_( agents )
    , type_  ( automat.Get< gui::EntityType< kernel::AutomatType > >().GetType() )
{
    const kernel::SymbolHierarchy_ABC& symbol = automat.Get< kernel::SymbolHierarchy_ABC >();
    if( symbol.IsOverriden() )
        symbol_ = symbol.GetValue();
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents,
                                                const kernel::AutomatType& type,
                                                xml::xistream& xis )
    : TemplateElement_ABC( xis )
    , agents_( agents )
    , type_  ( type )
    , symbol_( xis.attribute( "symbol", "" ) )
{
    if( name_.isEmpty() )
        name_ = type_.GetName().c_str();
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::~AutomatTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AutomatTemplateElement::Instanciate( kernel::Entity_ABC& superior,
                                                         const geometry::Point2f&,
                                                         ColorController& colorController )
{
    if( superior.GetTypeName() == kernel::Formation_ABC::typeName_ )
    {
        kernel::Automat_ABC& automat = agents_.CreateAutomat( superior, type_, name_ );
        SetColor( automat, colorController );
        SetExtensions( automat );
        if( !symbol_.empty() )
        {
            kernel::SymbolHierarchy_ABC& symbol = automat.Get< kernel::SymbolHierarchy_ABC >();
            symbol.OverrideValue( symbol_ );
        }
        return &automat;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Serialize( xml::xostream& xos )
{
    TemplateElement_ABC::Serialize( xos );
    xos << xml::attribute( "type", "automat" )
        << xml::attribute( "automatType", type_.GetName() );
    if( !symbol_.empty() )
        xos << xml::attribute( "symbol", symbol_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AutomatTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return superior.GetTypeName() == kernel::Formation_ABC::typeName_;
}
