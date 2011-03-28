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
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <urban/PhysicalAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/MotivationsVisitor_ABC.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <urban/ColorAttribute.h>

using namespace gui;

const QString TerrainObjectProxy::typeName_ = "terrainObjectProxy";

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, unsigned int id,
                                        const QString& name, const kernel::ObjectType& type, gui::UrbanDisplayOptions& options )
    : EntityImplementation< kernel::Object_ABC >( controllers.controller_, id, name )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_      ( object )
    , controllers_ ( controllers )
    , type_        ( type )
    , options_     ( options )
{
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    urban::ColorAttribute* colorAttribute = object_.Retrieve< urban::ColorAttribute >();
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
TerrainObjectProxy::TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, const kernel::ObjectType& type, gui::UrbanDisplayOptions& options )
    : EntityImplementation< kernel::Object_ABC >( controllers.controller_, object.GetId(), QString( object.GetName().c_str() ) )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_      ( object )
    , controllers_ ( controllers )
    , type_        ( type )
    , options_     ( options )
{
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    urban::ColorAttribute* colorAttribute = object_.Retrieve< urban::ColorAttribute >();
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
    urban::ColorAttribute* colorAttribute = object_.Retrieve< urban::ColorAttribute >();
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
    std::string name = object_.GetName();
    if ( name.empty() )
        return QString( tools::translate( "Urban", "Urban block[%1]" ).arg( object_.GetId() ) );
    return object_.GetName().c_str();
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
void TerrainObjectProxy::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void TerrainObjectProxy::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary& dictionary = *new kernel::PropertiesDictionary( controller );
    EntityImplementation< kernel::Object_ABC >::Attach( dictionary );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Identifier" ), EntityImplementation< kernel::Object_ABC >::id_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Name" ), EntityImplementation< kernel::Object_ABC >::name_ );
    AddDictionaryForArchitecture( dictionary );
}

namespace
{
    class MotivationsVisitor : public urban::MotivationsVisitor_ABC
    {
    public:
        explicit MotivationsVisitor( std::map< std::string, float >& motivations )
            : motivations_( motivations )
        {
            // NOTHING
        }
        virtual void Visit( const std::string& motivation, float proportion )
        {
            motivations_[ motivation ] = proportion;
        }
        std::map< std::string, float >& motivations_;
    };
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForArchitecture
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary )
{
    const urban::PhysicalAttribute* pPhysical = object_.Retrieve< urban::PhysicalAttribute >();
    if( pPhysical )
    {
        if( pPhysical->GetArchitecture() )
        {
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), pPhysical->GetArchitecture()->GetHeight() );
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), pPhysical->GetArchitecture()->GetFloorNumber() );
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), pPhysical->GetArchitecture()->GetRoofShape() );
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), pPhysical->GetArchitecture()->GetMaterial() );
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), pPhysical->GetArchitecture()->GetOccupation() );
            dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), pPhysical->GetArchitecture()->GetTrafficability() );
        }
        if( pPhysical->GetMotivations() )
        {
            MotivationsVisitor visitor( motivations_ );
            object_.Accept( visitor );
            BOOST_FOREACH( const T_Motivations::value_type& motivation, motivations_ )
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + QString( motivation.first.c_str() ), motivation.second );
        }
    }
    BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
    {
        const QString keyBase = tools::translate( "Block", "Populations/" ) + human.first.c_str() + "/";
        const QString keyNumber = keyBase + tools::translate( "Block", "Resident" );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyNumber, human.second.number_ );
        const QString keyAlerted = keyBase + tools::translate( "Block", "Alerted" );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyAlerted, human.second.alerted_ );
        const QString keyConfined = keyBase + tools::translate( "Block", "Confined" );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyConfined, human.second.confined_ );
        const QString keyEvacuated = keyBase + tools::translate( "Block", "Evacuated" );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyEvacuated, human.second.evacuated_ );
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
void TerrainObjectProxy::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Block", "Density:" ), GetDensity() );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateHumans( const std::string& inhabitant, unsigned int number, bool alerted, bool confined, bool evacuated )
{
    T_Human& mutableHuman = humans_[ inhabitant ];
    mutableHuman.number_ = number;
    mutableHuman.alerted_ = alerted;
    mutableHuman.confined_ = confined;
    mutableHuman.evacuated_ = evacuated;
    const T_Human& human = mutableHuman;
    const QString keyBase = tools::translate( "Block", "Populations/" ) + inhabitant.c_str() + "/";
    kernel::PropertiesDictionary& dictionary = Get< kernel::PropertiesDictionary >();
    const QString keyNumber = keyBase + tools::translate( "Block", "Resident" );
    if( !dictionary.HasKey( keyNumber ) )
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyNumber, human.number_ );
    const QString keyAlerted = keyBase + tools::translate( "Block", "Alerted" );
    if( !dictionary.HasKey( keyAlerted ) )
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyAlerted, human.alerted_ );
    const QString keyConfined = keyBase + tools::translate( "Block", "Confined" );
    if( !dictionary.HasKey( keyConfined ) )
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyConfined, human.confined_ );
    const QString keyEvacuated = keyBase + tools::translate( "Block", "Evacuated" );
    if( !dictionary.HasKey( keyEvacuated ) )
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyEvacuated, human.evacuated_ );
    if( human.number_ == 0u )
    {
        dictionary.Remove( keyNumber );
        dictionary.Remove( keyAlerted );
        dictionary.Remove( keyConfined );
        dictionary.Remove( keyEvacuated );
        humans_.erase( inhabitant );
    }
    UpdateColor();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::IsInside
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::IsInside( const geometry::Point2f& point ) const
{
    return object_.GetFootprint()->IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Barycenter
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
geometry::Point2f TerrainObjectProxy::Barycenter() const
{
    return object_.GetFootprint()->Barycenter();
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
const urban::TerrainObject_ABC* TerrainObjectProxy::GetObject() const
{
    // $$$$ JSR 2010-11-30: Utilisé pour le display dans UrbanKnowledge pour kernel::Formatter< TerrainObject_ABC >
    // A supprimer quand TerrainObjectProxy sera passé dans clients_kernel
    return &object_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::NotifyUpdated
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void TerrainObjectProxy::NotifyUpdated( const gui::UrbanDisplayOptions& )
{
    UpdateColor();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateColor()
{
    urban::ColorAttribute* colorAttribute = object_.Retrieve< urban::ColorAttribute >();
    if( !options_.SetColor( colorAttribute, GetDensity(), motivations_ ) )
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
        humans += human.second.number_;
    return humans;
}
