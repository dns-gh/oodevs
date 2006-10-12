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
    : EntityImplementation< Automat_ABC >( controller, idManager.GetNextId(), "" )
    , type_         ( &type )
{
    name_ = QString( "Automate %1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : EntityImplementation< Automat_ABC >( controller, ReadId( xis ), ReadName( xis ) )
{
    std::string type;
    xis >> attribute( "type", type );
    type_ = &agentTypes.Resolver< AutomatType, QString >::Get( type.c_str() );
    RegisterSelf( *this );
    CreateDictionary();
    idManager.Lock( id_ );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Automat::ReadId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
unsigned long Automat::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Automat::ReadName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
QString Automat::ReadName( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    return name.c_str();
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
// Name: Automat::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Automat::Rename( const QString& name )
{
    name_ = name;
    Touch();
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
