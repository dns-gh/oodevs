// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DisasterPrototype_ABC.h"
#include "moc_DisasterPrototype_ABC.cpp"
#include "RichCheckBox.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "Tools.h"

#include "clients_kernel/Tools.h"

#include "tools/GeneralConfig.h"

#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype_ABC::DisasterPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config )
    : ObjectAttributePrototype_ABC( parent, "DisasterPrototypeABC", tools::translate( "gui::DisasterPrototype_ABC", "Propagation" ) )
{
    SubObjectName subObject( "DisasterPrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2, 7, 10 );
    layout->setMargin( 5 );
    layout->addWidget( new QLabel( tools::translate( "gui::DisasterPrototype_ABC", "Propagation Model:" ) ) );
    propagationFiles_ = new ValuedComboBox< std::string >( "propagationFiles", 0 );
    layout->addWidget( propagationFiles_ );
    FillInPaths( config.GetRootDir() );
    checkbox_ = new RichCheckBox( "time", tools::translate( "gui::DisasterPrototype_ABC", "Time:" ) );
    connect( checkbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );
    layout->addWidget( checkbox_ );
    date_ = new RichWidget< QDateTimeEdit >( "date" );
    date_->setEnabled( false );
    layout->addWidget( date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC destructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype_ABC::~DisasterPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::FillInPaths
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype_ABC::FillInPaths( const tools::Path& root )
{
    tools::Path path = root / "data/propagations";
    const tools::Path::T_Paths results = path.ListElements( boost::bind( &tools::IsPropagationDir, _1 ) );
    for( auto it = results.begin(); it != results.end(); ++it )
    {
        const std::string text = it->ToUTF8();
        propagationFiles_->AddItem( text, text );
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::CheckValidity
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
bool DisasterPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return !propagationFiles_->GetValue().empty();
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::OnStateChanged
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype_ABC::OnStateChanged( int state )
{
    date_->setEnabled( state == Qt::Checked );
}
