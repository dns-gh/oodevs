// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanObject_ABC.h"
#include "Tools.h"
#include "AccommodationType.h"
#include "AccommodationTypes.h"
#include "ActionController.h"
#include "Architecture_ABC.h"
#include "PhysicalAttribute_ABC.h"
#include "PropertiesDictionary.h"
#include "UrbanColor_ABC.h"
#include "UrbanPositions_ABC.h"
#include "Usages_ABC.h"
#include "protocol/Simulation.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

const QString UrbanObject_ABC::typeName_ = "urbanObject";

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject_ABC::UrbanObject_ABC( Controller& controller, const std::string& name, unsigned int id, const ObjectType& type
                                , const AccommodationTypes& accommodations )
    : EntityImplementation< Object_ABC >( controller, id, name.c_str() )
    , Creatable< UrbanObject_ABC >( controller, this )
    , density_        ( 0 )
    , type_           ( type )
    , accommodations_ ( accommodations )
    , livingSpace_    ( 0 )
    , nominalCapacity_( 0 )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC constructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanObject_ABC::UrbanObject_ABC( xml::xistream& xis, kernel::Controller& controller
                                , const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations )
    : EntityImplementation< Object_ABC >( controller, xis.attribute< unsigned int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , Creatable< UrbanObject_ABC >( controller, this )
    , density_        ( 0 )
    , type_           ( type )
    , accommodations_ ( accommodations )
    , livingSpace_    ( 0 )
    , nominalCapacity_( 0 )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject_ABC::~UrbanObject_ABC()
{
    if( UrbanColor_ABC* pColor = Retrieve< UrbanColor_ABC >() )
        pColor->Restore();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::Update
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void UrbanObject_ABC::DoUpdate( const sword::UrbanUpdate& /*msg*/ )
{
    Touch();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC GetName
// Created: SLG 2009-11-2
// -----------------------------------------------------------------------------
QString UrbanObject_ABC::GetName() const
{
    if( name_.isEmpty() )
        return QString( tools::translate( "Urban", "Urban block[%L1]" ).arg( id_ ) );
    return name_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetTypeName
// Created: ABR 2011-02-18
// -----------------------------------------------------------------------------
QString UrbanObject_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::Select
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void UrbanObject_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::MultipleSelect
// Created: JSR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanObject_ABC::MultipleSelect( ActionController& controller, const std::vector< const Selectable_ABC* >& elements ) const
{
    std::vector< const UrbanObject_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( int i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const UrbanObject_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::Activate
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void UrbanObject_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void UrbanObject_ABC::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    EntityImplementation< Object_ABC >::Attach( dictionary );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Identifier" ), static_cast< const unsigned long& >( id_ ) );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Block", "Info/Name" ), static_cast< const QString& >( name_ ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::DisplayInSummary
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void UrbanObject_ABC::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Block", "Density:" ), density_ );
    displayer.Display( tools::translate( "Block", "Total of inhabitants:" ), GetHumans() );
    for( CIT_BlockOccupation it = motivations_.begin(); it != motivations_.end(); ++it )
        displayer.Display( tools::translate( "Block", "Occupation rate (%L1):" ).arg( it->first ), it->second.second );
}

namespace
{
    template< typename T >
    void RegisterValue( UrbanObject_ABC& object, const QString& key, const T& value )
    {
        PropertiesDictionary& dico = object.Get< PropertiesDictionary >();
        if( !dico.HasKey( key ) )
            dico.Register( static_cast< const Entity_ABC& >( object ), key, value );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void UrbanObject_ABC::UpdateHumans( const std::string& inhabitant, const sword::PopulationUpdate_BlockOccupation& occupation )
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
// Name: UrbanObject_ABC::UpdateDensity
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void UrbanObject_ABC::UpdateDensity()
{
    float livingSpace = GetLivingSpace();
    density_ = livingSpace == 0 ? 0 : GetHumans() / livingSpace;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetHumans
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
unsigned int UrbanObject_ABC::GetHumans() const
{
    unsigned int humans = 0;
    BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
        for( CIT_BlockOccupation it = human.second.persons_.begin(); it != human.second.persons_.end(); ++ it )
            humans += it->second.first;
    return humans;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetLivingSpace
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
float UrbanObject_ABC::GetLivingSpace( unsigned int floorNumber, unsigned int occupation ) const
{
    if( livingSpace_ == 0 )
        if( const UrbanPositions_ABC* positions = Retrieve< UrbanPositions_ABC >() )
        {
            livingSpace_ = positions->ComputeArea();
            livingSpace_ *= ( floorNumber + 1 ) * ( static_cast< float >( occupation ) * 0.01f );
        }
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetLivingSpace
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanObject_ABC::GetLivingSpace() const
{
    if( livingSpace_ == 0 )
        if( const Architecture_ABC* architecture = Get< PhysicalAttribute_ABC >().GetArchitecture() )
            return GetLivingSpace( architecture->GetFloorNumber(), architecture->GetOccupation() );
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double UrbanObject_ABC::GetNominalCapacity() const
{
    if( nominalCapacity_ == 0 )
    {
        tools::Iterator< const AccommodationType& > it = accommodations_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const AccommodationType& acc = it.NextElement();
            double proportion = Get< PhysicalAttribute_ABC >().FindUsagesValue( acc.GetRole() ) * 0.01;
            nominalCapacity_ += GetLivingSpace() * proportion * acc.GetNominalCapacity();
        }
    }
    return nominalCapacity_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double UrbanObject_ABC::GetNominalCapacity( const std::string& motivation ) const
{
    if( const AccommodationType* acc = accommodations_.Find( motivation ) )
        return GetLivingSpace() * Get< PhysicalAttribute_ABC >().FindUsagesValue( motivation ) * 0.01 * acc->GetNominalCapacity();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetAccommodations
// Created: MMC 2012-03-21
// -----------------------------------------------------------------------------
const kernel::AccommodationTypes& UrbanObject_ABC::GetAccommodations() const
{
    return accommodations_; // $$$$ ABR 2012-05-25: WTF !!! C'est deja dans staticModel...
}