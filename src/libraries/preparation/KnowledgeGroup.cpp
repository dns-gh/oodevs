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
#include "clients_kernel/Automat_ABC.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Controller& controller, IdManager& idManager )
    : controller_( controller )
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
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager )
    : controller_( controller )
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
    DestroyExtensions();
    controller_.Delete( *(KnowledgeGroup_ABC*)this );
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
// Name: KnowledgeGroup::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_ );
}
