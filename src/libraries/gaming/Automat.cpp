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
#include "clients_kernel/DataDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const ASN1T_MsgAutomateCreation& message, Controller& controller, 
                  const Resolver_ABC< AutomatType >& resolver )
    : controller_   ( controller )
    , type_         ( resolver.Get( message.type_automate ) )
    , id_           ( message.oid_automate )
{
    RegisterSelf( *this );
    name_ = QString( "%1 [%2]" ).arg( message.nom ).arg( id_ );

    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    // $$$$ AGE 2006-10-06: 
    dictionary.Register( tools::translate( "Automat", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Automat", "Info/Nom" ), name_ );
    controller_.Create( *(Automat_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    controller_.Delete( *(Automat_ABC*)this );
    DestroyExtensions();
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
