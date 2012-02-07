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
#include "clients_kernel/Tools.h"
#include "LogisticLevelAttritube.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/App6Symbol.h"
#include <xeumeuleu/xml.hpp>
#include <QtGui/qmessagebox.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const AutomatType& type, Controller& controller, IdManager& idManager, const QString& name )
    : EntityImplementation< Automat_ABC >( controller, idManager.GetNextId(), "" )
    , type_       ( type )
    , karmaFactor_( 0.f )
{
    name_ = name.isEmpty() ? type.GetName().c_str() + QString( " [%1]" ).arg( id_ )
                           : name + " " + QString( "[%1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xml::xistream& xis, Controller& controller, IdManager& idManager, const AutomatType& type )
    : EntityImplementation< Automat_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_       ( type )
    , karmaFactor_( 0.f )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
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
// Name: Automat::GetType
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
{
    return type_;
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

namespace
{
    // $$$$ LGY 2012-02-07 : hardcoded for displaying !!!
    float GetFactor( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return 0.f;
        else if( karma == kernel::Karma::enemy_ )
            return 140.f;
        else if( karma == kernel::Karma::neutral_ )
            return 0.f;
        return 0.f;
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, 2 );
        const geometry::Point2f center( where.X(), where.Y() + karmaFactor_ );
        tools.DrawApp6Symbol( level_, center, 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Automat::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbol_ == symbol && level_ == level )
        return;
    symbol_ = symbol;
    level_ = level;
    const kernel::Karma& karma = hierarchies.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    karmaFactor_ = GetFactor( karma );
    kernel::App6Symbol::SetKarma( symbol_, karma );
}

// -----------------------------------------------------------------------------
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Automat& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Name" ), name_, *this, &Automat::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Type" ), constSelf.type_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::SerializeAttributes
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "type", type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetLogisticLevel
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const LogisticLevel& Automat::GetLogisticLevel() const
{
    return Get< LogisticLevelAttritube >().GetLogisticLevel();
}
