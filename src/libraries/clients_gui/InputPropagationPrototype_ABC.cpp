// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InputPropagationPrototype_ABC.h"
#include "Tools.h"
#include "tools/GeneralConfig.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
InputPropagationPrototype_ABC::InputPropagationPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation" ) )
    , root_( config.GetRootDir() )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2, 6, 10 );
    layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation Model:" ) ) );
    propagationFiles_ = new ValuedComboBox< std::string >( 0 );
    layout->addWidget( propagationFiles_ );
    FillInPaths();

    layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Lookup data:" ) ) );
    dataField_ = new ValuedComboBox< std::string >( this );
    dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure C" ), std::string( "nom_var_shp_mesure_C" ) );
    dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure Ct" ), std::string( "nom_var_shp_mesure_Ct" ) );
    layout->addWidget( dataField_ );

    layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Send data:" ) ) );
    exportData_ = new QCheckBox();
    layout->addWidget( exportData_ );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
InputPropagationPrototype_ABC::~InputPropagationPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::FillInPaths
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void InputPropagationPrototype_ABC::FillInPaths()
{
    tools::Path path = root_ / "data/propagations";
    tools::Path::T_Paths files = path.ListElements( &IsPropagationDir, true );

    for( auto it = files.begin(); it != files.end(); ++it )
        propagationFiles_->AddItem( it->ToUTF8(), it->ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool InputPropagationPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return !propagationFiles_->GetValue().empty();
}
