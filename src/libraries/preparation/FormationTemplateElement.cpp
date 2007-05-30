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
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "FormationModel.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations, const kernel::Formation_ABC& formation )
    : formations_( formations )
    , levelId_   ( formation.GetLevel().GetId() )
    , name_      ( "Formation - " + formation.GetLevel().GetName() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations, xml::xistream& input )
    : formations_( formations )
{
    std::string name;
    input >> xml::attribute( "level", levelId_ )
          >> xml::attribute( "name", name );
    name_ = name.c_str();
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
kernel::Entity_ABC* FormationTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& )
{
    return formations_.Create( superior, levelId_ );
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void FormationTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "formation" )
           << xml::attribute( "level", levelId_ )
           << xml::attribute( "name", name_.ascii() );
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool FormationTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &superior );
    return formation && formation->GetLevel().GetId() > levelId_;
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString FormationTemplateElement::GetName() const
{
    return name_;
}
