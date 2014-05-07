// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_BaseParameter.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_BaseParameter )

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter constructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_BaseParameter::MIL_BaseParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter destructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_BaseParameter::~MIL_BaseParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToBool
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToBool( bool& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToId
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToId( int& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToNumeric
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNumeric( float& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToString
// Created: MMC 2011-03-22
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNumericList( std::vector< float >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToString
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToString( std::string& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPoint
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPoint( boost::shared_ptr< MT_Vector2D >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPointList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDirection
// Created: MGD 2010-05-03
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDirection( boost::shared_ptr< MT_Vector2D >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygon
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygon( boost::shared_ptr< TER_Localisation >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygonList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocation
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocation( boost::shared_ptr< TER_Localisation >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPath
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPathList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocationList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToNatureAtlas
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNatureAtlas( int& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomat
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomat( DEC_Decision_ABC*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomatList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgent
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgent( const DEC_Decision_ABC*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentList( std::vector< const DEC_Decision_ABC* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPopulationKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPopulationKnowledge( boost::shared_ptr< DEC_Knowledge_Population >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDotationType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDotationType( const PHY_DotationCategory*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToEquipmentType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToEquipmentType( const PHY_ComposanteTypePion*& ) const
{
    return false;
}

bool MIL_BaseParameter::ToEquipmentTypeList( std::vector< const PHY_ComposanteTypePion* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGDH
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGDH( float& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObject
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObjectList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMaintenancePriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMaintenancePriorities( T_MaintenancePriorityVector& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMedicalPriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMedicalPriorities( T_MedicalPriorityVector& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectiveList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToUrbanBlock
// Created: MGD 2009-11-04
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToUrbanBlock( MIL_UrbanObject_ABC*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToUrbanBlockList
// Created: ABR 2013-11-28
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToUrbanBlockList( std::vector< MIL_UrbanObject_ABC* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToList
// Created: LDC 2010-09-22
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToList
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLima
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLima( boost::shared_ptr< TER_Localisation >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLimaList
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToResourceNetworkNode
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToResourceNetworkList
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToResourceNetworkNodeList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::Append
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void MIL_BaseParameter::Append( boost::shared_ptr< MIL_MissionParameter_ABC > param )
{
    throw MASA_EXCEPTION( "Trying to append a location. Incompatible types." );
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDotationTypeList
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToResourceNetworkType
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToResourceNetworkType( const PHY_ResourceNetworkType*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToResourceNetworkTypeList
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToResourceNetworkTypeList( std::vector< const PHY_ResourceNetworkType* >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::load
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_BaseParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_MissionParameter_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::save
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_BaseParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_MissionParameter_ABC >( *this );
}
