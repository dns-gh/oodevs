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
#include "ParamObstacleType.h"
#include "ParamQuantity.h"
#include "ParamStringField.h"
#include "ParamTime.h"
#include "ParamFireClass.h"
#include "actions/Action_ABC.h"
#include "actions/Automat.h"
#include "actions/Bool.h"
#include "actions/EngineerConstruction.h"
#include "actions/FireClass.h"
#include "actions/Location.h"
#include "actions/ObstacleType.h"
#include "actions/Quantity.h"
#include "actions/String.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Units.h"
#include "ENT/ENT_Tr.h"
#include <tools/Resolver.h>

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
    , controller_ ( builder.GetControllers().controller_ )
    , typeCombo_  ( 0 )
{
    SetRecursive( true );

    location_ = static_cast< ParamLocation* >( AddElement( "location", tr( "Construction location" ).toStdString(), true ) );
    density_ = static_cast< ParamFloat* >( &builder.BuildOne( kernel::OrderParameter( tr( "Number of supplies per 100m/100m²" ).toStdString(), "float", false ), false ) );
    tc2_ = static_cast< ParamAutomat* >( AddElement( "automat", tr( "TC2" ).toStdString(), true ) );
    kernel::OrderParameter activatedParameter( tr( "Activation" ).toStdString(), "obstacletype", false );
    activatedCombo_ = static_cast< ParamObstacleType* >( &builder.BuildOne( activatedParameter, false ) );
    kernel::OrderParameter activityTimeParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ).toStdString(), "quantity", true );
    activityTime_ = static_cast< ParamQuantity* >( &builder.BuildOne( activityTimeParameter, false ) );
    kernel::OrderParameter activationTimeParameter( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ).toStdString(), "quantity", true );
    activationTime_ = static_cast< ParamQuantity* >( &builder.BuildOne( activationTimeParameter, false ) );
    activityTime_->SetSuffix( kernel::Units::seconds.AsString() );
    activationTime_->SetSuffix( kernel::Units::seconds.AsString() );

    name_                = static_cast< ParamStringField* >( &builder.BuildOne( kernel::OrderParameter( tr( "Name" ).toStdString(), "string", true ), false ) );
    timeLimit_           = static_cast< ParamTime* >     ( AddElement( "time",       tr( "Time limit" ).toStdString(), true ) );
    mining_              = static_cast< ParamBool* >     ( AddElement( "boolean",    tr( "Obstacle mining" ).toStdString(), true ) );
    altitudeModifier_    = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Altitude modifier" ).toStdString(), true ) );
    lodging_             = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Lodging" ).toStdString(), true ) );
    fireClass_           = static_cast< ParamFireClass* >( AddElement( "fireClass",  tr( "Fire class:" ).toStdString(), true ) );
    maxCombustionEnergy_ = static_cast< ParamQuantity* > ( AddElement( "quantity",   tr( "Max combustion energy" ).toStdString(), false ) );

    tc2_                ->SetKeyName( "tc2" );
    location_           ->SetKeyName( "location" );
    name_               ->SetKeyName( "name" );
    activatedCombo_     ->SetKeyName( "obstacletype" );
    activityTime_       ->SetKeyName( "activitytime" );
    activationTime_     ->SetKeyName( "activationtime" );
    density_            ->SetKeyName( "density" );
    altitudeModifier_   ->SetKeyName( "altitude_modifier" );
    timeLimit_          ->SetKeyName( "time_limit" );
    mining_             ->SetKeyName( "obstacle_mining" );
    lodging_            ->SetKeyName( "lodging" );
    fireClass_          ->SetKeyName( "fire_class" );
    maxCombustionEnergy_->SetKeyName( "max_combustion_energy" );

    altitudeModifier_->SetLimit( 0, std::numeric_limits< int >::max() );
    altitudeModifier_->SetSuffix( kernel::Units::meters.AsString() );
    lodging_->SetLimit( 0, std::numeric_limits< int >::max() );

    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    RemoveFromController();
    controller_.Unregister( *this );
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
void ParamObstacle::RegisterIn()
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
    density_->RemoveFromController();
    tc2_->RemoveFromController();
    activatedCombo_->RemoveFromController();
    activityTime_->RemoveFromController();
    activationTime_->RemoveFromController();
    name_->RemoveFromController();
    altitudeModifier_->RemoveFromController();
    timeLimit_->RemoveFromController();
    mining_->RemoveFromController();
    lodging_->RemoveFromController();
    fireClass_->RemoveFromController();
    maxCombustionEnergy_->RemoveFromController();
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
            if( parameter_.HasObject( type.GetType() ) )
                typeCombo_->AddItem( type.GetName(), &type );
        }
        layout->addWidget( new QLabel( tr( "Type:" ), parent ) );
        layout->addWidget( typeCombo_ );
    }
    // Density
    {
        QWidget* densityBox = density_->BuildInterface( "densityBox", parent );
        density_->SetLimits( 0.f, std::numeric_limits< float >::max() );
        densityBox->layout()->setMargin( 0 );
        densityBox->layout()->setSpacing( 0 );
        layout->addWidget( densityBox );
    }
    // TC2
    {
        QWidget* tc2Box = tc2_->BuildInterface( "tc2Box", parent );
        tc2Box->layout()->setMargin( 0 );
        tc2Box->layout()->setSpacing( 0 );
        layout->addWidget( tc2Box );
    }
    // Location
    {
        QWidget* locationBox = location_->BuildInterface( "locationBox", parent );
        locationBox->layout()->setMargin( 0 );
        locationBox->layout()->setSpacing( 0 );
        layout->addWidget( locationBox );
    }
    // Name
    {
        QWidget* nameBox = name_->BuildInterface( "nameBox", parent );
        nameBox->layout()->setMargin( 0 );
        nameBox->layout()->setSpacing( 0 );
        layout->addWidget( nameBox );
    }
    // Activity/Activation Times
    {
        QWidget* activatedBox = activatedCombo_->BuildInterface( "activatedBox", parent );
        activatedBox->layout()->setMargin( 0 );
        activatedBox->layout()->setSpacing( 0 );
        layout->addWidget( activatedBox );
        QWidget* activityTimeBox = activityTime_->BuildInterface( "activityTimeBox", parent );
        activityTimeBox->layout()->setMargin( 0 );
        activityTimeBox->layout()->setSpacing( 0 );
        layout->addWidget( activityTimeBox );
        QWidget* activationTimeBox = activationTime_->BuildInterface( "activationTimeBox", parent );
        activationTimeBox->layout()->setMargin( 0 );
        activationTimeBox->layout()->setSpacing( 0 );
        layout->addWidget( activationTimeBox );
    }
    // Altitude modifier
    {
        QWidget* altitudeBox = altitudeModifier_->BuildInterface( "altitudeBox", parent );
        altitudeBox->layout()->setMargin( 0 );
        altitudeBox->layout()->setSpacing( 0 );
        layout->addWidget( altitudeBox );
    }
    // Time limit
    {
        QWidget* timeLimitBox = timeLimit_->BuildInterface( "timeLimit", parent );
        timeLimitBox->layout()->setMargin( 0 );
        timeLimitBox->layout()->setSpacing( 0 );
        layout->addWidget( timeLimitBox );
    }
    // Mining
    {
        QWidget* miningBox = mining_->BuildInterface( "miningBox", parent );
        miningBox->layout()->setMargin( 0 );
        miningBox->layout()->setSpacing( 0 );
        layout->addWidget( miningBox );
    }
    // Lodging
    {
        QWidget* lodgingBox = lodging_->BuildInterface( "lodgingBox", parent );
        lodgingBox->layout()->setMargin( 0 );
        lodgingBox->layout()->setSpacing( 0 );
        layout->addWidget( lodgingBox );
    }
    // Fire class
    {
        QWidget* fireBox = fireClass_->BuildInterface( "FireBox", parent );
        fireBox->layout()->setMargin( 0 );
        fireBox->layout()->setSpacing( 0 );
        layout->addWidget( fireBox );
        QWidget* maxCombustionBox = maxCombustionEnergy_->BuildInterface( "maxCombustionBox", parent );
        maxCombustionBox->layout()->setMargin( 0 );
        maxCombustionBox->layout()->setSpacing( 0 );
        layout->addWidget( maxCombustionBox );
    }
    connect( typeCombo_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    OnTypeChanged( false );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: ABR 2014-18-02
// -----------------------------------------------------------------------------
bool ParamObstacle::IsChecked() const
{
    return Param_ABC::IsChecked();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
bool ParamObstacle::CheckValidity() const
{
    return Param_ABC::CheckValidity();
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
        ( !typeCombo_->GetValue()->HasBurnCapacity() || fireClass_->CheckValidity() ) &&
        ( !typeCombo_->GetValue()->HasBurnCapacity() || maxCombustionEnergy_->CheckValidity() ) &&
        mining_->CheckValidity() &&
        location_->CheckValidity();
}

namespace
{
    void CommitAndSetKeyName( const Param_ABC& guiParam, actions::Parameter_ABC& param )
    {
        guiParam.CommitTo( param );
        if( auto newParam = param.Find( param.Count() - 1 ) )
            newParam->SetKeyName( guiParam.GetKeyName() );
        else
            throw MASA_EXCEPTION( "Impossible to retrieve the last inserted parameter in ParamObstacle" );
    }
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
    if( Param_ABC::IsChecked() )
    {
        std::unique_ptr< actions::parameters::EngineerConstruction > param( new actions::parameters::EngineerConstruction( parameter_, *type ) );
        if( type->HasBuildableDensity() )
            CommitAndSetKeyName( *density_, *param );
        if( type->HasLogistic() )
            CommitAndSetKeyName( *tc2_, *param );
        if( type->CanBeActivated() )
        {
            CommitAndSetKeyName( *activatedCombo_, *param );
            CommitAndSetKeyName( *activityTime_, *param );
            CommitAndSetKeyName( *activationTime_, *param );
        }
        CommitAndSetKeyName( *name_, *param );
        name_->CommitTo( *param );
        if( type->HasAltitudeModifierCapacity() )
            CommitAndSetKeyName( *altitudeModifier_, *param );
        if( type->HasTimeLimitedCapacity() )
            CommitAndSetKeyName( *timeLimit_, *param );
        if( type->HasLodgingCapacity() )
            CommitAndSetKeyName( *lodging_, *param );
        if( type->CanBeValorized() )
            CommitAndSetKeyName( *mining_, *param );
        if( type->HasBurnCapacity() )
        {
            CommitAndSetKeyName( *fireClass_, *param );
            CommitAndSetKeyName( *maxCombustionEnergy_, *param );
        }
        CommitAndSetKeyName( *location_, *param );
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

namespace
{
    template< typename T >
    void HideAndRemoveFromController( T& param )
    {
        param.Purge();
        param.SetVisible( false );
        param.RemoveFromController();
    }

    template< typename T >
    void ShowAndAddToControllerIfNeeded( T& param, const kernel::ObjectType& type, bool (kernel::ObjectType::* has_field)() const )
    {
        if( (type.*has_field)() )
        {
            param.RegisterIn();
            param.SetVisible( true );
        }
    }

}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::OnTypeChanged
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamObstacle::OnTypeChanged( bool update /* = true */ )
{
    HideAndRemoveFromController( *location_ );
    HideAndRemoveFromController( *density_ );
    HideAndRemoveFromController( *tc2_ );
    HideAndRemoveFromController( *activatedCombo_ );
    HideAndRemoveFromController( *activityTime_ );
    HideAndRemoveFromController( *activationTime_ );
    HideAndRemoveFromController( *altitudeModifier_ );
    HideAndRemoveFromController( *timeLimit_ );
    HideAndRemoveFromController( *mining_ );
    HideAndRemoveFromController( *lodging_ );
    HideAndRemoveFromController( *fireClass_ );
    HideAndRemoveFromController( *maxCombustionEnergy_ );

    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;

    ShowAndAddToControllerIfNeeded( *density_,             *type, &kernel::ObjectType::HasBuildableDensity );
    ShowAndAddToControllerIfNeeded( *tc2_,                 *type, &kernel::ObjectType::HasLogistic );
    ShowAndAddToControllerIfNeeded( *activatedCombo_,      *type, &kernel::ObjectType::CanBeActivated );
    ShowAndAddToControllerIfNeeded( *activityTime_,        *type, &kernel::ObjectType::CanBeActivated );
    ShowAndAddToControllerIfNeeded( *activationTime_,      *type, &kernel::ObjectType::CanBeActivated );
    ShowAndAddToControllerIfNeeded( *altitudeModifier_,    *type, &kernel::ObjectType::HasAltitudeModifierCapacity );
    ShowAndAddToControllerIfNeeded( *timeLimit_,           *type, &kernel::ObjectType::HasTimeLimitedCapacity );
    ShowAndAddToControllerIfNeeded( *mining_,              *type, &kernel::ObjectType::CanBeValorized );
    ShowAndAddToControllerIfNeeded( *lodging_,             *type, &kernel::ObjectType::HasLodgingCapacity );
    ShowAndAddToControllerIfNeeded( *fireClass_,           *type, &kernel::ObjectType::HasBurnCapacity );
    ShowAndAddToControllerIfNeeded( *maxCombustionEnergy_, *type, &kernel::ObjectType::HasBurnCapacity );

    location_->SetShapeFilter( type->CanBePoint(), type->CanBeLine(), type->CanBePolygon(), type->CanBeCircle(), type->CanBeRectangle() );
    location_->RegisterIn();
    location_->SetVisible( true );
    if( update )
        Update();
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

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::EngineerConstruction& param )
{
    ActivateOptionalIfNeeded( param );
    assert( typeCombo_ != 0 );
    if( const kernel::ObjectType* type = objectTypes_.tools::StringResolver< kernel::ObjectType >::Find( param.GetValue() ) )
    {
        typeCombo_->SetCurrentItem( type );
        OnTypeChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::Automat& param )
{
    assert( tc2_ != 0 );
    param.Accept( *tc2_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::Bool& param )
{
    assert( mining_ != 0 );
    param.Accept( *mining_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2014-03-27
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::FireClass& param )
{
    assert( fireClass_ != 0 );
    param.Accept( *fireClass_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::Location& param )
{
    assert( location_ != 0 );
    param.Accept( *location_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::Numeric& param )
{
    assert( density_ != 0 );
    if( param.GetKeyName() == density_->GetKeyName() )
        param.Accept( *density_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::ObstacleType& param )
{
    assert( activatedCombo_ != 0 );
    activatedCombo_->SetValue( param.GetValue() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::Quantity& param )
{
    assert( altitudeModifier_ != 0 && lodging_ != 0 && timeLimit_ != 0 &&
            activatedCombo_ != 0 && activityTime_ != 0 && activationTime_ != 0 );
    if( param.GetKeyName() == altitudeModifier_->GetKeyName() )
        param.Accept( *altitudeModifier_ );
    else if( param.GetKeyName() == lodging_->GetKeyName() )
        param.Accept( *lodging_ );
    else if( param.GetKeyName() == timeLimit_->GetKeyName() )
        param.Accept( *timeLimit_ );
    else if( param.GetKeyName() == activityTime_->GetKeyName() )
        param.Accept( *activityTime_ );
    else if( param.GetKeyName() == activatedCombo_->GetKeyName() )
        param.Accept( *activatedCombo_ );
    else if( param.GetKeyName() == activationTime_->GetKeyName() )
        param.Accept( *activationTime_ );
    else if( param.GetKeyName() == maxCombustionEnergy_->GetKeyName() )
        param.Accept( *maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Visit
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void ParamObstacle::Visit( const actions::parameters::String& param )
{
    assert( name_ != 0 );
    param.Accept( *name_ );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::HasParameter
// Created: ABR 2013-11-14
// -----------------------------------------------------------------------------
bool ParamObstacle::HasParameter( const Param_ABC& param ) const
{
    return ParamLocationComposite::HasParameter( param ) ||
        location_->HasParameter( param ) ||
        density_->HasParameter( param ) ||
        tc2_->HasParameter( param ) ||
        activatedCombo_->HasParameter( param ) ||
        activityTime_->HasParameter( param ) ||
        activationTime_->HasParameter( param ) ||
        name_->HasParameter( param ) ||
        altitudeModifier_->HasParameter( param ) ||
        timeLimit_->HasParameter( param ) ||
        mining_->HasParameter( param ) ||
        lodging_->HasParameter( param ) ||
        fireClass_->HasParameter( param ) ||
        maxCombustionEnergy_->HasParameter( param );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::NotifyUpdated
// Created: ABR 2013-11-14
// -----------------------------------------------------------------------------
void ParamObstacle::NotifyUpdated( const Param_ABC& param )
{
    if( location_ && location_->HasParameter( param ) &&
        IsOptional() && group_ && !group_->isChecked() )
        group_->setChecked( true );
}
