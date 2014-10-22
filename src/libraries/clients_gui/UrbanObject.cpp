// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DictionaryUpdated.h"
#include "PropertiesDictionary.h"
#include "UrbanObject.h"
#include "UrbanDisplayOptions.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/StructuralStateAttribute_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/UrbanTemplateType.h"
#include "clients_kernel/Usages_ABC.h"

#include "protocol/Simulation.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace gui;

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: UrbanObject gaming constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Controllers& controllers, const std::string& name, unsigned int id, const ObjectType& type
    , const AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : EntityImplementation< UrbanObject_ABC >( controllers.controller_, id, name.c_str() )
    , density_         ( 0 )
    , type_            ( type )
    , templateType_    ( 0 )
    , accommodations_  ( accommodations )
    , livingSpace_     ( 0 )
    , nominalCapacity_ ( 0 )
    , updatingTemplate_( false )
    , fillingTemplate_ ( false )
    , controllers_     ( controllers )
    , options_         ( options )
{
    AddExtension( *this );
    CreateDictionary( false );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject preparation/terrain constructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( xml::xistream& xis, Controllers& controllers, const ObjectType& type
    , const AccommodationTypes& accommodations, UrbanDisplayOptions& options )
    : EntityImplementation< UrbanObject_ABC >( controllers.controller_,
                                               xis.attribute< unsigned int >( "id" ),
                                               ( xis.attribute< std::string >( "name" ).empty() )
                                               ? tools::translate( "Urban", "Urban block[%L1]" ).arg( xis.attribute< unsigned int >( "id" ) )
                                               : xis.attribute< std::string >( "name" ).c_str() )
    , density_         ( 0 )
    , type_            ( type )
    , templateType_    ( 0 )
    , accommodations_  ( accommodations )
    , livingSpace_     ( 0 )
    , nominalCapacity_ ( 0 )
    , updatingTemplate_( false )
    , fillingTemplate_ ( false )
    , controllers_     ( controllers )
    , options_         ( options )
{
    AddExtension( *this );
    CreateDictionary( true );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    kernel::Hierarchies* hierarchies = Retrieve< kernel::Hierarchies >();
    if( hierarchies )
    {
        Entity_ABC* superior = const_cast< Entity_ABC* >( hierarchies->GetSuperior() );
        if( superior )
            superior->Get< kernel::Hierarchies >().RemoveSubordinate( *this );
    }
    if( UrbanColor_ABC* pColor = Retrieve< UrbanColor_ABC >() )
        pColor->Restore();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Update
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void UrbanObject::DoUpdate( const sword::UrbanUpdate& /*msg*/ )
{
    Touch();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject GetName
// Created: SLG 2009-11-2
// -----------------------------------------------------------------------------
QString UrbanObject::GetName() const
{
    if( name_.isEmpty() )
        return QString( tools::translate( "Urban", "Urban block[%L1]" ).arg( id_ ) );
    return name_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void UrbanObject::CreateDictionary( bool readOnly )
{
    PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
    dictionary.Register( static_cast< const UrbanObject_ABC& >( *this ), tools::translate( "Block", "Info/Template" ), templateType_, *this, &UrbanObject::ApplyTemplate, readOnly, eUrbanTemplate );
    dictionary.Register( static_cast< const EntityImplementation& >( *this ), tools::translate( "EntityImplementation", "Info/Name" ), name_, static_cast< EntityImplementation& >( *this ), &EntityImplementation::Rename, !readOnly );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::ApplyTemplate
// Created: JSR 2012-06-20
// -----------------------------------------------------------------------------
void UrbanObject::ApplyTemplate( const UrbanTemplateTypePtr& urbanTemplate )
{
    if( urbanTemplate )
    {
        fillingTemplate_ = true;
        templateType_ = urbanTemplate;
        urbanTemplate->Fill( *this );
        controllers_.controller_.Update( static_cast< const UrbanObject_ABC& >( *this ) );
        controllers_.controller_.Update( DictionaryUpdated( *this, tools::translate( "Block", "PhysicalFeatures/Architecture" ) ) );
        fillingTemplate_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::UpdateTemplate
// Created: JSR 2012-06-20
// -----------------------------------------------------------------------------
void UrbanObject::UpdateTemplate( const ObjectTypes& objectTypes )
{
    if( fillingTemplate_ )
        return;
    updatingTemplate_ = true;
    UrbanTemplateTypePtr old = templateType_;
    templateType_ = 0;
    tools::Iterator< const UrbanTemplateType& > it = objectTypes.tools::StringResolver< UrbanTemplateType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const UrbanTemplateType& type = it.NextElement();
        if( type.Matches( *this ) )
        {
            templateType_ = const_cast< UrbanTemplateTypePtr >( &type );
            break;
        }
    }
    if( templateType_ != old )
    {
        controllers_.controller_.Update( static_cast< const UrbanObject_ABC& >( *this ) );
        controllers_.controller_.Update( DictionaryUpdated( *this, tools::translate( "Block", "Info/Template" ) ) );
    }
    updatingTemplate_ = false;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::IsUpdatingTemplate
// Created: JSR 2012-06-21
// -----------------------------------------------------------------------------
bool UrbanObject::IsUpdatingTemplate() const
{
    return updatingTemplate_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::DisplayInSummary
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void UrbanObject::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Block", "Density:" ), density_ );
    displayer.Display( tools::translate( "Block", "Total of inhabitants:" ), GetHumans() );
    for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
        displayer.Display( tools::translate( "Block", "Occupation rate (%L1):" ).arg( it->first ), it->second.second );
}

namespace
{
    template< typename T >
    void RegisterValue( UrbanObject_ABC& object, const QString& key, const T& value )
    {
        PropertiesDictionary& dictionary = object.Get< PropertiesDictionary >();
        if( !dictionary.HasKey( key ) )
            dictionary.Register( static_cast< const Entity_ABC& >( object ), key, value, true );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::UpdateHumans
// Created: LGY 2010-12-30
// -----------------------------------------------------------------------------
void UrbanObject::UpdateHumans( const std::string& inhabitant, const sword::PopulationUpdate_BlockOccupation& occupation )
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
    for( auto it = human.persons_.begin(); it != human.persons_.end(); ++ it )
    {
        const QString keyOccupation = keyBase + it->first + "/";
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Resident" ), it->second.first );
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Occupation rate" ), it->second.second );
    }
    RegisterValue( *this, keyBase + tools::translate( "Block", "Alerted" ), human.alerted_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Confined" ), human.confined_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Evacuated" ), human.evacuated_ );
    RegisterValue( *this, keyBase + tools::translate( "Block", "Angriness" ), human.angriness_ );
    for( auto it = motivations_.begin(); it != motivations_.end(); ++it )
        it->second.first = 0;
    for( auto h = humans_.begin(); h != humans_.end(); ++h )
        for( auto it = h->second.persons_.begin(); it != h->second.persons_.end(); ++it )
            motivations_[ it->first ].first += it->second.first;
    for( auto it = motivations_.begin(); it != motivations_.end(); ++ it )
    {
        double nominalCapacity = GetNominalCapacity( it->first.toStdString() );
        it->second.second = nominalCapacity ?  static_cast< unsigned int >( 100 * it->second.first / nominalCapacity ) : 0;
        const QString keyOccupation = tools::translate( "Block", "Occupations/" ) + it->first + "/";
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Resident" ), it->second.first );
        RegisterValue( *this, keyOccupation + tools::translate( "Block", "Occupation rate" ), it->second.second );
    }
    UpdateDensity();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::UpdateDensity
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
void UrbanObject::UpdateDensity()
{
    float livingSpace = GetLivingSpace();
    density_ = livingSpace == 0 ? 0 : GetHumans() / livingSpace;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::GetHumans
// Created: LGY 2011-01-07
// -----------------------------------------------------------------------------
unsigned int UrbanObject::GetHumans() const
{
    unsigned int humans = 0;
    BOOST_FOREACH( const T_Humans::value_type& human, humans_ )
        for( auto it = human.second.persons_.begin(); it != human.second.persons_.end(); ++ it )
            humans += it->second.first;
    return humans;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::GetLivingSpace
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
float UrbanObject::GetLivingSpace( unsigned int floorNumber, unsigned int occupation ) const
{
    if( livingSpace_ == 0 )
    {
        if( const UrbanPositions_ABC* positions = Retrieve< UrbanPositions_ABC >() )
        {
            livingSpace_ = positions->ComputeArea();
            livingSpace_ *= ( floorNumber + 1 ) * ( static_cast< float >( occupation ) * 0.01f );
        }
    }
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::GetLivingSpace
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanObject::GetLivingSpace( bool forceUpdate /* = false */ ) const
{
    livingSpace_ = forceUpdate ? 0 : livingSpace_;
    if( livingSpace_ == 0 )
    {
        const Architecture_ABC& architecture = Get< PhysicalAttribute_ABC >().GetArchitecture();
        return GetLivingSpace( architecture.GetFloorNumber(), architecture.GetOccupation() );
    }
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double UrbanObject::GetNominalCapacity() const
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
// Name: UrbanObject::GetNominalCapacity
// Created: JSR 2011-09-27
// -----------------------------------------------------------------------------
double UrbanObject::GetNominalCapacity( const std::string& motivation ) const
{
    if( const AccommodationType* acc = accommodations_.Find( motivation ) )
        return GetLivingSpace() * Get< PhysicalAttribute_ABC >().FindUsagesValue( motivation ) * 0.01 * acc->GetNominalCapacity();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::GetAccommodations
// Created: MMC 2012-03-21
// -----------------------------------------------------------------------------
const kernel::AccommodationTypes& UrbanObject::GetAccommodations() const
{
    return accommodations_;
}
