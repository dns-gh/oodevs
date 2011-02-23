// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Automat.h"
#include "IdManager.h"
#include "Tools.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/LogisticLevel.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const AutomatType& type, Controller& controller, IdManager& idManager )
    : EntityImplementation< Automat_ABC >( controller, idManager.GetNextId(), "" )
    , type_( type )
    , logisticLevel_( &kernel::LogisticLevel::none_ )
{
    name_ = type.GetName().c_str() + QString( " [%1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xml::xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : EntityImplementation< Automat_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_( agentTypes.Resolver< AutomatType, std::string >::Get( xis.attribute< std::string >( "type" ) ) )
    , logisticLevel_( &kernel::LogisticLevel::none_ )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
    idManager.Lock( id_ );

    xis >> xml::optional >> xml::attribute( "nature", nature_ );

    std::string logLevelName("none");
    xis >> xml::optional >> xml::attribute( "logistic-level", logLevelName );
    logisticLevel_ = const_cast< kernel::LogisticLevel* >( &kernel::LogisticLevel::Resolve( logLevelName ) );

    xis >> xml::optional >> xml::attribute( "color", color_ );
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
    QString id = QString( "[%1]" ).arg( id_ );
    if ( !name.endsWith( id ))
        name_ = name + " " +  id;
    else
        name_ = name;
    Touch();
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
        tools.DrawApp6Symbol( level_, where, 2 );
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
    kernel::App6Symbol::SetKarma( symbol_, hierarchies.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Automat::CreateDictionary
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void Automat::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/Name" ), name_ );
    if(type_.IsTC2())
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Automat", "Info/LogisticLevel" ), logisticLevel_ );
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
    if (nature_.length() > 0)
        xos << xml::attribute( "nature", nature_ );
    if( *logisticLevel_ != kernel::LogisticLevel::none_ )
        xos << xml::attribute( "logistic-level", logisticLevel_->GetName() );
    if (color_.length() > 0)
        xos << xml::attribute( "color", color_ );
}

// -----------------------------------------------------------------------------
// Name: Automat::SerializeAttributes
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& Automat::GetLogisticLevel() const
{
    return *logisticLevel_;
}
