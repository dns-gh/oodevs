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
// Name: MIL_BaseParameter::IsOfType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::IsOfType( const MIL_ParameterType_ABC& ) const
{
    return false;
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
// Name: MIL_BaseParameter::ToPoint
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPoint( Common::MsgPoint& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPointList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPointList( Common::MsgPointList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygon
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygon( Common::MsgPolygon& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygonList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygonList( Common::MsgPolygonList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgent
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgent( Common::UnitId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledge( Common::UnitKnowledgeId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledgeList( Common::UnitKnowledgeIdList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentList( Common::UnitIdList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomat
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomat( Common::AutomatId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomatList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomatList( Common::AutomatIdList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDirection
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDirection( Common::MsgHeading& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDotationType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDotationType( Common::ResourceType& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToEquipmentType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToEquipmentType( Common::EquipmentType& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGDH
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGDH( Common::MsgDateTime& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObject
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObject( Common::MsgPlannedWork& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObjectList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObjectList( Common::MsgPlannedWorkList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToIndirectFire
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToIndirectFire( Common::FireId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocation
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocation( Common::MsgLocation& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocationList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocationList( Common::MsgLocationList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMaintenancePriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMaintenancePriorities( Common::MsgLogMaintenancePriorities& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMedicalPriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMedicalPriorities( Common::MsgLogMedicalPriorities& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToNatureAtlas
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNatureAtlas( Common::MsgAtlasNature& ) const
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
// Name: MIL_BaseParameter::ToObjectiveList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectiveList( Common::MsgMissionObjectiveList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledge( Common::ObjectKnowledgeId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledgeList( Common::ObjectKnowledgeIdList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPath
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPath( Common::MsgPath& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPathList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPathList( Common::MsgPathList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPopulationKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPopulationKnowledge( Common::CrowdKnowledgeId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLima
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLima( Common::MsgLimasOrder& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLimaList
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLimaList( Common::MsgLimasOrder& ) const
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
bool MIL_BaseParameter::ToAgent( DEC_Decision_ABC*& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentList( std::vector< DEC_Decision_ABC* >& ) const
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
bool MIL_BaseParameter::ToPopulationKnowledge( DEC_Knowledge_Population*& ) const
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
bool MIL_BaseParameter::ToUrbanBlock( Common::UrbanObjectId& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToUrbanBlock
// Created: MGD 2009-11-04
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToList
// Created: LDC 2010-09-22
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToList( std::vector< Common::MsgMissionParameter_Value >& ) const
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
// Name: MIL_BaseParameter::Append
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void MIL_BaseParameter::Append( boost::shared_ptr< TER_Localisation > pLocation )
{
    throw std::runtime_error( "Trying to append a location. Incompatible types." );
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::Append
// Created: JSR 2010-03-04
// -----------------------------------------------------------------------------
void MIL_BaseParameter::Append( boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject )
{
    throw std::runtime_error( "Trying to append a knowledge object. Incompatible types." );
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::Append
// Created: LDC 2010-04-14
// -----------------------------------------------------------------------------
void MIL_BaseParameter::Append( boost::shared_ptr< DEC_Gen_Object > pKnowledgeObject )
{
    throw std::runtime_error( "Trying to append a gen object. Incompatible types." );
}