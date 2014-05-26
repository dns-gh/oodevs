// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObstacleType.h"
#include "moc_ParamObstacleType.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/ObstacleType.h"
#include "clients_kernel/OrderParameter.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacleType constructor
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
ParamObstacleType::ParamObstacleType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , comboBox_( 0 )
    , activation_( eObstacleActivation_Activated )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType destructor
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
ParamObstacleType::~ParamObstacleType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType::BuildInterface
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
QWidget* ParamObstacleType::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->setAlignment( Qt::AlignCenter );
    comboBox_ = new QComboBox( parent );
    comboBox_->addItem( tr( "True" ) );
    comboBox_->addItem( tr( "False" ) );
    comboBox_->setCurrentIndex( activation_ == eObstacleActivation_Activated ? 0 : 1 );
    layout->addWidget( comboBox_, Qt::AlignCenter );
    connect( comboBox_, SIGNAL( currentIndexChanged( int ) ), SLOT( OnCurrentIndexChanged( int ) ) );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType::CommitTo
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
void ParamObstacleType::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter(
        *new actions::parameters::ObstacleType(
            parameter_,
            IsChecked() && activation_ != eObstacleActivation_Activated ? 1 : 0 ) );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType::Visit
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
void ParamObstacleType::Visit( const actions::parameters::ObstacleType& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        SetValue( param.GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType::SetValue
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
void ParamObstacleType::SetValue( const QString& value )
{
    activation_ = ENT_Tr::ConvertToObstacleActivation( value.toStdString() );
    comboBox_->setCurrentIndex( activation_ == eObstacleActivation_Activated ? 0 : 1 );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleType::OnCurrentIndexChanged
// Created: LDC 2014-05-22
// -----------------------------------------------------------------------------
void ParamObstacleType::OnCurrentIndexChanged( int index )
{
    if( index == 0 )
        activation_ = eObstacleActivation_Activated;
    else
        activation_ = eObstacleActivation_Deactivated;
    Update();
}
