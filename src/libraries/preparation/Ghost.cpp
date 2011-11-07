// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Ghost.h"
#include "IdManager.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/MergingTacticalHierarchies.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Ghost constructor
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
Ghost::Ghost( kernel::Controller& controller, IdManager& idManager, const GhostPrototype& prototype )
    : EntityImplementation< Ghost_ABC >( controller, idManager.GetNextId(), prototype.name_.c_str() )
    , ghostType_ ( prototype.ghostType_ )
    , type_      ( prototype.type_.c_str() )
    , symbol_    ( prototype.symbol_ )
    , nature_    ( prototype.nature_ )
    , level_     ( prototype.level_ )
    , converted_ ( false )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost constructor
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
Ghost::Ghost( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis, kernel::SymbolFactory& symbolsFactory )
    : EntityImplementation< Ghost_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name", "" ).c_str() )
    , ghostType_ ( ENT_Tr::ConvertToGhostType( xis.attribute< std::string >( "ghost-type", ENT_Tr::ConvertFromGhostType( eGhostType_Invalid ) ) ) )
    , type_      ( xis.attribute< std::string >( "type", "" ).c_str() )
    , nature_    ( xis.attribute< std::string >( "nature", "" ) )
    , level_     ( xis.attribute< std::string >( "level", "" ) )
    , converted_ ( false )
{
    assert( ghostType_ != eGhostType_Invalid && !nature_.empty() && !level_.empty() );

    symbol_ = symbolsFactory.CreateSymbol( nature_ );
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost::Ghost( Controller& controller, IdManager& idManager, xml::xistream& xis, kernel::Entity_ABC& parent, E_GhostType ghostType )
    : EntityImplementation< Ghost_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name", "" ).c_str() )
    , ghostType_ ( ghostType )
    , type_      ( xis.attribute< std::string >( "type", "" ).c_str() )
    , symbol_    ( "symbols/s*gpu" )
    , nature_    ( "undefined/undefined/undefined/undefined" )
    , level_     ( ( ghostType == eGhostType_Automat ) ? "ii" : "ooo" )
    , converted_ ( true )
{
    assert( ghostType_ != eGhostType_Invalid );

    if( const kernel::TacticalHierarchies* pHierarchy = parent.Retrieve< kernel::TacticalHierarchies >() )
    {
        kernel::App6Symbol::SetKarma( symbol_, pHierarchy->GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );

        std::string levelSymbol = pHierarchy->GetLevel();
        if ( !levelSymbol.empty() )
            level_ = MergingTacticalHierarchies::DecreaseLevel( levelSymbol );
    }

    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost destructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost::~Ghost()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Ghost::InitializeSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void Ghost::InitializeSymbol() const
{
    kernel::App6Symbol::SetKarma( symbol_, Get< kernel::CommunicationHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Ghost::Draw
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, -1.f );
        tools.DrawApp6Symbol( GetLevelSymbol(), where, -1.f );
    }
}

// -----------------------------------------------------------------------------
// Name: Ghost::CreateDictionary
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Ghost& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Phantom type" ), ENT_Tr::ConvertFromGhostType( ghostType_, ENT_Tr::eToTr ) );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Name" ), name_, *this, &Ghost::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Type" ), type_ );
}

// -----------------------------------------------------------------------------
// Name: Ghost::DisplayInTooltip
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    QString id = QString( "[%1]" ).arg( GetId() );
    displayer.Item( "" ).Start( Styles::bold )
        .Add( ( Ghost_ABC* ) this )
        .AddToDisplay( id );
    displayer.End();
}

// -----------------------------------------------------------------------------
// Name: Ghost::SerializeAttributes
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::SerializeAttributes( xml::xostream& xos ) const
{
    assert( ghostType_ != eGhostType_Invalid );
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "ghost-type", ENT_Tr::ConvertFromGhostType( ghostType_, ENT_Tr::eToSim ) )
        << xml::attribute( "type", type_.ascii() )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "nature", nature_ )
        << xml::attribute( "level", level_ );
}

// -----------------------------------------------------------------------------
// Name: Ghost::Rename
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void Ghost::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetType
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
const QString& Ghost::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetGhostType
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
E_GhostType Ghost::GetGhostType() const
{
    return ghostType_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& Ghost::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetSymbol
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
const std::string& Ghost::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetLevelSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string Ghost::GetLevelSymbol() const
{
    return "levels/" + level_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::IsConverted
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
bool Ghost::IsConverted() const
{
    return converted_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::UpdateSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void Ghost::UpdateSymbol( const std::string& level, const std::string& nature, const std::string& symbol )
{
    level_ = level;
    nature_ = nature;
    symbol_ = symbol;
}
