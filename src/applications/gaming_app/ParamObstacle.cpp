// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObstacle.h"
#include "moc_ParamObstacle.cpp"
#include "ParamLocation.h"
#include "ParamNumericField.h"
#include "ParamAutomat.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Automat_ABC.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter.h"
#include "gaming/ActionParameterObstacle.h"
#include "gaming/ActionParameterObstacleType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( QObject* parent, const kernel::OrderParameter& parameter, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QObject     ( parent )
    , Param_ABC   ( parameter.GetName() )
    , parameter_  ( parameter )
    , objectTypes_( objectTypes )
    , layer_      ( layer )
    , converter_  ( converter )
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
void ParamObstacle::BuildInterface( QWidget* parent )
{
    QGroupBox* group = new QGroupBox( 1, Qt::Horizontal, GetName(), parent );
    QHBox* box = new QHBox( group );
    box->setSpacing( 5 );
    new QLabel( tr( "Type:" ), box );
    typeCombo_ = new ValuedComboBox< const ObjectType* >( box );
    typeCombo_->setSorting( true );
    Iterator< const ObjectType& > it = objectTypes_.Resolver2< ObjectType, unsigned long >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const ObjectType& type = it.NextElement();
        typeCombo_->AddItem( type.GetName(), &type );
    }

    box = new QHBox( group );
    box->setSpacing( 5 );

    {
        QLabel* label = new QLabel( tr( "Obstacle type:" ), box );
        obstacleTypeCombo_ = new ValuedComboBox< unsigned int >( box );
        for( unsigned int i = 0; i < eNbrTypeObstacle ; ++i )
            obstacleTypeCombo_->AddItem( tools::ToString( ( E_TypeObstacle)i ), i );
        connect( this, SIGNAL( ToggleReservable( bool ) ), label, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleReservable( bool ) ), obstacleTypeCombo_, SLOT( setShown( bool ) ) );
    }

    density_ = new ParamNumericField( OrderParameter( tr( "Density" ), "density", false ), true );
    density_->BuildInterface( group );
    density_->SetLimits( 0.f, 5.f );

    tc2_ = new ParamAutomat( this, OrderParameter( tr( "TC2" ), "tc2", false ) );
    tc2_->BuildInterface( group );

    location_ = new ParamLocation( OrderParameter( tr( "Location" ), "location", false ), layer_, converter_ );
    location_->BuildInterface( group );

    connect( typeCombo_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    OnTypeChanged();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RemoveFromController
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RemoveFromController()
{
    tc2_->RemoveFromController();
    location_->RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::RegisterIn
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::RegisterIn( ActionController& controller )
{
    tc2_->RegisterIn( controller );
    location_->RegisterIn( controller );
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
    switch( typeCombo_->GetValue()->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        bOk = density_->CheckValidity();
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        bOk = tc2_->CheckValidity();
        break;
    };
    bOk = bOk && location_->CheckValidity();
    return bOk;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( ! typeCombo_ )
        InterfaceNotInitialized();
    ASN1T_PlannedWork*& object = asn.value.u.plannedWork = new ASN1T_PlannedWork();
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_plannedWork;
    CommitTo( *object );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ASN1T_PlannedWork& object ) const
{
    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    object.type          = (ASN1T_EnumObjectType)type->id_;
    object.type_obstacle = (ASN1T_EnumObstacleType)obstacleTypeCombo_->GetValue();
    switch( type->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->CommitTo( object.densite );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->CommitTo( object.tc2 );
        break;
    };
    location_->CommitTo( object.position );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacle::Clean( ASN1T_MissionParameter& asn ) const
{
    location_->Clean( asn.value.u.plannedWork->position );
    delete asn.value.u.plannedWork;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( Action_ABC& action ) const
{
    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    std::auto_ptr< ActionParameterObstacle > param( new ActionParameterObstacle( parameter_, *type ) );
    switch( type->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->CommitTo( *param );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->CommitTo( *param );
        break;
    };
    if( type->CanBeReservedObstacle() )
        param->AddParameter( *new ActionParameterObstacleType( OrderParameter( tr( "Obstacle type" ), "obstacletype", false ), obstacleTypeCombo_->GetValue() ) );
    location_->CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ActionParameter_ABC& parameter ) const
{
    const kernel::ObjectType* type = typeCombo_->GetValue();
    if( !type ) // $$$$ SBO 2007-04-26: 
        return;
    std::auto_ptr< ActionParameterObstacle > param( new ActionParameterObstacle( parameter_, *type ) );
    location_->CommitTo( *param );
    switch( type->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->CommitTo( *param );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->CommitTo( *param );
        break;
    };
    if( type->CanBeReservedObstacle() )
        param->AddParameter( *new ActionParameterObstacleType( OrderParameter( tr( "Obstacle type" ), "obstacletype", false ), obstacleTypeCombo_->GetValue() ) );
    parameter.AddParameter( *param.release() );
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
    const ObjectType* type = typeCombo_->GetValue();
    if( !type )
        return;
    switch( type->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->Show();
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->Show();
        break;
    };
    emit ToggleReservable( type->CanBeReservedObstacle() );
}
