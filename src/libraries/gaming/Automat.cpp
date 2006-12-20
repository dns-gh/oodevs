// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Automat.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const ASN1T_MsgAutomateCreation& message, Controller& controller, 
                  const Resolver_ABC< AutomatType >& resolver )
    : EntityImplementation< Automat_ABC >( controller, message.oid_automate, message.nom )
    , type_( resolver.Get( message.type_automate ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName() ).arg( message.oid_automate );
    RegisterSelf( *this );
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    CreateDictionary( dictionary );
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
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Automat::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Automat", "Info/Identifier" ), id_ );
    dico.Register( *this, tools::translate( "Automat", "Info/Name" ), name_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
        type_.Draw( where, viewport, tools );
}
