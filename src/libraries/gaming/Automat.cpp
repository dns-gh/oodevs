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
                  const Resolver_ABC< AutomatType >& resolver,
                  const Resolver_ABC< KnowledgeGroup_ABC >& groupResolver )
    : controller_   ( controller )
    , type_         ( resolver.Get( message.type_automate ) )
    , groupResolver_( groupResolver )
    , group_        ( &groupResolver.Get( message.oid_groupe_connaissance ) )
    , name_         ( "$$$$Automate$$$$" ) // $$$$ AGE 2006-10-06: 
    , id_           ( message.oid_automate )
{
    RegisterSelf( *this );

    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    // $$$$ AGE 2006-10-06: 
    dictionary.Register( tools::translate( "Automat", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Automat", "Info/Nom" ), name_ );
    dictionary.Register( tools::translate( "Automat", "Hiérarchie/Supérieur" ), group_ );

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

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
    {
        ChangeKnowledgeGroup( & groupResolver_.Get( message.oid_groupe_connaissance ) );
        controller_.Update( *(Automat_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::ChangeKnowledgeGroup
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::ChangeKnowledgeGroup( KnowledgeGroup_ABC* group )
{
    if( group_ )
        group_->RemoveAutomat( id_ );
    group_ = group;
    if( group_ )
        group_->AddAutomat( id_, *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetKnowledgeGroup
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC& Automat::GetKnowledgeGroup() const
{
    return *group_;
}
