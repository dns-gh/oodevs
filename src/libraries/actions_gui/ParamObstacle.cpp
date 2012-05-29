// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObstacle.h"
#include "moc_ParamObstacle.cpp"
#include "InterfaceBuilder_ABC.h"
#include "ListParameter.h"
#include "ParamQuantity.h"
#include "ParamLocation.h"
#include "ParamNumericField.h"
#include "ParamAutomat.h"
#include "ParamBool.h"
#include "ParamTime.h"
#include "actions/Action_ABC.h"
#include "actions/EngineerConstruction.h"
#include "actions/ObstacleType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Units.h"
#include "tools/Resolver.h"
#include "MissionInterface_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamLocationComposite( builder, parameter )
    , objectTypes_( builder.GetStaticModel().objectTypes_ )
    , layer_      ( builder.GetParameterLayer() )
    , converter_  ( builder.GetStaticModel().coordinateConverter_ )
    , controller_ ( builder.GetControllers().actions_ )
    , typeCombo_  ( 0 )
    , obstacleTypeCombo_( 0 )
{
    location_           = static_cast< ParamLocation* > ( AddElement( "location",   tr( "Obstacle location" ).ascii() ) );
    density_            = static_cast< ParamFloat* >    ( AddElement( "float",      tr( "Density" ).ascii() ) );
    tc2_                = static_cast< ParamAutomat* >  ( AddElement( "automat",    tr( "TC2" ).ascii() ) );
    timeLimit_          = static_cast< ParamTime* >     ( AddElement( "time",       tr( "Time limit" ).ascii() ) );
    mining_             = static_cast< ParamBool* >     ( AddElement( "boolean",    tr( "Obstacle mining" ).ascii() ) );
    altitudeModifier_   = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Altitude modifier" ).ascii() ) );    

    density_            ->SetKeyName( "density" );
    altitudeModifier_   ->SetKeyName( "altitude_modifier" );
    timeLimit_          ->SetKeyName( "time_limit" );
    mining_             ->SetKeyName( "obstacle_mining" );

    altitudeModifier_->SetLimit( 0, std::numeric_limits< int >::max() );
    altitudeModifier_->SetSuffix( kernel::Units::meters.AsString() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::NotifyChanged
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
void ParamObstacle::NotifyChanged( Param_ABC& /*param*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RegisterIn
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
void ParamObstacle::RegisterIn( kernel::ActionController& /*controller*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RemoveFromController
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
void ParamObstacle::RemoveFromController()
{
    location_->RemoveFromController();
    tc2_->RemoveFromController();
    altitudeModifier_->RemoveFromController();
    timeLimit_->RemoveFromController();
    mining_->RemoveFromController();
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
            if( parameter_.HasGenObject( type.GetName() ) )
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
        QGroupBox* densityBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( density_ )->BuildInterface( parent ) );
        density_->SetLimits( 0.f, 5.f );
        densityBox->layout()->setMargin( 0 );
        densityBox->layout()->setSpacing( 0 );
        layout->addWidget( densityBox );
    }

    // TC2
    {
        QGroupBox* tc2Box = static_cast< QGroupBox* >( static_cast< Param_ABC* >( tc2_ )->BuildInterface( parent ) );
        tc2Box->layout()->setMargin( 0 );
        tc2Box->layout()->setSpacing( 0 );
        layout->addWidget( tc2Box );
    }

    // Location
    {
        QGroupBox* locationBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( location_ )->BuildInterface( parent ) );
        locationBox->layout()->setMargin( 0 );
        locationBox->layout()->setSpacing( 0 );
        layout->addWidget( locationBox );
    }

    // Altitude modifier
    {
        QGroupBox* altitudeBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( altitudeModifier_ )->BuildInterface( parent ) );
        altitudeBox->layout()->setMargin( 0 );
        altitudeBox->layout()->setSpacing( 0 );
        layout->addWidget( altitudeBox );
    }

    // Time limit
    {
        QGroupBox* timeLimitBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( timeLimit_ )->BuildInterface( parent ) );
        timeLimitBox->layout()->setMargin( 0 );
        timeLimitBox->layout()->setSpacing( 0 );
        layout->addWidget( timeLimitBox );
    }

    // Mining
    {
        QGroupBox* miningBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( mining_ )->BuildInterface( parent ) );
        miningBox->layout()->setMargin( 0 );
        miningBox->layout()->setSpacing( 0 );
        layout->addWidget( miningBox );
    }

    connect( typeCombo_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    OnTypeChanged();
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
bool ParamObstacle::CheckValidity()
{
    return InternalCheckValidity();
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
        ( !typeCombo_->GetValue()->HasTimeLimitedCapacity() || timeLimit_->CheckValidity() ) &&
        mining_->CheckValidity() &&
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
        if( type->HasAltitudeModifierCapacity() )
            altitudeModifier_->CommitTo( *param );
        if( type->HasTimeLimitedCapacity() )
            timeLimit_->CommitTo( *param );
        if( type->CanBeValorized() )
            mining_->CommitTo( *param );
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
    tc2_->Purge();
    tc2_->RemoveFromController();
    tc2_->Hide();
    altitudeModifier_->RemoveFromController();
    altitudeModifier_->Hide();
    timeLimit_->RemoveFromController();
    timeLimit_->Hide();
    mining_->RemoveFromController();
    mining_->Hide();

    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    if( type->HasBuildableDensity() )
        density_->Show();
    if( type->HasLogistic() )
    {
        tc2_->RegisterIn( controller_ );
        tc2_->Show();
    }
    if( type->HasAltitudeModifierCapacity() )
    {
        altitudeModifier_->RegisterIn( controller_ );
        altitudeModifier_->Show();
    }
    if( type->HasTimeLimitedCapacity() )
    {
        timeLimit_->RegisterIn( controller_ );
        timeLimit_->Show();
    }

    if( type->CanBeValorized() )
    {
        mining_->RegisterIn( controller_ );
        mining_->Show();
    }
    location_->RemoveFromController();
    location_->SetShapeFilter( type->CanBePoint(), type->CanBeLine(), type->CanBePolygon(), type->CanBeCircle(), type->CanBeRectangle() );
    location_->RegisterIn( controller_ );
    emit ToggleReservable( type->CanBeReservedObstacle() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CreateMenu
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
kernel::ContextMenu::T_MenuVariant ParamObstacle::CreateMenu( kernel::ContextMenu& menu )
{
    if( IsInList() && parentList_->IsPotential( this ) )
        return internalMenu_;
    return ParamLocationComposite::CreateMenu( menu );
}
