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
#include "UrbanColorAttribute.h"
#include "UrbanPhysicalAttribute.h"
#include "UrbanResourceNetworkAttribute.h"
#include "PHY_InfrastructureType.h"
#include "PHY_MaterialCompositionType.h"
#include "MIL_UrbanMotivationsVisitor_ABC.h"
#include "Entities/Inhabitants/MIL_LivingArea.h"
#include "Entities/Objects/InfrastructureCapacity.h"
#include "Entities/Objects/MaterialAttribute.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "protocol/ClientSenders.h"
#include <boost/lexical_cast.hpp>

/*
#include "MIL_AgentServer.h"
#include "ResourceNetworkAttribute.h"
#include "ResourceNetworkCapacity.h"
#include "MaterialAttribute.h"
#include "MedicalCapacity.h"
#include "InfrastructureCapacity.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Urban/PHY_InfrastructureType.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/MIL_UrbanMotivationsVisitor_ABC.h"
#include "Urban/UrbanColorAttribute.h"
#include "Urban/UrbanPhysicalAttribute.h"
#include "Urban/UrbanResourceNetworkAttribute.h"
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>
*/

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_UrbanObject )

const float MIL_UrbanObject::stretchOffset_ = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::MIL_UrbanObject( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_UrbanObject_ABC* parent /*= 0*/ )
    : MIL_UrbanObject_ABC( builder.GetType() )
    , nUrbanId_ ( xis.attribute< unsigned long >( "id" ) )
    , name_     ( xis.attribute< std::string >( "name" ) )
    , parent_   ( parent )
{
    if( name_.empty() )
        name_ = boost::lexical_cast< std::string >( nUrbanId_ );

    ReadLocalisation( xis );

    // TEMP
    // TODO refaire au propre quand on n'aura plus les attributs dans urban et qu'on aura viré le wrapper
    // Passer par l'attribute factory? 
    tools::Extendable< UrbanExtension_ABC >::Attach( *new UrbanColorAttribute( xis ) );
    if( xis.has_child( "physical" ) || ( parent && parent->GetParent() ) )
        tools::Extendable< UrbanExtension_ABC >::Attach( *new UrbanPhysicalAttribute( xis ) );

    xis >> xml::optional >> xml::start( "infrastructures" )
            >> xml::optional >> xml::start( "infrastructure" )
                >> xml::attribute( "type", infrastructure_ )
            >>xml::end
        >>xml::end;
    if( xis.has_child( "resources") )
        tools::Extendable< UrbanExtension_ABC >::Attach( *new UrbanResourceNetworkAttribute( xis ) );
    builder.Build( *this );
    InitializeAttributes();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject constructor
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
MIL_UrbanObject::MIL_UrbanObject()
    : nUrbanId_( 0 )
    , parent_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::~MIL_UrbanObject()
{
    tools::Extendable< UrbanExtension_ABC >::DestroyExtensions();
    DeleteAll();
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
// Name: MIL_UrbanObject::Accept
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const
{
    const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >();
    if( pPhysical )
        pPhysical->Accept( visitor );
}

namespace
{
    template< typename IT >
    bool FindOuterPoint( IT begin, IT end, const MT_Vector2D& from, const MT_Vector2D& direction, MT_Vector2D& worst )
    {
        bool bFound = false;
        double rMaxProjection = 0;
        for( IT it = begin; it != end; ++it )
        {
            const MT_Vector2D v( *it - from );
            const double rProjection = CrossProduct( direction, v );
            if( rProjection < -1.f ) // epsilon
            {
                bFound = true;
                if( rMaxProjection > rProjection )
                {
                    rMaxProjection = rProjection;
                    worst = *it;
                }
            }
        }
        return bFound;
    }
    
    void ComputeHull( const T_PointVector& vertices, T_PointVector& hull )
    {
        if( !vertices.empty() )
        {
            MT_Vector2D maxLeft = *vertices.begin();
            MT_Vector2D maxRight = maxLeft;

            for( T_PointVector::const_iterator it = vertices.begin(); it != vertices.end(); ++it )
            {
                if( it->rX_ < maxLeft.rX_ )
                    maxLeft = *it;
                if( it->rX_ > maxRight.rX_ )
                    maxRight = *it;
            }
            hull.push_back( maxLeft );
            hull.push_back( maxRight );
            unsigned int nPoint = 0;
            while( nPoint != hull.size() )
            {
                unsigned int nFollowingPoint = ( nPoint + 1 ) % hull.size();
                MT_Vector2D direction( hull[ nFollowingPoint ] -  hull[ nPoint ] );
                direction.Normalize();
                MT_Vector2D worst;
                if( FindOuterPoint( vertices.begin(), vertices.end(), hull[ nPoint ], direction, worst ) )
                {
                    hull.insert( hull.begin() + nFollowingPoint, worst );
                    nPoint = 0;
                }
                else
                    ++nPoint;
            }
        }
    }
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
    ComputeHull( vertices, hull );
    if( !hull.empty() )
        Initialize( TER_Localisation( TER_Localisation::ePolygon , hull ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetUrbanObjectLeaves
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::GetUrbanObjectLeaves( std::vector< const MIL_UrbanObject_ABC* >& leaves ) const
{
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    if( !it.HasMoreElements() )
        leaves.push_back( this );
    else
        while( it.HasMoreElements() )
            it.NextElement().GetUrbanObjectLeaves( leaves );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::HasChild
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::HasChild() const
{
    return Count() > 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetLivingSpace
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetLivingSpace() const
{
    const UrbanPhysicalAttribute* physical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >();
    double factor = physical ? ( physical->GetArchitecture().floorNumber_ + 1 ) * physical->GetArchitecture().occupation_ : 1.;
    return static_cast< float >( GetLocalisation().GetArea() * factor );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeComplexity
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float MIL_UrbanObject::ComputeComplexity() const
{
    // $$$$ _RC_ JSR 2010-09-16: A refaire (voir PHY_RolePion_UrbanLocation::Execute dans la sim)
    double area = GetLocalisation().GetArea();
    double complexity = area ? area * 0.1f : 1.f;
    const UrbanPhysicalAttribute* physical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >();
    if( physical )
        complexity *= ( ( 1 + physical->GetArchitecture().floorNumber_ ) * physical->GetArchitecture().occupation_ );
    return static_cast< float >( complexity );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetInfrastructure
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const std::string& MIL_UrbanObject::GetInfrastructure() const
{
    return infrastructure_;
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
    if( !pointVector.empty() )
        Initialize( TER_Localisation( TER_Localisation::ePolygon , pointVector ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ReadPoint
// Created: JSR 2012-08-28
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ReadPoint( xml::xistream& xis, T_PointVector& vector )
{
    std::string strPoint = xis.attribute< std::string >( "location" );
    MT_Vector2D vPoint;
    TER_World::GetWorld().MosToSimMgrsCoord( strPoint, vPoint );
    vector.push_back( vPoint );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::InitializeAttributes
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
void MIL_UrbanObject::InitializeAttributes()
{
    // resource network
    if( const UrbanResourceNetworkAttribute* resource = static_cast< tools::Extendable< UrbanExtension_ABC >* >( this )->Retrieve< UrbanResourceNetworkAttribute >() )
    {
        ResourceNetworkCapacity* capacity = Retrieve< ResourceNetworkCapacity >();
        if( capacity )
            capacity->Initialize( *resource );
    }
    if( const PHY_InfrastructureType* infraType = PHY_InfrastructureType::Find( GetInfrastructure() ) )
    {
        InfrastructureCapacity* capacity = new InfrastructureCapacity( *infraType );
        capacity->Register( *this );
        if( const PHY_InfrastructureType::MedicalProperties* medical = infraType->GetMedicalProperties() )
        {
            MedicalCapacity* capacity = new MedicalCapacity( medical->emergencyBedsRate_, medical->emergencyDoctorsRate_, medical->nightDoctorsRate_ );
            capacity->Register( *this );
        }
    }
    if( const UrbanPhysicalAttribute* pPhysical = static_cast< tools::Extendable< UrbanExtension_ABC >* >( this )->Retrieve< UrbanPhysicalAttribute >() )
        if( const PHY_MaterialCompositionType* material = PHY_MaterialCompositionType::Find(  pPhysical->GetArchitecture().material_ ) )
            GetAttribute< MaterialAttribute >() = MaterialAttribute( *material );
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
         >> infrastructure_
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
         << infrastructure_
         << inhabitants_
         << livingAreas_;
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
boost::shared_ptr< DEC_Knowledge_Object > MIL_UrbanObject::CreateKnowledge( const MIL_Army_ABC& )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
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

namespace
{
    class MotivationsVisitor : public MIL_UrbanMotivationsVisitor_ABC

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
    if( const UrbanColorAttribute* color = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanColorAttribute >() )
    {
        sword::RgbaColor* msg = message().mutable_attributes()->mutable_color();
        msg->set_red( color->Red() );
        msg->set_green( color->Green() );
        msg->set_blue( color->Blue() );
        msg->set_alpha( color->Alpha() );
    }
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
    {
        // architecture
        const UrbanPhysicalAttribute::Architecture& architecture = pPhysical->GetArchitecture();
        sword::UrbanAttributes_Architecture* msg = message().mutable_attributes()->mutable_architecture();
        msg->set_height( static_cast< float >( architecture.height_ ) );
        msg->set_floor_number( architecture.floorNumber_ );
        msg->set_roof_shape( architecture.roofShape_.c_str() );
        msg->set_material( architecture.material_.c_str() );
        msg->set_occupation( static_cast< float >( architecture.occupation_ ) );
        msg->set_trafficability( static_cast< float >( architecture.trafficability_ ) );
        msg->set_parking_floors( architecture.parkingFloors_ );
        //motivations
        std::map< std::string, float > motivations;
        MotivationsVisitor visitor( motivations );
        pPhysical->Accept( visitor );
        for( std::map< std::string, float >::const_iterator it = motivations.begin(); it != motivations.end(); ++it )
        {
            sword::UrbanUsage& usage = *message().mutable_attributes()->add_usages();
            usage.set_role( it->first );
            usage.set_percentage( static_cast< unsigned int >( it->second * 100 + 0.5f ) );
        }
    }
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
        MIL_Object::SendFullState();
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
        MIL_Object::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeLocalisationsInsideBlock
// Created: MIA 2011-06-13
// -----------------------------------------------------------------------------
const std::vector< boost::shared_ptr< MT_Vector2D > >& MIL_UrbanObject::ComputeLocalisationsInsideBlock() const
{
    if( strechedArea_.size() == 0 )
    {
        const T_PointVector& points = GetLocalisation().GetPoints();
        const MT_Vector2D barycenter = GetLocalisation().ComputeBarycenter();
        for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
            strechedArea_.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *it + MT_Vector2D( *it - barycenter ).Normalize() * stretchOffset_ ) ) );
    }
    return strechedArea_;
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
// Name: MIL_UrbanObject::GetHeight
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetHeight() const
{
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        return static_cast< float >( pPhysical->GetArchitecture().height_ );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetStructuralHeight
// Created: MMC 2012-01-09
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetStructuralHeight() const
{
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        if( const StructuralCapacity* structuralCapacity = Retrieve< StructuralCapacity >() )
            return pPhysical->GetArchitecture().height_ * structuralCapacity->GetStructuralState();
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetStructuralState
// Created: MMC 2012-01-23
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetStructuralState() const
{
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        if( const StructuralCapacity* structuralCapacity = Retrieve< StructuralCapacity >() )
            return structuralCapacity->GetStructuralState();
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetOccupation
// Created: JSR 2011-05-26
// -----------------------------------------------------------------------------
double MIL_UrbanObject::GetOccupation() const
{
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        return pPhysical->GetArchitecture().occupation_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetTrafficability
// Created: JSR 2011-05-26
// -----------------------------------------------------------------------------
double MIL_UrbanObject::GetTrafficability() const
{
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        return pPhysical->GetArchitecture().trafficability_;
    return std::numeric_limits< double >::max();
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
        for( CIT_Motivations it = inhabitants_[ &livingArea ].begin(); it != inhabitants_[ &livingArea ].end(); ++it )
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
    for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( CIT_Motivations it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            ret += it2->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetTotalInhabitantsForMotivation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_UrbanObject::GetTotalInhabitantsForMotivation( const std::string& motivation ) const
{
    unsigned int ret = 0;
    for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( CIT_Motivations it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if( it2->first == motivation )
                ret += it2->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::OnUpdate
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode MIL_UrbanObject::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
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
    return MIL_Object::OnUpdate( attributes );
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
        for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
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
        for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
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
        for( IT_LivingAreas it = livingAreas_.begin(); it != livingAreas_.end(); ++it )
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
// Name: MIL_UrbanObject::HasArchitecture
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::HasArchitecture() const
{
    return tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetMaterial
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
const std::string& MIL_UrbanObject::GetMaterial() const
{
    static const std::string empty;
    if( const UrbanPhysicalAttribute* pPhysical = tools::Extendable< UrbanExtension_ABC >::Retrieve< UrbanPhysicalAttribute >() )
        return pPhysical->GetArchitecture().material_;
    return empty;
}


// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::HasParent
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::HasParent() const
{
    return parent_ != 0;
}
