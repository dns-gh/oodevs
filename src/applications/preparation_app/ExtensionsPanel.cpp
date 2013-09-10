// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ExtensionsPanel.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Inhabitant.h"
#include "preparation/Inhabitants.h"
#include "preparation/Population.h"
#include "preparation/Populations.h"

using namespace kernel;

namespace
{
    void FindUsedIdCDB( const tools::Resolver< kernel::Agent_ABC >& agents, std::map< std::string, unsigned int >& used )
    {
        auto it = agents.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Agent_ABC& agent = it.NextElement();
            const DictionaryExtensions* ext = agent.Retrieve< DictionaryExtensions >();
            if( !ext )
                continue;
            const std::string& idCDB = ext->GetValue( "IdCDB" );
            if( !idCDB.empty() )
                used[ idCDB ]++;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel constructor
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
ExtensionsPanel::ExtensionsPanel( QMainWindow* parent, Controllers& controllers, const ExtensionTypes& extensions,
                                  const tools::Resolver< Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations )
    : gui::ExtensionsPanel( parent, controllers, extensions, agents, formations )
    , cpSuperior_( 0 )
    , idIndex_( 0 )
    , agents_( agents )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel destructor
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
ExtensionsPanel::~ExtensionsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::Purge
// Created: MMC 2013-09-09
// -----------------------------------------------------------------------------
void ExtensionsPanel::Purge()
{
    usedIdCDB_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::Load
// Created: MMC 2013-09-09
// -----------------------------------------------------------------------------
void ExtensionsPanel::Load()
{
    FindUsedIdCDB( agents_, usedIdCDB_ );
}

namespace
{
    class ChangeNationality : private boost::noncopyable
    {
    public:
        ChangeNationality( const std::string& nationality ) : nationality_( nationality ) {}
        ~ChangeNationality() {}
        virtual void operator()( const Entity_ABC& entity ) const
        {
            if( DictionaryExtensions* ext = const_cast< DictionaryExtensions* >( entity.Retrieve< DictionaryExtensions >() ) )
            {
                ext->SetEnabled( true );
                ext->SetValue( "Nationalite", nationality_ );
            }
        }
    private:
        std::string nationality_;
    };

    void ChangeNationalityRecursive( const Entity_ABC& entity, const ChangeNationality& functor )
    {
        const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
        if( !hierarchies )
            return;
        tools::Iterator< const Entity_ABC& > subIt = hierarchies->CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const Entity_ABC& subEntity = subIt.NextElement();
            functor( subEntity );
            ChangeNationalityRecursive( subEntity, functor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnChangeNationality
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnChangeNationality()
{
    if( !selected_ )
        return;
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    if( !ext )
        return;
    const std::string& country = ext->GetValue( "Nationalite" );
    if( !country.empty() )
    {
        const ChangeNationality functor( country );
        ChangeNationalityRecursive( *selected_, functor );
        if( selected_->GetTypeName() == Team_ABC::typeName_ )
        {
            if( Populations* populations = selected_->Retrieve< Populations >() )
                populations->Apply( functor );
            if( Inhabitants* inhabitants = selected_->Retrieve< Inhabitants >() )
                inhabitants->Apply( functor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnChangeTypeSIOC
// Created: MMC 2013-08-30
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnChangeTypeSIOC()
{
    if( !selected_ )
        return;
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    if( !ext )
        return;
    const TacticalHierarchies* hierarchies = selected_->Retrieve< TacticalHierarchies >();
    if( !hierarchies )
        return;
    if( ext->GetValue( "TypeSIOC" ).empty() )
        return;
    if( !ext->GetValue( "IdCDB" ).empty() )
        return;
    for( int i = 0; i< allIdCDB_.size(); ++i )
    {
        int curIndex = ( i + idIndex_ ) % allIdCDB_.size();
        const std::string& curIdCDB = allIdCDB_[ curIndex ];
        if( usedIdCDB_.find( curIdCDB ) == usedIdCDB_.end() )
        {
            ext->SetValue( "IdCDB", curIdCDB );
            usedIdCDB_[ curIdCDB ] = 1;
            NotifyUpdated( *ext );
            idIndex_ = curIndex + 1;
            if( idIndex_ >= allIdCDB_.size() )
                idIndex_ = 0;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnChangeIdCDB
// Created: MMC 2013-09-03
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnChangeIdCDB( const std::string& idCDB )
{
    if( !selected_ )
        return;
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    if( !ext )
        return;
    const TacticalHierarchies* hierarchies = selected_->Retrieve< TacticalHierarchies >();
    if( !hierarchies )
        return;
    const std::string& previousIdCDB = ext->GetValue( "IdCDB" );
    auto it = usedIdCDB_.find( previousIdCDB );
    if( it != usedIdCDB_.end() )
    {
        if( it->second <= 1)
        {
            usedIdCDB_.erase( it );
            idIndex_ = 0;
        }
        else
            it->second--;
    }
    if( !idCDB.empty() )
        usedIdCDB_[ idCDB ]++;
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::SetAllIdCDB
// Created: MMC 2013-08-30
// -----------------------------------------------------------------------------
void ExtensionsPanel::SetAllIdCDB( const QStringList& IdCDB )
{
    if( IdCDB.empty() )
        return;
    if( !allIdCDB_.empty() )
        return;
    allIdCDB_.reserve( IdCDB.size() );
    for( auto it = IdCDB.begin(); it != IdCDB.end(); ++it )
        if( !it->isEmpty() )
            allIdCDB_.push_back( it->toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifyContextMenu
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifyContextMenu
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifyContextMenu
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( team, menu );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::DoNotifyContextMenu
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    cpSuperior_ = &entity;

    ContextMenu* subMenu = menu.SubMenu( "Helpers", tools::translate( "ExtensionsPanel", "Extension" ), false, 12 );
    if( subMenu )
    {
        subMenu->InsertItem( "Command", tools::translate( "ExtensionsPanel", "Delete CP extensions" ), this, SLOT( OnDeleteCPExtensions() ) );
        subMenu->InsertItem( "Command", tools::translate( "ExtensionsPanel", "Fill empty CP extensions" ), this, SLOT( OnFillEmptyCPExtensions() ) );
    }
}

namespace
{
    bool IsAgentOrGhostAgent( const Entity_ABC& entity )
    {
        return ( entity.GetTypeName() == kernel::Agent_ABC::typeName_ ) ||
               ( entity.GetTypeName() == kernel::Ghost_ABC::typeName_ && static_cast< const kernel::Ghost_ABC& >( entity ).GetGhostType() == eGhostType_Agent );
    }

    void DeleteCPExtensions( const kernel::Entity_ABC& entity )
    {
        const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
        if( !hierarchies )
            return;
        tools::Iterator< const Entity_ABC& > subIt = hierarchies->CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const Entity_ABC& subEntity = subIt.NextElement();
            if( IsAgentOrGhostAgent( subEntity ) )
            {
                if( DictionaryExtensions* ext = const_cast< DictionaryExtensions* >( subEntity.Retrieve< DictionaryExtensions >() ) )
                {
                    ext->Reset( "TypeSIOC" );
                    ext->Reset( "TypePC" );
                }
            }
            else
                DeleteCPExtensions( subEntity );
        }
    }

    std::pair< std::string, std::string > GetTypeAndLevel( const kernel::Entity_ABC& entity )
    {
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            const kernel::AgentType& type = static_cast< const kernel::Agent_ABC& >( entity ).GetType();
            return std::make_pair< std::string, std::string >( type.GetTypeName(), type.GetNature().GetLevel() );
        }
        if( entity.GetTypeName() == kernel::Ghost_ABC::typeName_ )
        {
            const kernel::Ghost_ABC& ghost = static_cast< const kernel::Ghost_ABC& >( entity );
            return std::make_pair< std::string, std::string >( ghost.GetType().toAscii().constData(), ghost.GetLevel() );
        }
        return std::make_pair< std::string, std::string >( "", "" );
    }

    void FillCPExtensions( const kernel::Entity_ABC& entity )
    {
        const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >();
        if( !hierarchies )
            return;
        tools::Iterator< const Entity_ABC& > subIt = hierarchies->CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const Entity_ABC& subEntity = subIt.NextElement();
            if( IsAgentOrGhostAgent( subEntity ) )
            {
                const kernel::CommandPostAttributes_ABC* pc = subEntity.Retrieve< CommandPostAttributes_ABC >();
                DictionaryExtensions* ext = const_cast< DictionaryExtensions* >( subEntity.Retrieve< DictionaryExtensions >() );
                if( ext && pc && pc->IsCommandPost() )
                {
                    std::pair< std::string, std::string > typeAndLevel = GetTypeAndLevel( subEntity );
                    ext->SetEnabled( true );
                    if( ext->GetValue( "TypePC" ).empty() )
                        ext->SetValue( "TypePC", "PCscipio" );
                    if( ext->GetValue( "TypeSIOC" ).empty() )
                    {
                        if( typeAndLevel.first == "Pion ASS" || typeAndLevel.first == "Pion ASA" )
                            ext->SetValue( "TypeSIOC", "ATLAS" );
                        else
                        {
                            if( typeAndLevel.second == "i" || typeAndLevel.second == "ii" )
                                ext->SetValue( "TypeSIOC", "SIR" );
                            else if( typeAndLevel.second == "x" || typeAndLevel.second == "xx" )
                                ext->SetValue( "TypeSIOC", "SICF" );
                            else if( typeAndLevel.second == "c" || typeAndLevel.second == "b" ||
                                     typeAndLevel.second == "o" || typeAndLevel.second == "oo" ||
                                     typeAndLevel.second == "ooo" )
                                ext->SetValue( "TypeSIOC", "SITEL" );
                        }
                    }
                }
            }
            else
                FillCPExtensions( subEntity );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnDeleteCPExtensions
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnDeleteCPExtensions()
{
    if( !cpSuperior_ )
        return;
    DeleteCPExtensions( *cpSuperior_ );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnFillEmptyCPExtensions
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnFillEmptyCPExtensions()
{
    if( !cpSuperior_ )
        return;
    FillCPExtensions( *cpSuperior_ );
}
