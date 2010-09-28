// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_MissionParameter_ABC_h_
#define __MIL_MissionParameter_ABC_h_

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Common
{
    class AutomatId;
    class AutomatIdList;
    class EquipmentType;
    class FireId;
    class MsgAtlasNature;
    class MsgDateTime;
    class MsgHeading;
    class MsgLocation;
    class MsgLocationList;
    class MsgLogMaintenancePriorities;
    class MsgLogMedicalPriorities;
    class MsgMissionObjectiveList;
    class MsgMissionParameter_Value;
    class MsgPath;
    class MsgPathList;
    class MsgPlannedWork;
    class MsgPlannedWorkList;
    class MsgPoint;
    class MsgPointList;
    class MsgPolygon;
    class MsgPolygonList;
    class MsgUnitKnowledge;
    class ObjectKnowledgeId;
    class ObjectKnowledgeIdList;
    class CrowdKnowledgeId;
    class ResourceType;
    class UnitId;
    class UnitIdList;
    class UnitKnowledgeId;
    class UnitKnowledgeIdList;
    class UrbanObjectId;
}

namespace urban
{
    class Block;
}

class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_Knowledge_Urban;
class DEC_Objective;
class MIL_ParameterType_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;
class PHY_ComposanteTypePion;
class PHY_HumanWound;
class TER_Localisation;

// =============================================================================
/** @class  MIL_MissionParameter_ABC
    @brief  MIL Mission parameter definition
*/
// Created: LDC 2009-04-29
// =============================================================================
class MIL_MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_MissionParameter_ABC() {}
    virtual ~MIL_MissionParameter_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;

    typedef std::vector< const PHY_HumanWound* > T_MedicalPriorityVector;
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const = 0;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const = 0;
    virtual bool ToId( int& ) const = 0;
    virtual bool ToPoint( Common::MsgPoint& ) const = 0;
    virtual bool ToPointList( Common::MsgPointList& ) const = 0;
    virtual bool ToPolygon( Common::MsgPolygon& ) const = 0;
    virtual bool ToPolygonList( Common::MsgPolygonList& ) const = 0;
    virtual bool ToAgent( Common::UnitId& ) const = 0;
    virtual bool ToAgentKnowledge( Common::UnitKnowledgeId& ) const = 0;
    virtual bool ToAgentKnowledgeList( Common::UnitKnowledgeIdList& asn ) const = 0;
    virtual bool ToAgentList( Common::UnitIdList& asn ) const = 0;
    virtual bool ToAutomat( Common::AutomatId& ) const = 0;
    virtual bool ToAutomatList( Common::AutomatIdList& ) const = 0;
    virtual bool ToDirection( Common::MsgHeading& ) const = 0;
    virtual bool ToDotationType( Common::ResourceType& ) const = 0;
    virtual bool ToEquipmentType( Common::EquipmentType& ) const = 0;
    virtual bool ToGDH( Common::MsgDateTime& ) const = 0;
    virtual bool ToGenObject( Common::MsgPlannedWork& ) const = 0;
    virtual bool ToGenObjectList( Common::MsgPlannedWorkList& ) const = 0;
    virtual bool ToIndirectFire( Common::FireId& ) const = 0;
    virtual bool ToLocation( Common::MsgLocation& ) const = 0;
    virtual bool ToLocationList( Common::MsgLocationList& ) const = 0;
    virtual bool ToMaintenancePriorities( Common::MsgLogMaintenancePriorities& ) const = 0;
    virtual bool ToMedicalPriorities( Common::MsgLogMedicalPriorities& ) const = 0;
    virtual bool ToNatureAtlas( Common::MsgAtlasNature& diaTo ) const = 0;
    virtual bool ToNumeric( float& ) const = 0;
    virtual bool ToObjectiveList( Common::MsgMissionObjectiveList& asn ) const = 0;
    virtual bool ToObjectKnowledge( Common::ObjectKnowledgeId& asn ) const = 0;
    virtual bool ToObjectKnowledgeList( Common::ObjectKnowledgeIdList& asn ) const = 0;
    virtual bool ToPath( Common::MsgPath& asn ) const = 0;
    virtual bool ToPathList( Common::MsgPathList& asn ) const = 0;
    virtual bool ToPopulationKnowledge( Common::CrowdKnowledgeId& asn ) const = 0;
    virtual bool ToString( std::string& asn ) const = 0;
    virtual bool ToUrbanBlock( Common::UrbanObjectId& ) const = 0;
    virtual bool ToList( std::vector< Common::MsgMissionParameter_Value >& ) const = 0;

    //virtual bool ToString( std::string& ) const = 0;
    // The lifecycle of pointers in all functions below must be handled by MIL_MissionParameter_ABC
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const = 0;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const = 0;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const = 0;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const = 0;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const = 0;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const = 0;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const = 0;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const = 0;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const = 0;
    virtual bool ToNatureAtlas( int& atlas ) const = 0;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const = 0;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const = 0;
    virtual bool ToAgent( DEC_Decision_ABC*& ) const = 0;
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& ) const = 0;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const = 0;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const = 0;
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const = 0;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const = 0;
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const = 0;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const = 0;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const = 0;
    virtual bool ToGDH( float& value ) const = 0;
    //virtual bool ToNumeric( float& value ) const = 0;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const = 0;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const = 0;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const = 0;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const = 0;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const = 0;
    virtual bool ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& ) const = 0;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const = 0;

    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation ) = 0;
    virtual void Append( boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject ) = 0;
    virtual void Append( boost::shared_ptr< DEC_Gen_Object > pKnowledgeObject ) = 0;
    //@}
};

#endif // __MIL_MissionParameter_ABC_h_
