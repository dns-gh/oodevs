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
#include "Gradient.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include <urban/Architecture.h>
#include <urban/TerrainObject_ABC.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <urban/ColorAttribute.h>

using namespace gui;

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, unsigned int id,
                                        const QString& name, const kernel::ObjectType& type )
    : EntityImplementation< kernel::Object_ABC >( controllers.controller_, id, name )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_      ( object )
    , controllers_ ( controllers )
    , densityColor_( false )
    , pGradient_   ( 0 )
    , type_        ( type )
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
    const std::string colors = controllers.options_.GetOption( "Density/urbanBlock", QString( "default" ) ).To< QString >();
    if( colors != "default" )
    {
        pGradient_.reset( new Gradient() );
        pGradient_->LoadValues( colors.c_str() );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, const kernel::ObjectType& type )
    : EntityImplementation< kernel::Object_ABC >( controllers.controller_, object.GetId(), QString( object.GetName().c_str() ) )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , object_      ( object )
    , controllers_ ( controllers )
    , densityColor_( false )
    , pGradient_   ( 0 )
    , type_        ( type )
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
    const std::string colors = controllers.options_.GetOption( "Density/urbanBlock", QString( "default" ) ).To< QString >();
    if( colors != "default" )
    {
        pGradient_.reset( new Gradient() );
        pGradient_->LoadValues( colors.c_str() );
    }
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
        colorAttribute->SetBlue( color_.red_ );
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
    return object_.GetName().c_str();
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

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForArchitecture
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary )
{
    urban::Architecture* architecture = object_.Retrieve< urban::Architecture >();
    if( architecture )
    {
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), architecture->GetHeight() );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), architecture->GetFloorNumber() );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), architecture->GetRoofShape() );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), architecture->GetMaterial() );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), architecture->GetOccupation() );
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), architecture->GetTrafficability() );
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
    if( !humans_.empty() )
    {
        displayer.Display( "Populations:", "" );
        BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
        {
            const std::string name = " - " + boost::lexical_cast< std::string >( human.first ) + ":";
            displayer.Display( name.c_str(), human.second );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateHumans( const std::string& inhabitant, unsigned int number )
{
    humans_[ inhabitant ] = number;
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
// Name: TerrainObjectProxy::OptionChanged
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void TerrainObjectProxy::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "UrbanDensityColor" )
    {
        bool density = value.To< bool >();
        if( densityColor_ != density )
        {
            densityColor_ = density;
            UpdateColor();
        }
    }
    else if( name == "Density/urbanBlock" )
    {
        pGradient_.reset( new Gradient() );
        pGradient_->LoadValues( value.To< QString >() );
        UpdateColor();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateColor
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateColor()
{
    urban::ColorAttribute* colorAttribute = object_.Retrieve< urban::ColorAttribute >();
    if( colorAttribute && pGradient_.get() )
    {
        unsigned short red = color_.red_;
        unsigned short green = color_.green_;
        unsigned short blue = color_.blue_;
        if( densityColor_ )
        {
            float density = GetDensity();
            QColor result = pGradient_->Compute( density, colorAttribute->Alpha() );
            red = static_cast< unsigned short >( result.red() );
            green = static_cast< unsigned short >( result.green() );
            blue = static_cast< unsigned short >( result.blue() );
        }
        colorAttribute->SetRed( red );
        colorAttribute->SetGreen( green );
        colorAttribute->SetBlue( blue );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetDensity
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
float TerrainObjectProxy::GetDensity() const
{
    float surface = object_.GetFootprint()->ComputeArea();
    unsigned int floors = 1u;
    urban::Architecture* architecture = object_.Retrieve< urban::Architecture >();
    if( architecture )
        floors += architecture->GetFloorNumber();
    if( surface == 0.f )
        return 0.f;
    return GetHumans() / ( surface * floors );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetHumans
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
unsigned int TerrainObjectProxy::GetHumans() const
{
    unsigned int humans = 0;
    BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
        humans += human.second;
    return humans;
}
