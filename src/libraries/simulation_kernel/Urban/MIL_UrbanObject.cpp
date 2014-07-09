// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanObject.h"
#include "UrbanPhysicalCapacity.h"
#include "PHY_InfrastructureType.h"
#include "PHY_MaterialCompositionType.h"
#include "Entities/Inhabitants/MIL_LivingArea.h"
#include "Entities/Objects/InfrastructureCapacity.h"
#include "Entities/Objects/MaterialAttribute.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessageParameters.h"
#include "simulation_terrain/TER_Geometry.h"
#include "simulation_terrain/TER_World.h"
#include <boost/lexical_cast.hpp>
#include "tools/NET_AsnException.h"
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_UrbanObject )

const float MIL_UrbanObject::stretchOffset_ = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
MIL_IDManager MIL_UrbanObject::idManager_;
size_t MIL_UrbanObject::maxScaledLocationsNumber_ = 16;

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::MIL_UrbanObject( unsigned long id, const std::string& name, const MIL_ObjectType_ABC& type,
                                  MIL_UrbanObject_ABC* parent /*= 0*/ )
    : MIL_UrbanObject_ABC( type, idManager_.GetId( id ) )
    , nUrbanId_( id )
    , name_( name )
    , parent_( parent )
    , complexity_( 0 )
    , livingSpace_( 0 )
{
    if( name_.empty() )
        name_ = boost::lexical_cast< std::string >( nUrbanId_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject constructor
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
MIL_UrbanObject::MIL_UrbanObject()
    : nUrbanId_( 0 )
    , parent_( 0 )
    , complexity_( 0 )
    , livingSpace_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::~MIL_UrbanObject()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadData
// Created: LGY 2012-09-20
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadData( xml::xistream& xis )
{
    ReadLocalisation( xis );
    ReadColor( xis );
    ReadInfrastructure( xis );
    ReadPhysical( xis );
    ReadResourceNetworks( xis );
    ReadStructuralState( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetUrbanId
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
unsigned long MIL_UrbanObject::GetUrbanId() const
{
    return nUrbanId_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetName
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const std::string& MIL_UrbanObject::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetParent
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject_ABC* MIL_UrbanObject::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeConvexHull
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ComputeConvexHull()
{
    T_PointVector vertices;
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const T_PointVector& objectVertices = it.NextElement().GetLocalisation().GetPoints();
        vertices.insert( vertices.end(), objectVertices.begin(), objectVertices.end() );
    }
    T_PointVector hull;
    TER_Geometry::ComputeHull( hull, vertices );
    if( hull.size() > 2 )
        Initialize( TER_Localisation( TER_Localisation::ePolygon , hull ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetUrbanBlocks
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::GetUrbanBlocks( std::vector< const MIL_UrbanObject_ABC* >& blocks ) const
{
    if( IsBlock() )
        blocks.push_back( this );
    else
    {
        tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
        while( it.HasMoreElements() )
            it.NextElement().GetUrbanBlocks( blocks );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetLivingSpace
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetLivingSpace() const
{
    if( livingSpace_ == 0 )
    {
        const UrbanPhysicalCapacity* physical = Retrieve< UrbanPhysicalCapacity >();
        double factor = physical ? ( physical->GetFloorNumber() + 1 ) * physical->GetOccupation() : 1.;
        return static_cast< float >( GetLocalisation().GetArea() * factor );
    }
    return livingSpace_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetFloorNumber
// Created: LDC 2014-08-27
// -----------------------------------------------------------------------------
int MIL_UrbanObject::GetFloorNumber() const
{
    const UrbanPhysicalCapacity* physical = Retrieve< UrbanPhysicalCapacity >();
    if( physical )
        return 1 + physical->GetFloorNumber();
    return 1;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetOccupation
// Created: LDC 2014-08-27
// -----------------------------------------------------------------------------
double MIL_UrbanObject::GetOccupation() const
{
    const UrbanPhysicalCapacity* physical = Retrieve< UrbanPhysicalCapacity >();
        if( physical )
            return physical->GetOccupation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadLocalisation
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadLocalisation( xml::xistream& xis )
{
    T_PointVector pointVector;
    xis >> xml::optional >> xml::start( "footprint" )
            >> xml::list( "point", *this, &MIL_UrbanObject::ReadPoint, pointVector )
        >> xml::end;
    TER_Localisation location;
    if( location.Reset( TER_Localisation::ePolygon, pointVector ) )
        Initialize( location );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadColor
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadColor( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "color" )
            >> xml::optional >> xml::attribute( "red", color_.red_ )
            >> xml::optional >> xml::attribute( "green", color_.green_ )
            >> xml::optional >> xml::attribute( "blue", color_.blue_ )
            >> xml::optional >> xml::attribute( "alpha", color_.alpha_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadInfrastructure
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadInfrastructure( xml::xistream& xis )
{
    std::string type;
    std::string role;
    xis >> xml::optional >> xml::start( "infrastructures" )
           >> xml::optional >> xml::start( "infrastructure" )
           >> xml::optional >> xml::attribute( "type", type )
           >> xml::optional >> xml::attribute( "role", role )
            >>xml::end
        >>xml::end;
    if( type.empty() && !role.empty() )
        type =  role;
    if( const PHY_InfrastructureType* infraType = PHY_InfrastructureType::Find( type ) )
    {
        InfrastructureCapacity* capacity = new InfrastructureCapacity( *infraType );
        capacity->Register( *this );
        xis >> xml::start( "infrastructures" )
                >> xml::start( "infrastructure" );
        capacity->Update( xis, *this );
        xis     >> xml::end
            >> xml::end;
        if( infraType->IsMedical() )
        {
            MedicalCapacity* capacity = new MedicalCapacity();
            capacity->Register( *this );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadPhysical
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadPhysical( xml::xistream& xis )
{
    if( xis.has_child( "physical" ) || IsBlock() )
    {
        UrbanPhysicalCapacity* pPhysical = new UrbanPhysicalCapacity( xis );
        pPhysical->Register( *this );
        if( const PHY_MaterialCompositionType* material = PHY_MaterialCompositionType::Find(  pPhysical->GetMaterial() ) )
            GetAttribute< MaterialAttribute >() = MaterialAttribute( *material );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadResourceNetworks
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadResourceNetworks( xml::xistream& xis )
{
    if( xis.has_child( "resources") )
        if( ResourceNetworkCapacity* capacity = Retrieve< ResourceNetworkCapacity >() )
        {
            xis >> xml::start( "resources" );
            capacity->Update( xis, *this );
            xis >> xml::end;
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadStructuralState
// Created: JSR 2012-09-05
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadStructuralState( xml::xistream& xis )
{
    if( xis.has_child( "structural-state") )
        if( StructuralCapacity* capacity = Retrieve< StructuralCapacity >() )
        {
            xis >> xml::start( "structural-state" );
            capacity->Update( xis, *this );
            xis >> xml::end;
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadPoint
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadPoint( xml::xistream& xis, T_PointVector& vector )
{
    MT_Vector2D vPoint;
    TER_World::GetWorld().MosToSimMgrsCoord( xis.attribute< std::string >( "location" ), vPoint );
    vector.push_back( vPoint );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WriteLocalisation
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_UrbanObject::WriteLocalisation( xml::xostream& xos ) const
{
    xos << xml::start( "footprint" );
        WritePointVector( xos, GetLocalisation().GetPoints() );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WriteColor
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_UrbanObject::WriteColor( xml::xostream& xos ) const
{
    xos << xml::start( "color" )
            << xml::attribute( "red", color_.red_ )
            << xml::attribute( "green", color_.green_ )
            << xml::attribute( "blue", color_.blue_ )
            << xml::attribute( "alpha", color_.alpha_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WriteCapacity
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
template< typename T >
void MIL_UrbanObject::WriteCapacity( xml::xostream& xos ) const
{
    const T* capacity = Retrieve< T >();
    if( capacity )
        capacity->WriteUrban( xos );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WritePoint
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_UrbanObject::WritePointVector( xml::xostream& xos , const T_PointVector& vector ) const
{
    for( auto it = vector.begin(); it != vector.end(); it++ )
    {
        std::string stringCoord;
        TER_World::GetWorld().SimToMosMgrsCoord( ( *it ), stringCoord );
        xos << xml::start( "point" )
                << xml::attribute( "location", stringCoord )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::load
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void MIL_UrbanObject::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_UrbanObject_ABC >( *this );
    file >> nUrbanId_
         >> name_
         >> parent_
         >> color_.red_
         >> color_.green_
         >> color_.blue_
         >> color_.alpha_
         >> inhabitants_
         >> livingAreas_;
    if( parent_ )
        static_cast< tools::Resolver< MIL_UrbanObject_ABC >* >( parent_ )->Register( nUrbanId_, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::save
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void MIL_UrbanObject::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_UrbanObject_ABC >( *this );
    file << nUrbanId_
         << name_
         << parent_
         << color_.red_
         << color_.green_
         << color_.blue_
         << color_.alpha_
         << inhabitants_
         << livingAreas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WriteUrban
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_UrbanObject::WriteUrban( xml::xostream& xos ) const
{
    xos << xml::start( "urban-object" )
        << xml::attribute( "id", nUrbanId_ )
        << xml::attribute( "name", name_ );
    WriteColor( xos );
    WriteCapacity< UrbanPhysicalCapacity >( xos );
    if( IsBlock() )
    {
        WriteLocalisation( xos );
        WriteCapacity< InfrastructureCapacity >( xos );
        WriteCapacity< ResourceNetworkCapacity >( xos );
        WriteCapacity< StructuralCapacity >( xos );

    }
    if( Count() )
    {
        xos << xml::start( "urban-objects" );
        for( auto it = elements_.begin(); it != elements_.end(); ++it )
            it->second->WriteUrban( xos );
        xos     << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::WriteODB
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void MIL_UrbanObject::WriteODB( xml::xostream& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_UrbanObject::CreateKnowledge( const MIL_KnowledgeGroup& )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::SendCapacity
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
template < typename T >
void MIL_UrbanObject::SendCapacity( sword::UrbanAttributes& msg ) const
{
    const T* capacity = Retrieve< T >();
    if( capacity )
        capacity->SendState( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::SendFullStateCapacity
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
template < typename T >
void MIL_UrbanObject::SendFullStateCapacity( sword::UrbanAttributes& msg ) const
{
    const T* capacity = Retrieve< T >();
    if( capacity )
        capacity->SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::SendCreation
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void MIL_UrbanObject::SendCreation() const
{
    if( !IsBlock() )
        return;
    client::UrbanCreation message;
    message().mutable_object()->set_id( GetID() );
    message().set_name( GetName() );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *message().mutable_location() );
    sword::RgbaColor* msgColor = message().mutable_attributes()->mutable_color();
    msgColor->set_red( color_.red_ );
    msgColor->set_green( color_.green_ );
    msgColor->set_blue( color_.blue_ );
    msgColor->set_alpha( color_.alpha_ );
    SendFullStateCapacity< UrbanPhysicalCapacity >( *message().mutable_attributes() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::SendDestruction
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void MIL_UrbanObject::SendDestruction() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::SendFullState
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void MIL_UrbanObject::SendFullState() const
{
    if( !IsBlock() )
        return;
    client::UrbanUpdate message;
    message().mutable_object()->set_id( GetID() );
    SendFullStateCapacity< StructuralCapacity >( *message().mutable_attributes() );
    SendFullStateCapacity< ResourceNetworkCapacity >( *message().mutable_attributes() );
    SendFullStateCapacity< InfrastructureCapacity >( *message().mutable_attributes() );
    message.Send( NET_Publisher_ABC::Publisher() );
    size_t notSendableAttributes = RetrieveAttribute< MaterialAttribute >() ? 1 : 0;
    if( CountAttributes() > notSendableAttributes )
        MIL_UrbanObject_ABC::SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::UpdateState
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void MIL_UrbanObject::UpdateState()
{
    if( !IsBlock() )
        return;
    client::UrbanUpdate message;
    message().mutable_object()->set_id( GetID() );
    SendCapacity< StructuralCapacity >( *message().mutable_attributes() );
    SendCapacity< ResourceNetworkCapacity >( *message().mutable_attributes() );
    SendCapacity< InfrastructureCapacity >( *message().mutable_attributes() );
    if( message().attributes().has_structure() || message().attributes().has_infrastructures() )
        message.Send( NET_Publisher_ABC::Publisher() );
    size_t notSendableAttributes = RetrieveAttribute< MaterialAttribute >() ? 1 : 0;
    if( CountAttributes() > notSendableAttributes )
        MIL_UrbanObject_ABC::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeLocalisationsInsideBlock
// Created: MIA 2011-06-13
// -----------------------------------------------------------------------------
const std::vector< boost::shared_ptr< MT_Vector2D > >& MIL_UrbanObject::ComputeLocalisationsInsideBlock() const
{
    if( stretchedArea_.empty() )
    {
        const T_PointVector& points = GetLocalisation().GetPoints();
        const MT_Vector2D barycenter = GetLocalisation().ComputeBarycenter();
        for( auto it = points.begin(); it != points.end(); ++it )
            stretchedArea_.push_back( boost::make_shared< MT_Vector2D >( *it + MT_Vector2D( *it - barycenter ).Normalize() * stretchOffset_ ) );
    }
    return stretchedArea_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::IsBlock
// Created: JSR 2012-04-27
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::IsBlock() const
{
    return GetParent() != 0 && GetParent()->GetParent() != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::UpdateInhabitants
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
void MIL_UrbanObject::UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number )
{
    inhabitants_[ &livingArea ][ motivation ] = number;
    if( number == 0 )
    {
        inhabitants_[ &livingArea ].erase( motivation );
        unsigned int total = 0;
        for( auto it = inhabitants_[ &livingArea ].begin(); it != inhabitants_[ &livingArea ].end(); ++it )
            total += it->second;
        if( total == 0 )
            inhabitants_.erase( &livingArea );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetTotalInhabitants
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
unsigned int MIL_UrbanObject::GetTotalInhabitants() const
{
    unsigned int ret = 0;
    for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            ret += it2->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetScaledLocation
// Created: LDC 2014-08-14
// -----------------------------------------------------------------------------
TER_Polygon MIL_UrbanObject::GetScaledLocation( double distance ) const
{
    auto it = scaledLocations_.find( distance );
    if( it == scaledLocations_.end() )
    {
        TER_Polygon polygon;
        TER_Geometry::Buffer( polygon, GetLocalisation().GetPoints(), distance );
        lastUsedScaledLocations_.push_front( distance );
        scaledLocations_.insert( std::make_pair( distance,
            std::make_pair( polygon, lastUsedScaledLocations_.begin() )));
        if( lastUsedScaledLocations_.size() > maxScaledLocationsNumber_ )
        {
            scaledLocations_.erase( lastUsedScaledLocations_.back() );
            lastUsedScaledLocations_.pop_back();
        }
        return polygon;
    }
    else
    {
        lastUsedScaledLocations_.splice( lastUsedScaledLocations_.begin(),
                lastUsedScaledLocations_, it->second.second );
        return it->second.first;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetTotalInhabitantsForMotivation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_UrbanObject::GetTotalInhabitantsForMotivation( const std::string& motivation ) const
{
    unsigned int ret = 0;
    for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if( it2->first == motivation )
                ret += it2->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::OnUpdate
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::OnUpdate( const sword::MissionParameters& params )
{
    auto size = params.elem( 0 ).value().size();
    for( auto i = 0; i < size; ++i )
    {
        const unsigned int actionId = protocol::GetIdentifier( params, 0, i, 0 );
        const sword::MissionParameter_Value& attribute = params.elem( 0 ).value( i );
        switch( actionId )
        {
        case sword::ObjectMagicAction::alerted:
            OnReceiveSetAlerted( attribute );
            break;
        case sword::ObjectMagicAction::confined:
            OnReceiveSetConfined( attribute );
            break;
        case sword::ObjectMagicAction::evacuated:
            OnReceiveSetEvacuated( attribute );
            break;
        default:
            break;
        }
    }
    MIL_UrbanObject_ABC::OnUpdate( params );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::OnReceiveSetAlerted
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::OnReceiveSetAlerted( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool alerted = attribute.list( 1 ).booleanvalue();
        for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
            it->first->SetAlerted( alerted, this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::OnReceiveSetConfined
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::OnReceiveSetConfined( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool confined = attribute.list( 1 ).booleanvalue();
        for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
            it->first->SetConfined( confined, this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::OnReceiveSetEvacuated
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void MIL_UrbanObject::OnReceiveSetEvacuated( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool evacuated = attribute.list( 1 ).booleanvalue();
        for( auto it = livingAreas_.begin(); it != livingAreas_.end(); ++it )
            ( *it )->SetEvacuated( evacuated, this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::AddLivingArea
// Created: ABR 2011-03-24
// -----------------------------------------------------------------------------
void MIL_UrbanObject::AddLivingArea( MIL_LivingArea& livingArea )
{
    livingAreas_.push_back( &livingArea );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::CanInteractWith
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    if( !IsBlock() )
        return false;
    return MIL_UrbanObject_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::CanInteractWith
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::CanInteractWith( const MIL_Population& population ) const
{
    if( !IsBlock() )
        return false;
    return MIL_UrbanObject_ABC::CanInteractWith( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::CanInteractWithEntity
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::CanInteractWithEntity() const
{
    if( !IsBlock() )
        return false;
    return MIL_UrbanObject_ABC::CanInteractWithEntity();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ApplyIndirectFire
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ApplyIndirectFire( const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation, MIL_Army_ABC* army )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ApplyIndirectFire( attritionSurface, dotation, army );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ApplyDirectFire
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ApplyDirectFire() const
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ApplyDirectFire();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ApplyDestruction
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ApplyDestruction( const TER_Localisation& attritionSurface, const PHY_UrbanAttritionData& attrition )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ApplyDestruction( attritionSurface, attrition );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::PreprocessAgent
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::PreprocessAgent( MIL_Agent_ABC& agent )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::PreprocessAgent( agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ProcessAgentEntering
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ProcessAgentEntering( agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ProcessAgentExiting
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ProcessAgentExiting( agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ProcessAgentMovingInside
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ProcessAgentMovingInside( agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ProcessAgentInside
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ProcessAgentInside( agent );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ProcessPopulationInside
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ProcessPopulationInside( population );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ApplyStructuralState
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ApplyStructuralState( float structuralState ) const
{
    if( !IsBlock() )
        return;
    MIL_UrbanObject_ABC::ApplyStructuralState( structuralState );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::CanCollideWithEntity
// Created: LDC 2012-11-07
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::CanCollideWithEntity() const
{
    if( !IsBlock() )
        return false;
    return MIL_UrbanObject_ABC::CanCollideWithEntity();
}
