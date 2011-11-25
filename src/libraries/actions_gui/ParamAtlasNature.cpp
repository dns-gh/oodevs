// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAtlasNature.h"
#include "moc_ParamAtlasNature.cpp"
#include "actions/Action_ABC.h"
#include "actions/AtlasNature.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"
#include <Qt3Support/q3buttongroup.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::ParamAtlasNature( QObject* parent, const kernel::OrderParameter& parameter, const kernel::AtlasNatures& natures )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , natures_  ( natures )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::~ParamAtlasNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamAtlasNature::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    Q3ButtonGroup* buttonGroup = new Q3ButtonGroup( 2, Qt::Horizontal, parent );
    buttonGroup->setMargin( 0 );
    buttonGroup->setFlat( true );
    tools::Iterator< const kernel::AtlasNature& > it( natures_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::AtlasNature& nature = it.NextElement();
        new QCheckBox( nature.GetName(), buttonGroup );
        fields_.push_back( &nature );
    }
    connect( buttonGroup, SIGNAL( clicked( int ) ), SLOT( OnClicked( int ) ) );
    layout->addWidget( buttonGroup );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::CommitTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ParamAtlasNature::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::AtlasNature( parameter_, nature_ ) );
    else
        action.AddParameter( *new actions::parameters::AtlasNature( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::OnClicked
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamAtlasNature::OnClicked( int id )
{
    nature_.Toggle( *fields_[id] );
}
