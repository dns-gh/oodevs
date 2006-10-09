// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Automat.h"
#include "IdManager.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const AutomatType& type, Controller& controller, IdManager& idManager )
    : controller_   ( controller )
    , type_         ( &type )
    , name_         ( "$$$$Automate$$$$" ) // $$$$ AGE 2006-10-06: 
    , id_           ( idManager.GetNextId() )
{
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Automat_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : controller_( controller )
{
    std::string name, type;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name )
        >> attribute( "type", type );
    name_ = name.c_str();
    type_ = &agentTypes.Resolver< AutomatType, QString >::Get( type.c_str() );
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Automat_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetName
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
QString Automat::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
unsigned long Automat::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
        type_->Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::CreateDictionary()
{
    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    // $$$$ AGE 2006-10-06: 
    dictionary.Register( tools::translate( "Automat", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Automat", "Info/Nom" ), name_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::DoSerialize
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() )
        << attribute( "type", type_->GetName().ascii() );
}
