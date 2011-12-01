// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LongNameEditor.h"
#include "moc_LongNameEditor.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DictionaryEntryType.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "preparation/Formation.h"
#include "preparation/StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LongNameEditor constructor
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
LongNameEditor::LongNameEditor( QWidget* parent, Controllers& controllers, const StaticModel& model )
    : QObject( parent )
    , controllers_( controllers )
    , types_      ( model.extensions_ )
    , selected_   ( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor destructor
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
LongNameEditor::~LongNameEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::NotifyContextMenu
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
void LongNameEditor::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::NotifyContextMenu
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
void LongNameEditor::NotifyContextMenu( const Formation_ABC& entity, ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::NotifyContextMenu
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
void LongNameEditor::NotifyContextMenu( const Automat_ABC& entity, ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::NotifyContextMenu
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
void LongNameEditor::NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::DoNotifyContextMenu
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
void LongNameEditor::DoNotifyContextMenu( const Entity_ABC& entity, ContextMenu& menu )
{
    ExtensionType* type = types_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( type && type->Find( "NomLong" ) )
    {
        selected_ = const_cast< Entity_ABC* >( &entity );
        menu.InsertItem( "NomLong", tools::translate( "LongNameEditor", "Generate long names" ), this, SLOT( Generate() ) );
    }
}

namespace
{
    QString GetEntityName( const Entity_ABC& entity )
    {
        if( entity.GetTypeName() == Formation_ABC::typeName_ )
        {
            const Formation& formation = static_cast< const Formation& >( entity );
            return formation.EntityImplementation< Formation_ABC >::GetName();
        }
        return entity.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::Generate
// Created: JSR 2011-09-12
// -----------------------------------------------------------------------------
void LongNameEditor::Generate()
{
    if( selected_ )
    {
        ExtensionType* type = types_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
        if( !type )
            return;
        AttributeType* attribute = type->Find( "NomLong" );
        if( !attribute )
            return;

        QString longName = GetEntityName( *selected_ );
        const Entity_ABC* entity = selected_;
        const TacticalHierarchies* pTactical = selected_->Retrieve< TacticalHierarchies >();
        while( entity && pTactical )
        {
            if( entity->GetTypeName() == Formation_ABC::typeName_ )
            {
                const Formation_ABC& formation = static_cast< const Formation_ABC& >( *entity );
                QString level = formation.GetLevel().GetName();
                if( level == "iii" || level.endsWith( 'x' ) ) // >= Régiment
                    break;
            }
            entity = pTactical->GetSuperior();
            if( entity )
            {
                if( entity->GetTypeName() == Team_ABC::typeName_ )
                    break;
                longName += "." + GetEntityName( *entity );
                pTactical = entity->Retrieve< TacticalHierarchies >();
            }
        }

        TransmitToSubordinates( *selected_.ConstCast(), longName, *attribute );
        SetExtension( *selected_.ConstCast(), longName, *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::GetCountryAlias
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
std::string LongNameEditor::GetCountryAlias( const std::string& country ) const
{
    if( !country.empty() )
        if( DictionaryType* dico = types_.tools::StringResolver< DictionaryType >::Find( "T_Nationalite" ) )
            if( DictionaryEntryType* entry = dico->Find( country ) )
                return entry->GetAlias();
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::TransmitToSubordinates
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
void LongNameEditor::TransmitToSubordinates( const Entity_ABC& entity, const QString& name, const AttributeType& attribute ) const
{
    const TacticalHierarchies* pTactical = entity.Retrieve< TacticalHierarchies >();
    if( pTactical )
    {
        tools::Iterator< const Entity_ABC& > children = pTactical->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            Entity_ABC& child = const_cast< Entity_ABC& >( children.NextElement() );
            QString childName = GetEntityName( child ) + "." + name;
            SetExtension( child, childName, attribute );
            TransmitToSubordinates( child, childName, attribute );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LongNameEditor::SetExtension
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
void LongNameEditor::SetExtension( Entity_ABC& entity, const QString& name, const AttributeType& attribute ) const
{
    if( DictionaryExtensions* ext = entity.Retrieve< DictionaryExtensions >() )
    {
        bool extends = false;
        QString typeName = entity.GetTypeName();
        if( typeName == Agent_ABC::typeName_ )
            extends = attribute.Extends( "unit" );
        else if( typeName == Automat_ABC::typeName_ )
            extends = attribute.Extends( "automat" );
        else if( typeName == Formation_ABC::typeName_ )
            extends = attribute.Extends( "formation" );
        else if( typeName == Population_ABC::typeName_ )
            extends = attribute.Extends( "crowd" );
        else if( typeName == Inhabitant_ABC::typeName_ )
            extends = attribute.Extends( "population" );
        else if( typeName == Team_ABC::typeName_ )
            extends = attribute.Extends( "party" );
        if( extends )
        {
            ext->SetEnabled( true );
            QString longName = name;
            std::string country = ext->GetValue( "Nationalite" );
            if( !country.empty() )
                longName += ( "." + GetCountryAlias( country ) ).c_str();
            longName = longName.remove( ' ' ).replace( 'é', "e", Qt::CaseInsensitive ).replace( 'è', "e", Qt::CaseInsensitive )
                .replace( 'ç', "c", Qt::CaseInsensitive ).replace( 'à', "a", Qt::CaseInsensitive ).replace( QRegExp( "[^a-zA-Z0-9.]" ), "" )
                .toUpper();
            int min;
            int max;
            attribute.GetMinMaxLength( min, max );
            if( max != -1 )
                longName = longName.left( max );
            ext->SetValue( "NomLong", longName.toStdString() );
            controllers_.controller_.Update( *ext );
            controllers_.controller_.Update( entity );
        }
    }
}
