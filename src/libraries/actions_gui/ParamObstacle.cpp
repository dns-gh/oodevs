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
    , Param_ABC   ( parameter.GetName().c_str(), parameter.IsOptional() )
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
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    RemoveFromController();
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
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );

    // Type
    {
        typeCombo_ = new ::gui::ValuedComboBox< const kernel::ObjectType* >( parent );
        typeCombo_->setSorting( true );
        tools::Iterator< const kernel::ObjectType& > it = objectTypes_.StringResolver< kernel::ObjectType >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::ObjectType& type = it.NextElement();
            typeCombo_->AddItem( type.GetName(), &type );
        }
        layout->addWidget( new QLabel( tr( "Type:" ), parent ) );
        layout->addWidget( typeCombo_ );
    }

    // Target type
    {
        QLabel* label = new QLabel( tr( "Demolition target type:" ), parent );
        obstacleTypeCombo_ = new ::gui::ValuedComboBox< unsigned int >( parent );
        for( unsigned int i = 0; i < eNbrDemolitionTargetType ; ++i )
            obstacleTypeCombo_->AddItem( tools::ToString( ( E_DemolitionTargetType)i ), i );
        connect( this, SIGNAL( ToggleReservable( bool ) ), label, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleReservable( bool ) ), obstacleTypeCombo_, SLOT( setShown( bool ) ) );
        layout->addWidget( label );
        layout->addWidget( obstacleTypeCombo_ );
    }

    // Density
    {
        density_ = new ParamNumericField( kernel::OrderParameter( tr( "Density" ).ascii(), "density", false ), true );
        QGroupBox* densityBox = static_cast< QGroupBox* >( density_->BuildInterface( parent ) );
        density_->SetLimits( 0.f, 5.f ); // $$$$ ABR 2011-11-21: Must be after BuildInterface call
        densityBox->layout()->setMargin( 0 );
        densityBox->layout()->setSpacing( 0 );
        layout->addWidget( densityBox );
    }

    // TC2
    {
        tc2_ = new ParamAutomat( this, kernel::OrderParameter( tr( "TC2" ).ascii(), "tc2", false ), controller_ );
        QGroupBox* tc2Box = static_cast< QGroupBox* >( tc2_->BuildInterface( parent ) );
        tc2Box->layout()->setMargin( 0 );
        tc2Box->layout()->setSpacing( 0 );
        layout->addWidget( tc2Box );
    }

    // Location
    {
        location_ = new ParamLocation( kernel::OrderParameter( tr( "Obstacle location" ).ascii(), "location", false ), layer_, converter_ );
        QGroupBox* locationBox = static_cast< QGroupBox* >( location_->BuildInterface( parent ) );
        locationBox->layout()->setMargin( 0 );
        locationBox->layout()->setSpacing( 0 );
        layout->addWidget( locationBox );
    }

    connect( typeCombo_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    OnTypeChanged();
    return group_;
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
    Param_ABC::RemoveFromController();
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
// Name: ParamObstacle::InternalCheckValidity
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool ParamObstacle::InternalCheckValidity() const
{
    return typeCombo_->count() > 0 &&
        ( !typeCombo_->GetValue()->HasBuildableDensity() || density_->CheckValidity() ) &&
        ( !typeCombo_->GetValue()->HasLogistic() || tc2_->CheckValidity() ) &&
        location_->CheckValidity();
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
    if( IsChecked() )
    {
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
    else
        action.AddParameter( *new actions::parameters::EngineerConstruction( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Draw
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ParamObstacle::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
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
