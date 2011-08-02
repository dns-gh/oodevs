// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObstacle.h"
#include "moc_ParamObstacle.cpp"
#include "ParamLocation.h"
#include "ParamNumericField.h"
#include "ParamAutomat.h"
#include "actions/Action_ABC.h"
#include "actions/EngineerConstruction.h"
#include "actions/ObstacleType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "tools/Resolver.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller )
    : QObject     ( parent )
    , Param_ABC   ( parameter.GetName().c_str() )
    , parameter_  ( parameter )
    , objectTypes_( objectTypes )
    , layer_      ( layer )
    , converter_  ( converter )
    , controller_ ( controller )
    , typeCombo_  ( 0 )
    , obstacleTypeCombo_( 0 )
    , location_   ( 0 )
    , density_    ( 0 )
    , tc2_        ( 0 )
    , optional_   ( parameter.IsOptional() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    delete tc2_;
    delete density_;
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamObstacle::BuildInterface( QWidget* parent )
{
    Q3GroupBox* group = new Q3GroupBox( 1, Qt::Horizontal, GetName(), parent );
    Q3HBox* box = new Q3HBox( group );
    box->setSpacing( 5 );
    new QLabel( tr( "Type:" ), box );
    typeCombo_ = new ::gui::ValuedComboBox< const kernel::ObjectType* >( box );
    typeCombo_->setSorting( true );
    tools::Iterator< const kernel::ObjectType& > it = objectTypes_.StringResolver< kernel::ObjectType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::ObjectType& type = it.NextElement();
        typeCombo_->AddItem( type.GetName(), &type );
    }

    box = new Q3HBox( group );
    box->setSpacing( 5 );

    {
        QLabel* label = new QLabel( tr( "Demolition target type:" ), box );
        obstacleTypeCombo_ = new ::gui::ValuedComboBox< unsigned int >( box );
        for( unsigned int i = 0; i < eNbrDemolitionTargetType ; ++i )
            obstacleTypeCombo_->AddItem( tools::ToString( ( E_DemolitionTargetType)i ), i );
        connect( this, SIGNAL( ToggleReservable( bool ) ), label, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleReservable( bool ) ), obstacleTypeCombo_, SLOT( setShown( bool ) ) );
    }

    density_ = new ParamNumericField( kernel::OrderParameter( tr( "Density" ).ascii(), "density", false ), true );
    density_->BuildInterface( group );
    density_->SetLimits( 0.f, 5.f );

    tc2_ = new ParamAutomat( this, kernel::OrderParameter( tr( "TC2" ).ascii(), "tc2", false ), controller_ );
    tc2_->BuildInterface( group );

    location_ = new ParamLocation( kernel::OrderParameter( tr( "Location" ).ascii(), "location", false ), layer_, converter_ );
    location_->BuildInterface( group );

    connect( typeCombo_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    OnTypeChanged();
    return group;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RemoveFromController
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RemoveFromController()
{
    location_->RemoveFromController();
    tc2_->RemoveFromController();
    density_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RegisterIn
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RegisterIn( kernel::ActionController& controller )
{
    location_->RegisterIn( controller );
    tc2_->RegisterIn( controller );
    density_->RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool ParamObstacle::CheckValidity()
{
    // $$$$ SBO 2007-04-25: check optional
    if( !typeCombo_->count() )
        return false;
    bool bOk = true;
    if( typeCombo_->GetValue()->HasBuildableDensity() )
        bOk = density_->CheckValidity();
    if( typeCombo_->GetValue()->HasLogistic() )
        bOk = tc2_->CheckValidity();
    bOk = bOk && location_->CheckValidity();
    return bOk;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    std::auto_ptr< actions::parameters::EngineerConstruction > param( new actions::parameters::EngineerConstruction( parameter_, *type ) );
    if( type->HasBuildableDensity() )
        density_->CommitTo( *param );
    if( type->HasLogistic() )
        tc2_->CommitTo( *param );
    if( type->CanBeReservedObstacle() )
        param->AddParameter( *new actions::parameters::ObstacleType( kernel::OrderParameter( tr( "Obstacle type" ).ascii(), "obstacletype", false ), obstacleTypeCombo_->GetValue() ) );
    location_->CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Draw
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ParamObstacle::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
//    if( const ObjectType* type = typeCombo_->GetValue() )
//        type->Draw( location_->GetPosition(), viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::OnTypeChanged
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamObstacle::OnTypeChanged()
{
    density_->Hide();
    tc2_->Hide();
    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    if( type->HasBuildableDensity() )
        density_->Show();
    if( type->HasLogistic() )
        tc2_->Show();
    location_->SetShapeFilter( type->CanBePoint(), type->CanBeLine(), type->CanBePolygon(), type->CanBeCircle(), type->CanBeRectangle() );
    emit ToggleReservable( type->CanBeReservedObstacle() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamObstacle::IsOptional() const
{
    return parameter_.IsOptional();
}
