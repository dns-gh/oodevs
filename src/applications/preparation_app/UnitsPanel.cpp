// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitsPanel.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"
#include "UnitListView.h"
#include "clients_kernel/AgentTypes.h"

using namespace kernel;
using namespace gui;

namespace
{
    ValuedComboBox< std::string >* BuildNatureFieldsCombo( QWidget* parent )
    {
        ValuedComboBox< std::string >* combo = new ValuedComboBox< std::string >( parent );
        // $$$$ SBO 2006-08-28: Iterator sur AgentNatures?
        combo->AddItem( tools::translate( "Preparation", "Niveau" ), "level" );
        combo->AddItem( tools::translate( "Preparation", "Arme" ), "weapon" );
        combo->AddItem( tools::translate( "Preparation", "Specialisation" ), "specialization" );
        combo->AddItem( tools::translate( "Preparation", "Qualificatif" ), "qualifier" );
        combo->AddItem( tools::translate( "Preparation", "Categorie" ), "category" );
        combo->AddItem( tools::translate( "Preparation", "TypeMobilite" ), "mobility" );
        combo->AddItem( tools::translate( "Preparation", "QualificatifAtlas" ), "atlas qualifier" );
        combo->AddItem( tools::translate( "Preparation", "CapaciteMission" ), "mission capacity" );
        return combo;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitsPanel::UnitsPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, AgentTypes& types, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Unités" ) )
    , controllers_( controllers )
{
    QHBox* box = new QHBox( this );
    new QLabel( tools::translate( "Preparation", "Critère de tri:" ), box );
    combo_ = BuildNatureFieldsCombo( box );
    new UnitListView( this, controllers_, types, factory );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitsPanel::~UnitsPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitsPanel::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitsPanel::NotifyUpdated( const ModelLoaded& )
{
    Show();
}
