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
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Automat_ABC.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( QObject* parent, const QString& name, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QObject( parent )
    , Param_ABC( name )
    , objectTypes_( objectTypes )
    , layer_( layer )
    , converter_( converter )
    , location_( 0 )
    , typeCombo_( 0 )
    , density_( 0 )
    , tc2_( 0 )

{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    delete density_;
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamObstacle::BuildInterface( QWidget* parent )
{
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, GetName(), parent );
    new QLabel( tr( "Type:" ), box );
    typeCombo_ = new ValuedComboBox< const ObjectType* >( box );
    typeCombo_->setSorting( true );
    Iterator< const ObjectType& > it = objectTypes_.Resolver2< ObjectType, unsigned long >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const ObjectType& type = it.NextElement();
        typeCombo_->AddItem( type.GetName(), &type );
    }

    new QLabel( tr( "Sub-type:" ), box );
    preliminaryCombo_ = new QComboBox( box );
    for( int i = 0; i < eNbrMissionGenSousTypeObstacle; ++i )
        preliminaryCombo_->insertItem( ENT_Tr::ConvertFromMissionGenSousTypeObstacle( ( E_MissionGenSousTypeObstacle )i ).c_str(), i );

    density_ = new ParamNumericField( tr( "Density" ), true );
    density_->BuildInterface( box );
    density_->SetLimits( 0.f, 5.f );
    tc2_     = new EntityParameter< kernel::Automat_ABC >( this, tr( "TC2" ) );
    tc2_->BuildInterface( box );
    location_ = new ParamLocation( tr( "Location" ), layer_, converter_ );
    location_->BuildInterface( box );

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
    ASN1T_MissionGenObject*& object = asn.value.u.missionGenObject;
    object = 0;
    if( ! typeCombo_ )
        InterfaceNotInitialized();
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_missionGenObject;
    object = new ASN1T_MissionGenObject();
    CommitTo( *object );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ASN1T_MissionGenObject& object ) const
{
    object.type          = (ASN1T_EnumObjectType)typeCombo_->GetValue()->id_;
    object.preliminaire  = (ASN1T_EnumMissionGenSousTypeObstacle)preliminaryCombo_->currentItem();
    switch( typeCombo_->GetValue()->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->CommitTo( object.densite );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        if( tc2_->CheckValidity() ) // $$$$ SBO 2006-11-08: ParamObstacleList commits before to CheckValidity => throw
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
    delete asn.value.u.missionGenObject;
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
// Name: ParamObstacle::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamObstacle::Show()
{
    typeCombo_->parentWidget()->show();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamObstacle::Hide()
{
    typeCombo_->parentWidget()->hide();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::OnTypeChanged
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamObstacle::OnTypeChanged()
{
    density_->Hide();
    tc2_->Hide();
    switch( typeCombo_->GetValue()->id_ )
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
}
