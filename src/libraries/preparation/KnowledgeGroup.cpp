// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroup.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Controller& controller, const Team_ABC& team, IdManager& idManager )
    : controller_( controller )
    , team_( team )
    , id_ ( idManager.GetNextId() )
    , type_( "Standard" ) // $$$$ SBO 2006-09-06: 
{
    RegisterSelf( *this );
    controller_.Create( *(KnowledgeGroup_ABC*)this );
    name_ = QString( "Gtia %1" ).arg( id_ ); // $$$$ AGE 2006-08-23: 
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, const kernel::Team_ABC& team, IdManager& idManager )
    : controller_( controller )
    , team_( team )
{
    std::string type;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "type", type );
    type_ = type.c_str();
    idManager.Lock( id_ );

    RegisterSelf( *this );
    controller_.Create( *(KnowledgeGroup_ABC*)this );
    name_ = QString( "Gtia %1" ).arg( id_ ); // $$$$ AGE 2006-08-23: 
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    DeleteAll();
    const_cast< Team_ABC& >( team_ ).Resolver< KnowledgeGroup_ABC >::Remove( id_ ); // $$$$ SBO 2006-10-03: 
    DestroyExtensions();
    controller_.Delete( *(KnowledgeGroup_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::AddAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::AddAutomat( unsigned long id, Agent_ABC& automat )
{
    Resolver< Agent_ABC >::Register( id, automat );
    controller_.Update( *(KnowledgeGroup_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::RemoveAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::RemoveAutomat( unsigned long id )
{
    Resolver< Agent_ABC >::Remove( id );
    controller_.Update( *(KnowledgeGroup_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetId
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
unsigned long KnowledgeGroup::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
QString KnowledgeGroup::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
const Team_ABC& KnowledgeGroup::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void KnowledgeGroup::Select( ActionController& controller ) const
{
    controller.Select( *this );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void KnowledgeGroup::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void KnowledgeGroup::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_ );
}
