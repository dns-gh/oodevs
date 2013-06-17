// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAtlasNature.h"
#include "moc_ParamAtlasNature.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/AtlasNature.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include <Qt3Support/q3buttongroup.h>
#pragma warning( pop )

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature constructor
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
ParamAtlasNature::ParamAtlasNature( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , natures_ ( builder.GetStaticModel().atlasNatures_ )
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
QWidget* ParamAtlasNature::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    Q3ButtonGroup* buttonGroup = new Q3ButtonGroup( 2, Qt::Horizontal, parent );
    buttonGroup->setMargin( 0 );
    buttonGroup->setFlat( true );
    tools::Iterator< const kernel::AtlasNature& > it( natures_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::AtlasNature& nature = it.NextElement();
        QCheckBox* checkBox = new QCheckBox( nature.GetName(), buttonGroup );
        fields_.push_back( &nature );
        checkboxes_.push_back( checkBox );
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

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamAtlasNature::Visit( const actions::parameters::AtlasNature& param )
{
    ActivateOptionalIfNeeded( param );
    const kernel::AtlasNature& nature = param.GetValue();
    unsigned short value = nature.GetValue();
    assert( fields_.size() == checkboxes_.size() );
    for( std::size_t i = 0; i < fields_.size(); ++i )
        if( fields_[ i ] != 0 && checkboxes_[ i ] != 0 )
            checkboxes_[ i ]->setCheckState( ( fields_[ i ]->GetValue() & value ) != 0 ? Qt::Checked : Qt::Unchecked );
}
