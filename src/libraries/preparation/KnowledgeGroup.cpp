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
    : EntityImplementation< KnowledgeGroup_ABC >( controller, idManager.GetNextId(), "" )
    , type_( "Standard" ) // $$$$ SBO 2006-09-06: 
{
    RegisterSelf( *this );
    name_ = QString( "Knowledge group %1" ).arg( id_ ); // $$$$ AGE 2006-08-23: au moins tr
}

// $$$$ AGE 2006-10-12: sauvegarder et relier le nom ?
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, ReadId( xis ), "" )
{
    std::string type;
    xis >> attribute( "type", type );
    type_ = type.c_str();
    
    RegisterSelf( *this );
    name_ = QString( "Knowledge group %1" ).arg( id_ ); // $$$$ AGE 2006-08-23: tr
    idManager.Lock( id_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ReadId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
unsigned long KnowledgeGroup::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void KnowledgeGroup::Rename( const QString& name )
{
    name_ = name;
    Touch();
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
