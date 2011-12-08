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
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Inhabitant.h"
#include "preparation/Inhabitants.h"
#include "preparation/Population.h"
#include "preparation/Populations.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel constructor
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
ExtensionsPanel::ExtensionsPanel( QMainWindow* parent, Controllers& controllers, const ExtensionTypes& extensions, const tools::Resolver< Agent_ABC >& agents,
                                  gui::ItemFactory_ABC& factory, const gui::EntitySymbols& icons, const Profile_ABC& profile, const char* name /*= 0*/ )
    : gui::ExtensionsPanel( parent, controllers, extensions, agents, factory, icons, profile, name )
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
