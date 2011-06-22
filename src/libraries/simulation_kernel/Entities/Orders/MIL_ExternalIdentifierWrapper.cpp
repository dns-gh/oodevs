// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ExternalIdentifierWrapper.h"
#include "MIL.h"
#include "Checkpoints/SerializationTools.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ExternalIdentifierWrapper )

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper constructor
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
MIL_ExternalIdentifierWrapper::MIL_ExternalIdentifierWrapper()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper constructor
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
MIL_ExternalIdentifierWrapper::MIL_ExternalIdentifierWrapper( boost::shared_ptr< MIL_MissionParameter_ABC > parameter, unsigned int identifier )
    : parameter_         ( parameter )
    , externalIdentifier_( identifier )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper destructor
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
MIL_ExternalIdentifierWrapper::~MIL_ExternalIdentifierWrapper()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::IsOfType
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return parameter_->IsOfType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToBool
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToBool( bool& value ) const
{
    return parameter_->ToBool( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToId
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToId( int& value ) const
{
    return parameter_->ToId( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToNumeric
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToNumeric( float& value ) const
{
    return parameter_->ToNumeric( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToNumericList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToNumericList( std::vector< float >& value ) const
{
    return parameter_->ToNumericList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToString
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToString( std::string& asn ) const
{
    return parameter_->ToString( asn );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& value ) const
{
    return parameter_->ToList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToElement
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToElement( sword::MissionParameter_Value& value ) const
{
    value.set_external_identifier( externalIdentifier_ );
    return parameter_->ToElement( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToDirection
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToDirection( boost::shared_ptr< MT_Vector2D >& value ) const
{
    return parameter_->ToDirection( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPoint
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPoint( boost::shared_ptr< MT_Vector2D >& value ) const
{
    return parameter_->ToPoint( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPointList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    return parameter_->ToPointList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPolygon
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPolygon( boost::shared_ptr< TER_Localisation >& value ) const
{
    return parameter_->ToPolygon( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPolygonList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& value ) const
{
    return parameter_->ToPolygonList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToLocation
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToLocation( boost::shared_ptr< TER_Localisation >& value ) const
{
    return parameter_->ToLocation( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPath
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    return parameter_->ToPath( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPathList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& value ) const
{
    return parameter_->ToPathList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToLocationList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& value ) const
{
    return parameter_->ToLocationList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToNatureAtlas
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToNatureAtlas( int& atlas ) const
{
    return parameter_->ToNatureAtlas( atlas );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAutomat
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAutomat( DEC_Decision_ABC*& value ) const
{
    return parameter_->ToAutomat( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAutomatList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAutomatList( std::vector< DEC_Decision_ABC* >& value ) const
{
    return parameter_->ToAutomatList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAgent
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAgent( DEC_Decision_ABC*& value ) const
{
    return parameter_->ToAgent( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAgentList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAgentList( std::vector< DEC_Decision_ABC* >& value ) const
{
    return parameter_->ToAgentList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAgentKnowledge
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& value ) const
{
    return parameter_->ToAgentKnowledge( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToAgentKnowledgeList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& value ) const
{
    return parameter_->ToAgentKnowledgeList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToObjectKnowledge
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& value ) const
{
    return parameter_->ToObjectKnowledge( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToObjectKnowledgeList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& value ) const
{
    return parameter_->ToObjectKnowledgeList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToPopulationKnowledge
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToPopulationKnowledge( DEC_Knowledge_Population*& value ) const
{
    return parameter_->ToPopulationKnowledge( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToDotationType
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToDotationType( const PHY_DotationCategory*& value ) const
{
    return parameter_->ToDotationType( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToDotationTypeList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToDotationTypeList( std::vector< const PHY_DotationCategory* >& value ) const
{
    return parameter_->ToDotationTypeList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToEquipmentType
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToEquipmentType( const PHY_ComposanteTypePion*& value ) const
{
    return parameter_->ToEquipmentType( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToGDH
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToGDH( float& value ) const
{
    return parameter_->ToGDH( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToGenObject
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToGenObject( boost::shared_ptr< DEC_Gen_Object >& value ) const
{
    return parameter_->ToGenObject( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToGenObjectList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& value ) const
{
    return parameter_->ToGenObjectList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToMaintenancePriorities
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToMaintenancePriorities( T_MaintenancePriorityVector& value ) const
{
    return parameter_->ToMaintenancePriorities( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToMedicalPriorities
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToMedicalPriorities( T_MedicalPriorityVector& value ) const
{
    return parameter_->ToMedicalPriorities( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToObjectiveList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& value ) const
{
    return parameter_->ToObjectiveList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToUrbanBlock
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToUrbanBlock( UrbanObjectWrapper*& value ) const
{
    return parameter_->ToUrbanBlock( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& value ) const
{
    return parameter_->ToList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToLima
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToLima( boost::shared_ptr< TER_Localisation >& value ) const
{
    return parameter_->ToLima( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToLimaList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& value ) const
{
    return parameter_->ToLimaList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToResourceNetwork
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork >& value ) const
{
    return parameter_->ToResourceNetwork( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::ToResourceNetworkList
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool MIL_ExternalIdentifierWrapper::ToResourceNetworkList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& value ) const
{
    return parameter_->ToResourceNetworkList( value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::Append
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void MIL_ExternalIdentifierWrapper::Append( boost::shared_ptr< MIL_MissionParameter_ABC > param )
{
    parameter_->Append( param );
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::load
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void MIL_ExternalIdentifierWrapper::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_MissionParameter_ABC >( *this )
         >> parameter_
         >> externalIdentifier_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ExternalIdentifierWrapper::save
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void MIL_ExternalIdentifierWrapper::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_MissionParameter_ABC >( *this )
         << parameter_
         << externalIdentifier_;
}
