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
bool MIL_BaseParameter::ToPoint( ASN1T_Point& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPointList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPointList( ASN1T_PointList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygon
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygon( ASN1T_Polygon& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPolygonList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPolygonList( ASN1T_PolygonList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgent
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgent( ASN1T_Unit& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledge( ASN1T_UnitKnowledge& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAgentList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAgentList( ASN1T_UnitList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomat
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomat( ASN1T_Automat& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToAutomatList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToAutomatList( ASN1T_AutomatList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDirection
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDirection( ASN1T_Heading& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToDotationType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToDotationType( ASN1T_DotationType& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToEquipmentType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToEquipmentType( ASN1T_EquipmentType& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGDH
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGDH( ASN1T_DateTime& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObject
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObject( ASN1T_PlannedWork& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToGenObjectList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToGenObjectList( ASN1T_PlannedWorkList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToIndirectFire
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToIndirectFire( ASN1T_UnitFire& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocation
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocation( ASN1T_Location& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToLocationList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToLocationList( ASN1T_LocationList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMaintenancePriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMaintenancePriorities( ASN1T_LogMaintenancePriorities& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToMedicalPriorities
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToMedicalPriorities( ASN1T_LogMedicalPriorities& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToNatureAtlas
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNatureAtlas( ASN1T_AtlasNature& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToNumeric
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNumeric( ASN1REAL& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectiveList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectiveList( ASN1T_MissionObjectiveList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledge( ASN1T_ObjectKnowledge& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPath
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPath( ASN1T_Path& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPathList
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPathList( ASN1T_PathList& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToPopulationKnowledge
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToPopulationKnowledge( ASN1T_PopulationKnowledge& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_BaseParameter::ToString
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToString( ASN1VisibleString& ) const
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
// Name: MIL_BaseParameter::ToNumeric
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_BaseParameter::ToNumeric( float& ) const
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
// Name: MIL_BaseParameter::Append
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void MIL_BaseParameter::Append( boost::shared_ptr< TER_Localisation > pLocation )
{
    throw std::runtime_error( "Trying to append a location. Incompatible types." );
}
