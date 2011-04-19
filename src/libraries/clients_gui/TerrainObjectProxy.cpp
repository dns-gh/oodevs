// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainObjectProxy.h"
#include "Tools.h"
#include "UrbanDisplayOptions.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "protocol/Simulation.h"
#include <urban/TerrainObject_ABC.h>
#include <boost/foreach.hpp>

using namespace gui;
using namespace kernel;
using namespace urban;

const QString TerrainObjectProxy::typeName_ = "terrainObjectProxy";

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( Controllers& controllers, TerrainObject_ABC& object, const std::string& name, unsigned int id
                                      , const ObjectType& type, UrbanDisplayOptions& options )
    : EntityImplementation< Object_ABC >( controllers.controller_, id, name.c_str() )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_     ( object )
    , controllers_( controllers )
    , name_       ( name )
    , id_         ( id )
    , type_       ( type )
    , options_    ( options )
{
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::~TerrainObjectProxy()
{
    Get< kernel::UrbanColor_ABC >().Restore();
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Update
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void TerrainObjectProxy::DoUpdate( const sword::UrbanUpdate& /*msg*/ )
{
    Touch();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy GetName
// Created: SLG 2009-11-2
// -----------------------------------------------------------------------------
QString TerrainObjectProxy::GetName() const
{
    if( name_.empty() )
        return QString( tools::translate( "Urban", "Urban block[%1]" ).arg( id_ ) );
    return name_.c_str();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetTypeName
// Created: ABR 2011-02-18
// -----------------------------------------------------------------------------
QString TerrainObjectProxy::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Select
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Select( ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void TerrainObjectProxy::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    EntityImplementation< Object_ABC >::Attach( dictionary );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Identifier" ), EntityImplementation< Object_ABC >::id_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Name" ), EntityImplementation< Object_ABC >::name_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::DisplayInSummary
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Block", "Density:" ), GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateHumans( const std::string& inhabitant, const T_BlockOccupation& occupations, bool alerted, bool confined, bool evacuated, float angriness  )
{
    T_Human& mutableHuman = humans_[ inhabitant ];
    for( CIT_BlockOccupation it = occupations.begin(); it != occupations.end(); ++it )
        mutableHuman.persons_[ it->first ] = it->second;
    mutableHuman.alerted_ = alerted;
    mutableHuman.confined_ = confined;
    mutableHuman.evacuated_ = evacuated;
    mutableHuman.angriness_ = angriness;
    const T_Human& human = mutableHuman;
    PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
    const Entity_ABC& constEntity = *static_cast< const Entity_ABC* >( this );
    const QString keyBase = tools::translate( "Block", "Populations/" ) + inhabitant.c_str() + "/";
    for( CIT_BlockOccupation it = human.persons_.begin(); it != human.persons_.end(); ++ it )
    {
        const QString keyOccupation = keyBase + it->first + "/";
        const QString keyNumber = keyOccupation + tools::translate( "Block", "Resident" );
        if( !dictionary.HasKey( keyNumber ) )
            dictionary.Register( constEntity, keyNumber, it->second );
    }
    const QString keyAlerted = keyBase + tools::translate( "Block", "Alerted" );
    const QString keyConfined = keyBase + tools::translate( "Block", "Confined" );
    const QString keyEvacuated = keyBase + tools::translate( "Block", "Evacuated" );
    if( !dictionary.HasKey( keyAlerted ) )
        dictionary.Register( constEntity, keyAlerted, human.alerted_ );
    if( !dictionary.HasKey( keyConfined ) )
        dictionary.Register( constEntity, keyConfined, human.confined_ );
    if( !dictionary.HasKey( keyEvacuated ) )
        dictionary.Register( constEntity, keyEvacuated, human.evacuated_ );
    const QString keyAngriness = keyBase + tools::translate( "Block", "Angriness" );
    if( !dictionary.HasKey( keyAngriness ) )
        dictionary.Register( constEntity, keyAngriness, human.angriness_ );
    UpdateColor();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetObject
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
const TerrainObject_ABC* TerrainObjectProxy::GetObject() const
{
    // $$$$ JSR 2010-11-30: Utilisé pour le display dans UrbanKnowledge pour kernel::Formatter< TerrainObject_ABC >
    // A supprimer quand TerrainObjectProxy sera passé dans clients_kernel
    return &object_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::NotifyUpdated
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void TerrainObjectProxy::NotifyUpdated( const UrbanDisplayOptions& )
{
    UpdateColor();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateColor()
{
    const Usages_ABC* pUsages = Retrieve< Usages_ABC >();
    UrbanColor_ABC* pColor = Retrieve< UrbanColor_ABC >();
    if( pUsages && pColor && !options_.SetColor( *pColor, object_.GetLivingSpace(), humans_, *pUsages ) )
        pColor->Restore();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetDensity
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
float TerrainObjectProxy::GetDensity() const
{
    return GetHumans() / object_.GetLivingSpace();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetHumans
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
unsigned int TerrainObjectProxy::GetHumans() const
{
    unsigned int humans = 0;
    BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
        for( CIT_BlockOccupation it = human.second.persons_.begin(); it != human.second.persons_.end(); ++ it )
            humans += it->second;
    return humans;
}
