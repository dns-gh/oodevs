// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAtlasNature.h"
#include "moc_ParamAtlasNature.cpp"
#include "game_asn/Asn.h"
#include "clients_gui/Tools.h"
#include "gaming/ActionParameter.h"

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::ParamAtlasNature( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
    , bits_( 0 )
{
    AddField( tools::translate( "AtlasNature", "Armored" )        , BytXblinde          , BitMblinde );
    AddField( tools::translate( "AtlasNature", "SSA" )            , BytXass             , BitMass );
    AddField( tools::translate( "AtlasNature", "Command post" )   , BytXpc              , BitMpc );
    AddField( tools::translate( "AtlasNature", "Logistic" )       , BytXlog             , BitMlog );
    AddField( tools::translate( "AtlasNature", "Mortar" )         , BytXmortier         , BitMmortier );
    AddField( tools::translate( "AtlasNature", "Rocket launcher" ), BytXlance_roquette  , BitMlance_roquette );
    AddField( tools::translate( "AtlasNature", "Vehicle" )        , BytXvehicule        , BitMvehicule );
    AddField( tools::translate( "AtlasNature", "Footman")         , BytXpersonnel_a_pied, BitMpersonnel_a_pied );
    AddField( tools::translate( "AtlasNature", "Helicopter" )     , BytXhelicoptere     , BitMhelicoptere );
    AddField( tools::translate( "AtlasNature", "Undefined" )      , BytXindefini        , BitMindefini );
    AddField( tools::translate( "AtlasNature", "None" )           , BytXnone            , BitMnone );
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
// Name: ParamAtlasNature::AddField
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamAtlasNature::AddField( const QString& name, unsigned char byte, unsigned char mask )
{
    fields_.push_back( std::make_pair( name, byte ? mask : ( mask << 8 ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamAtlasNature::BuildInterface( QWidget* parent )
{
    QButtonGroup* group = new QButtonGroup( 2, Qt::Horizontal, GetName(), parent );
    for( T_AtlasFields::const_iterator it = fields_.begin(); it != fields_.end(); ++it )
        new QCheckBox( it->first, group );
    connect( group, SIGNAL( clicked( int ) ), SLOT( OnClicked( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAtlasNature::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_natureAtlas;
    asn.value.u.natureAtlas = new ASN1T_NatureAtlas( 11, (unsigned char*)&bits_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAtlasNature::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.natureAtlas;
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamAtlasNature::CommitTo( Action_ABC& action ) const
{
//    std::auto_ptr< ActionParameter< E_NatureAtlasType > > param( new ActionParameter< E_NatureAtlasType >( GetName() ) );
//    param->SetValue( (E_NatureAtlasType)0 ); // $$$$ SBO 2007-03-19: std::vector< E_NatureAtlasType > or create a specific class
//    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::OnClicked
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamAtlasNature::OnClicked( int id )
{
    bits_ ^= fields_.at( id ).second;
}
