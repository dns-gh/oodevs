// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationTemplateElement.h"
#include "FormationModel.h"

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations,
                                                    const kernel::Formation_ABC& formation )
    : TemplateElement_ABC( formation )
    , formations_( formations )
    , levelId_   ( static_cast< unsigned int >( formation.GetLevel() ) )
{
    const kernel::SymbolHierarchy_ABC& symbol = formation.Get< kernel::SymbolHierarchy_ABC >();
    if( symbol.IsOverriden() )
        symbol_ = symbol.GetValue();
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations,
                                                    xml::xistream& xis )
    : TemplateElement_ABC( xis )
    , formations_( formations )
    , levelId_( xis.attribute< unsigned int >( "level" ) )
    , symbol_( xis.attribute( "symbol", "" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::~FormationTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* FormationTemplateElement::Instanciate( kernel::Entity_ABC& superior,
                                                           const geometry::Point2f&,
                                                           ColorController& colorController )
{
    kernel::Formation_ABC* formation = formations_.Create( superior, static_cast< E_NatureLevel >( levelId_ ), name_ );
    if( formation )
    {
        SetColor( *formation, colorController );
        SetExtensions( *formation );
        if( !symbol_.empty() )
        {
            kernel::SymbolHierarchy_ABC& symbol = formation->Get< kernel::SymbolHierarchy_ABC >();
            symbol.OverrideValue( symbol_ );
        }
    }
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void FormationTemplateElement::Serialize( xml::xostream& xos ) const
{
    TemplateElement_ABC::Serialize( xos );
    xos << xml::attribute( "type", "formation" )
        << xml::attribute( "level", levelId_ );
    if( !symbol_.empty() )
        xos << xml::attribute( "symbol", symbol_ );
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool FormationTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &superior );
    return formation && static_cast< unsigned int >( formation->GetLevel() ) > levelId_ ||
           superior.GetTypeName() == kernel::Team_ABC::typeName_;
}
