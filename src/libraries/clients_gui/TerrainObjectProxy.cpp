// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
#include "protocol/Simulation.h"
#include <urban/PhysicalAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <boost/foreach.hpp>
#include <urban/ColorAttribute.h>

using namespace gui;
using namespace kernel;
using namespace urban;

const QString TerrainObjectProxy::typeName_ = "terrainObjectProxy";

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( Controllers& controllers, TerrainObject_ABC& object, unsigned int id
                                      , const QString& name, const ObjectType& type, UrbanDisplayOptions& options )
    : EntityImplementation< Object_ABC >( controllers.controller_, id, name )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_     ( object )
    , controllers_( controllers )
    , type_       ( type )
    , options_    ( options )
{
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    ColorAttribute* colorAttribute = object_.Retrieve< ColorAttribute >();
    if( colorAttribute )
    {
        color_.red_ = colorAttribute->Red();
        color_.green_ = colorAttribute->Green();
        color_.blue_ = colorAttribute->Blue();
    }
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( Controllers& controllers, TerrainObject_ABC& object, const ObjectType& type, UrbanDisplayOptions& options )
    : EntityImplementation< Object_ABC >( controllers.controller_, object.GetId(), object.GetName().c_str() )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_     ( object )
    , controllers_( controllers )
    , type_       ( type )
    , options_    ( options )
{
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    ColorAttribute* colorAttribute = object_.Retrieve< ColorAttribute >();
    if( colorAttribute )
    {
        color_.red_ = colorAttribute->Red();
        color_.green_ = colorAttribute->Green();
        color_.blue_ = colorAttribute->Blue();
    }
    UpdateColor();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::~TerrainObjectProxy()
{
    Restore();
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Restore
// Created: LGY 2011-01-11
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Restore()
{
    ColorAttribute* colorAttribute = object_.Retrieve< ColorAttribute >();
    if( colorAttribute )
    {
        colorAttribute->SetRed( color_.red_ );
        colorAttribute->SetGreen( color_.green_ );
        colorAttribute->SetBlue( color_.blue_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy operator==
// Created: MGD 2009-11-2
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::operator==( const TerrainObjectProxy& object ) const
{
    return &object_ == &object.object_;
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
    const std::string& name = object_.GetName();
    if ( name.empty() )
        return QString( tools::translate( "Urban", "Urban block[%1]" ).arg( object_.GetId() ) );
    return name.c_str();
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
    AddDictionaryForArchitecture( dictionary );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForArchitecture
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForArchitecture( PropertiesDictionary& dictionary )
{
    const PhysicalAttribute* pPhysical = object_.Retrieve< PhysicalAttribute >();
    const Entity_ABC& constEntity = *static_cast< const Entity_ABC* >( this );
    if( pPhysical )
    {
        if( pPhysical->GetArchitecture() )
        {
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), pPhysical->GetArchitecture()->GetHeight() );
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), pPhysical->GetArchitecture()->GetFloorNumber() );
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), pPhysical->GetArchitecture()->GetRoofShape() );
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), pPhysical->GetArchitecture()->GetMaterial() );
            occupation_ = static_cast< unsigned int >( pPhysical->GetArchitecture()->GetOccupation() * 100u );
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), occupation_ );
            dictionary.Register( constEntity, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), pPhysical->GetArchitecture()->GetTrafficability() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::SetSelected
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
void TerrainObjectProxy::SetSelected( bool selected ) const
{
    object_.SetSelected( selected );
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
// Name: TerrainObjectProxy::GetFootprint
// Created: SLG 2010-11-26
// -----------------------------------------------------------------------------
const geometry::Polygon2f* TerrainObjectProxy::GetFootprint() const
{
    return object_.GetFootprint();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetObject
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
const TerrainObject_ABC* TerrainObjectProxy::GetObject() const
{
    // $$$$ JSR 2010-11-30: Utilis� pour le display dans UrbanKnowledge pour kernel::Formatter< TerrainObject_ABC >
    // A supprimer quand TerrainObjectProxy sera pass� dans clients_kernel
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
    ColorAttribute* pColorAttribute = object_.Retrieve< ColorAttribute >();
    Usages_ABC* pUsages = Retrieve< Usages_ABC >();
    if( pUsages && !options_.SetColor( pColorAttribute, object_.GetLivingSpace(), humans_, *pUsages ) )
        Restore();
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
