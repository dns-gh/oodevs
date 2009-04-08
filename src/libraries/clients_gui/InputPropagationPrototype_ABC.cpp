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
#include "moc_InputPropagationPrototype_ABC.cpp"
#include "Tools.h"
#include <qfiledialog.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
InputPropagationPrototype_ABC::InputPropagationPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "InputPropagationPrototype_ABC", "Propagation" ) )    
{    
    new QLabel( tools::translate( "InputPropagationPrototype_ABC", "NBC Agent:" ), this );

    pPathButton_ = new QPushButton( tools::translate( "InputPropagationPrototype_ABC", "Input file..." ), this );
    connect( pPathButton_, SIGNAL( clicked() ), this, SLOT( LoadPath() ) );

    new QLabel( tools::translate( "InputPropagationPrototype_ABC", "Lookup data:" ), this );
    dataField_ = new ValuedComboBox< std::string >( this );
    // TODO : %TMP%
    dataField_->AddItem( std::string( "Mesure C" ), std::string( "nom_var_shp_mesure_C" ) );
    dataField_->AddItem( std::string( "Mesure Ct" ), std::string( "nom_var_shp_mesure_Ct" ) );
    
    new QLabel( tools::translate( "InputPropagationPrototype_ABC", "Send data:" ), this );
    exportData_ = new QCheckBox( this );
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
// Name: InputPropagationPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool InputPropagationPrototype_ABC::CheckValidity() const
{
    return ! source_.empty() && ! dataField_->GetValue().empty();
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::LoadPath
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void InputPropagationPrototype_ABC::LoadPath()
{
    static const int length = 22;
    QString fileName = QFileDialog::getOpenFileName( "./", tools::translate( "InputPropagationPrototype_ABC", "Propagation (propagation.xml)" ), 0, tools::translate( "InputPropagationPrototype_ABC", "Open File" ) );
    if ( fileName.ascii() )
    {        
        source_ = std::string( fileName.ascii() );
        if( source_.size() > length )
            pPathButton_->setText( std::string( "..." + std::string( source_.begin() + ( source_.size() - length ), source_.end() ) ).c_str() );
        else
            pPathButton_->setText( source_.c_str() );
    }
}
