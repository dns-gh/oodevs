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
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "protocol/Simulation.h"
#include <boost/foreach.hpp>

using namespace gui;
using namespace kernel;

const QString TerrainObjectProxy::typeName_ = "terrainObjectProxy";

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( Controllers& controllers, const std::string& name, unsigned int id, const ObjectType& type
                                      , UrbanDisplayOptions& options, const AccommodationTypes& accommodations )
    : EntityImplementation< Object_ABC >( controllers.controller_, id, name.c_str() )
    , Creatable< TerrainObjectProxy >( controllers.controller_, this )
    , controllers_    ( controllers )
    , density_        ( 0 )
    , type_           ( type )
    , accommodations_ ( accommodations )
    , livingSpace_    ( 0 )
    , nominalCapacity_( 0 )
    , options_        ( options )
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
    if( UrbanColor_ABC* pColor = Retrieve< UrbanColor_ABC >() )
        pColor->Restore();
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
    if( name_.isEmpty() )
        return QString( tools::translate( "Urban", "Urban block[%1]" ).arg( id_ ) );
    return name_;
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
// Name: TerrainObjectProxy::Activate
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void TerrainObjectProxy::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    EntityImplementation< Object_ABC >::Attach( dictionary );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Identifier" ), static_cast< const unsigned long& >( id_ ) );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Name" ), static_cast< const QString& >( name_ ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::DisplayInSummary
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Block", "Density:" ), density_ );
    displayer.Display( tools::translate( "Block", "Total of inhabitants:" ), GetHumans() );
    for( CIT_BlockOccupation it = motivations_.begin(); it != motivations_.end(); ++it )
        displayer.Display( tools::translate( "Block", "Occupation rate (%1):" ).arg( it->first ), it->second.second );
}

namespace
{
    template< typename T >
    void RegisterValue( TerrainObjectProxy& proxy, const QString& key, const T& value )
    {
        PropertiesDictionary& dico = proxy.Get< PropertiesDictionary >();
        if( !dico.HasKey( key ) )
            dico.Register( static_cast< const Entity_ABC& >( proxy ), key, value );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateHumans( const std::string& inhabitant, const sword::PopulationUpdate_BlockOccupation& occupation )
{
    T_Human& mutableHuman = humans_[ inhabitant ];
    for( int i = 0; i < occupation.persons_size(); ++i )
    {
        unsigned int number = occupation.persons( i ).number();
        const QString& usage = occupation.persons( i ).usage().c_str();
        mutableHuman.persons_[ usage ].first = number;
        mutableHuman.persons_[ usage ].second = GetNominalCapacity() == 0 ? 0 : static_cast< unsigned int >( 100 * number / GetNominalCapacity() );
    }
    mutableHuman.alerted_ = occupation.alerted();
    mutableHuman.confined_ = occupation.confined();
    mutableHuman.evacuated_ = occupation.evacuated();
    mutableHuman.angriness_ = occupation.angriness();
    const T_Human& human = mutableHuman;
    const QString keyBase = tools::translate( "Block", "Populations/" ) + inhabitant.c_str() + "/";
    for( CIT_BlockOccupation it = human.persons_.begin(); it != human.persons_.end(); ++ it )
    {
        const QString keyOccupation = keyBase + it->first + "/";
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Resident" ), it->second.first );
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Occupation rate" ), it->second.second );
    }
    RegisterValue( *this, keyBase + tools::translate( "Block", "Alerted" ), human.alerted_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Confined" ), human.confined_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Evacuated" ), human.evacuated_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Angriness" ), human.angriness_ );
    for( IT_BlockOccupation it = motivations_.begin(); it != motivations_.end(); ++it )
        it->second.first = 0;
    for( T_Humans::const_iterator h = humans_.begin(); h != humans_.end(); ++h )
        for( CIT_BlockOccupation it = h->second.persons_.begin(); it != h->second.persons_.end(); ++it )
            motivations_[ it->first ].first += it->second.first;
    for( IT_BlockOccupation it = motivations_.begin(); it != motivations_.end(); ++ it )
    {
        double nominalCapacity = GetNominalCapacity( it->first.toStdString() );
        it->second.second = nominalCapacity ?  static_cast< unsigned int >( 100 * it->second.first / nominalCapacity ) : 0;
        const QString keyOccupation = tools::translate( "Block", "Occupations/" ) + it->first + "/";
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Resident" ), it->second.first );
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Occupation rate" ), it->second.second );
    }
    UpdateDensity();
    UpdateColor();
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
    if( pUsages && pColor && !options_.SetColor( *pColor, GetLivingSpace(), humans_, *pUsages ) )
        pColor->Restore();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::UpdateDensity
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void TerrainObjectProxy::UpdateDensity()
{
    float livingSpace = GetLivingSpace();
    density_ = livingSpace == 0 ? 0 : GetHumans() / livingSpace;
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
            humans += it->second.first;
    return humans;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetLivingSpace
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float TerrainObjectProxy::GetLivingSpace() const
{
    if( livingSpace_ == 0 )
        if( const UrbanPositions_ABC* positions = Retrieve< UrbanPositions_ABC >() )
        {
            livingSpace_ = positions->ComputeArea();
            if( const Architecture_ABC* architecture = Retrieve< Architecture_ABC >() )
                livingSpace_ *= ( architecture->GetFloorNumber() + 1 ) * ( static_cast< float >( architecture->GetOccupation() ) * 0.01f );
        }
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double TerrainObjectProxy::GetNominalCapacity() const
{
    if( nominalCapacity_ == 0 )
    {
        tools::Iterator< const AccommodationType& > it = accommodations_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const AccommodationType& acc = it.NextElement();
            double proportion = Get< Usages_ABC >().Find( acc.GetRole() ) * 0.01;
            nominalCapacity_ += GetLivingSpace() * proportion * acc.GetNominalCapacity();
        }
    }
    return nominalCapacity_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double TerrainObjectProxy::GetNominalCapacity( const std::string& motivation ) const
{
    if( const AccommodationType* acc = accommodations_.Find( motivation ) )
        return GetLivingSpace() * Get< Usages_ABC >().Find( motivation ) * 0.01 * acc->GetNominalCapacity();
    return 0;
}
