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
#include "InterfaceBuilder_ABC.h"
#include "ListParameter.h"
#include "ParamAutomat.h"
#include "ParamBool.h"
#include "ParamDateTime.h"
#include "ParamInterface_ABC.h"
#include "ParamLocation.h"
#include "ParamNumericField.h"
#include "ParamQuantity.h"
#include "ParamStringField.h"
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
    location_ = static_cast< ParamLocation* >( AddElement( "location", tr( "Construction location" ).toStdString() ) );
    density_ = static_cast< ParamFloat* >( &builder.BuildOne( kernel::OrderParameter( tr( "Density per 100 square meter" ).toStdString(), "float", true ), false ) );
    tc2_ = static_cast< ParamAutomat* >( AddElement( "automat", tr( "TC2" ).toStdString() ) );
    kernel::OrderParameter activityTimeParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toStdString(), "integer", true );
    activityTimeParameter.SetKeyName( "ActivityTime" );
    activityTime_ = static_cast< ParamNumericField< int >* >( &builder.BuildOne( activityTimeParameter, false ) );
    kernel::OrderParameter activationTimeParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toStdString(), "integer", true );
    activationTimeParameter.SetKeyName( "ActivationTime" );
    activationTime_ = static_cast< ParamNumericField< int >* >( &builder.BuildOne( activationTimeParameter, false ) );
    activityTime_->SetSuffix( kernel::Units::seconds.AsString() );
    activationTime_->SetSuffix( kernel::Units::seconds.AsString() );

    name_ = static_cast< ParamStringField* >( &builder.BuildOne( kernel::OrderParameter( tr( "Name" ).toStdString(), "string", true ), false ) );
    timeLimit_          = static_cast< ParamTime* >     ( AddElement( "time",       tr( "Time limit" ).toStdString() ) );
    mining_             = static_cast< ParamBool* >     ( AddElement( "boolean",    tr( "Obstacle mining" ).toStdString() ) );
    altitudeModifier_   = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Altitude modifier" ).toStdString() ) );
    lodging_            = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Lodging" ).toStdString() ) );

    density_            ->SetKeyName( "density" );
    altitudeModifier_   ->SetKeyName( "altitude_modifier" );
    timeLimit_          ->SetKeyName( "time_limit" );
    mining_             ->SetKeyName( "obstacle_mining" );
    lodging_            ->SetKeyName( "lodging" );

    altitudeModifier_->SetLimit( 0, std::numeric_limits< int >::max() );
    altitudeModifier_->SetSuffix( kernel::Units::meters.AsString() );
    lodging_->SetLimit( 0, std::numeric_limits< int >::max() );
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
    lodging_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamObstacle::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    // Type
    {
        typeCombo_ = new ::gui::ValuedComboBox< const kernel::ObjectType* >( "typeCombo", parent );
        typeCombo_->SetSorting( true );
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
        obstacleTypeCombo_ = new ::gui::ValuedComboBox< unsigned int >( "obstacleTypeCombo", parent );
        for( unsigned int i = 0; i < eNbrDemolitionTargetType ; ++i )
            obstacleTypeCombo_->AddItem( tools::ToString( ( E_DemolitionTargetType)i ), i );
        connect( this, SIGNAL( ToggleReservable( bool ) ), label, SLOT( setVisible( bool ) ) );
        connect( this, SIGNAL( ToggleReservable( bool ) ), obstacleTypeCombo_, SLOT( setVisible( bool ) ) );
        layout->addWidget( label );
        layout->addWidget( obstacleTypeCombo_ );
    }

    // Density
    {
        QGroupBox* densityBox = static_cast< QGroupBox* >( density_->BuildInterface( "densityBox", parent ) );
        density_->SetLimits( 0.f, 5.f );
        densityBox->layout()->setMargin( 0 );
        densityBox->layout()->setSpacing( 0 );
        layout->addWidget( densityBox );
    }

    // TC2
    {
        QGroupBox* tc2Box = static_cast< QGroupBox* >( tc2_->BuildInterface( "tc2Box", parent ) );
        tc2Box->layout()->setMargin( 0 );
        tc2Box->layout()->setSpacing( 0 );
        layout->addWidget( tc2Box );
    }

    // Location
    {
        QGroupBox* locationBox = static_cast< QGroupBox* >( location_->BuildInterface( "locationBox", parent ) );
        locationBox->layout()->setMargin( 0 );
        locationBox->layout()->setSpacing( 0 );
        layout->addWidget( locationBox );
    }
    // Name
    {
        QGroupBox* nameBox = static_cast< QGroupBox* >( name_->BuildInterface( "nameBox", parent ) );
        nameBox->layout()->setMargin( 0 );
        nameBox->layout()->setSpacing( 0 );
        layout->addWidget( nameBox );
    }
    // Activity/Activation Times
    {
        QGroupBox* activityTimeBox = static_cast< QGroupBox* >( activityTime_->BuildInterface( "activityTimeBox", parent ) );
        activityTimeBox->layout()->setMargin( 0 );
        activityTimeBox->layout()->setSpacing( 0 );
        layout->addWidget( activityTimeBox );
        QGroupBox* activationTimeBox = static_cast< QGroupBox* >( activationTime_->BuildInterface( "activationTimeBox", parent ) );
        activationTimeBox->layout()->setMargin( 0 );
        activationTimeBox->layout()->setSpacing( 0 );
        layout->addWidget( activationTimeBox );
    }

    // Altitude modifier
    {
        QGroupBox* altitudeBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( altitudeModifier_ )->BuildInterface( "altitudeBox", parent ) );
        altitudeBox->layout()->setMargin( 0 );
        altitudeBox->layout()->setSpacing( 0 );
        layout->addWidget( altitudeBox );
    }

    // Time limit
    {
        QGroupBox* timeLimitBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( timeLimit_ )->BuildInterface( "timeLimit", parent ) );
        timeLimitBox->layout()->setMargin( 0 );
        timeLimitBox->layout()->setSpacing( 0 );
        layout->addWidget( timeLimitBox );
    }

    // Mining
    {
        QGroupBox* miningBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( mining_ )->BuildInterface( "miningBox", parent ) );
        miningBox->layout()->setMargin( 0 );
        miningBox->layout()->setSpacing( 0 );
        layout->addWidget( miningBox );
    }

    // Lodging
    {
        QGroupBox* lodgingBox = static_cast< QGroupBox* >( static_cast< Param_ABC* >( lodging_ )->BuildInterface( "lodgingBox", parent ) );
        lodgingBox->layout()->setMargin( 0 );
        lodgingBox->layout()->setSpacing( 0 );
        layout->addWidget( lodgingBox );
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
        {
            param->AddParameter( *new actions::parameters::ObstacleType( kernel::OrderParameter( tr( "Obstacle type" ).toStdString(), "obstacletype", false ), obstacleTypeCombo_->GetValue() ) );
            activityTime_->CommitTo( *param );
            activationTime_->CommitTo( *param );
        }
        name_->CommitTo( *param );
        if( type->HasAltitudeModifierCapacity() )
            altitudeModifier_->CommitTo( *param );
        if( type->HasTimeLimitedCapacity() )
            timeLimit_->CommitTo( *param );
        if( type->HasLodgingCapacity() )
            lodging_->CommitTo( *param );
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
void ParamObstacle::Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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
    activityTime_->Hide();
    activationTime_->Hide();
    altitudeModifier_->RemoveFromController();
    altitudeModifier_->Hide();
    timeLimit_->RemoveFromController();
    timeLimit_->Hide();
    mining_->RemoveFromController();
    mining_->Hide();
    lodging_->RemoveFromController();
    lodging_->Hide();

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
    if( type->CanBeReservedObstacle() )
    {
        activityTime_->Show();
        activationTime_->Show();
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
    if( type->HasLodgingCapacity() )
    {
        lodging_->RegisterIn( controller_ );
        lodging_->Show();
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
