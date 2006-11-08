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
ParamObstacle::ParamObstacle( QWidget* parent, ASN1T_MissionGenObject& asnObject, const QString& label, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : QGroupBox( 0, Qt::Horizontal, label, parent )
    , asnObject_( asnObject )
{
    QGridLayout* grid = new QGridLayout( 5, 2, 2 );
    layout()->addItem( grid );
    grid->addWidget( new QLabel( tr( "Type:" ), this ), 0, 0 );
    typeCombo_ = new ValuedComboBox< const ObjectType* >( this );
    typeCombo_->setSorting( true );
    Iterator< const ObjectType& > it = objectTypes.Resolver2< ObjectType, unsigned long >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const ObjectType& type = it.NextElement();
        typeCombo_->AddItem( type.GetName(), &type );
    }
    grid->addWidget( typeCombo_, 0, 1 );

    grid->addWidget( new QLabel( tr( "Sous type:" ), this ), 1, 0 );
    preliminaryCombo_ = new QComboBox( this );
    for( int i = 0; i < eNbrMissionGenSousTypeObstacle; ++i )
        preliminaryCombo_->insertItem( ENT_Tr::ConvertFromMissionGenSousTypeObstacle( ( E_MissionGenSousTypeObstacle )i ).c_str(), i );
    grid->addWidget( preliminaryCombo_, 1, 1 );

    density_ = new ParamNumericField( this, asnObject.densite, tr( "Densité" ), 0., 5. );
    grid->addMultiCellWidget( density_, 2, 2, 0, 1 );
    tc2_     = new EntityParameter< kernel::Automat_ABC >( this, asnObject.tc2, tr( "TC2" ), tr( "TC2" ) );
    grid->addMultiCellWidget( tc2_, 3, 3, 0, 1 );
    location_ = new ParamLocation( this, asnObject.position, tr( "Localisation" ), layer, converter );
    grid->addMultiCellWidget( location_, 4, 4, 0, 1 );

    connect( typeCombo_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );
    OnTypeChanged();
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
    // NOTHING
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
// Name: ParamObstacle::Commit
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::Commit()
{
    asnObject_.type          = (ASN1T_EnumObjectType)typeCombo_->GetValue()->id_;
    asnObject_.preliminaire  = (ASN1T_EnumMissionGenSousTypeObstacle)preliminaryCombo_->currentItem();
    switch( typeCombo_->GetValue()->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->Commit();
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        if( tc2_->CheckValidity() ) // $$$$ SBO 2006-11-08: ParamObstacleList commits before to CheckValidity => throw
            tc2_->Commit();
        break;
    };
    location_->Commit();
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacle::CommitTo( ASN1T_MissionGenObject& destination )
{
    destination.preliminaire = asnObject_.preliminaire;
    destination.type         = asnObject_.type;
    destination.densite      = 0;
    destination.tc2          = 0;
    switch( destination.type )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->CommitTo( destination.densite );
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->CommitTo( destination.tc2 );
        break;
    };
    location_->CommitTo( destination.position );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::Draw
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ParamObstacle::Draw( const geometry::Point2f& point, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
    location_->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacle::OnTypeChanged
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamObstacle::OnTypeChanged()
{
    density_->hide();
    tc2_->hide();
    switch( typeCombo_->GetValue()->id_ )
    {
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        density_->show();
        break;
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        tc2_->show();
        break;
    };
}
