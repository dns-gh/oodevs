// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __StubMIL_MissionParameter_ABC_h_
#define __StubMIL_MissionParameter_ABC_h_

#include "simulation_orders/MIL_MissionParameter_ABC.h"

// =============================================================================
/** @class  StubMIL_MissionParameter_ABC
    @brief  StubMIL_MissionParameter_ABC
*/
// Created: LDC 2009-04-30
// =============================================================================
class StubMIL_MissionParameter_ABC
    : public MIL_MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StubMIL_MissionParameter_ABC( const std::string& param )
        : param_( param )
             {}
    virtual ~StubMIL_MissionParameter_ABC() {}
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& )const
    { throw; }
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const
    { throw; }
    virtual bool ToId( int& ) const
    { throw; }
    virtual bool ToPoint( Common::MsgPoint& ) const
    { throw; }
    virtual bool ToPointList( Common::MsgPointList& ) const
    { throw; }
    virtual bool ToPolygon( Common::MsgPolygon& ) const
    { throw; }
    virtual bool ToPolygonList( Common::MsgPolygonList& ) const
    { throw; }
    virtual bool ToAgent( Common::MsgUnit& ) const
    { throw; }
    virtual bool ToAgentKnowledge( Common::MsgUnitKnowledge& ) const
    { throw; }
    virtual bool ToAgentKnowledgeList( Common::MsgUnitKnowledgeList& ) const
    { throw; }
    virtual bool ToAgentList( Common::MsgUnitList& ) const
    { throw; }
    virtual bool ToAutomat( Common::MsgAutomat& ) const
    { throw; }
    virtual bool ToAutomatList( Common::MsgAutomatList& ) const
    { throw; }
    virtual bool ToDirection( Common::MsgHeading& ) const
    { throw; }
    virtual bool ToDotationType( Common::MsgDotationType& ) const
    { throw; }
    virtual bool ToEquipmentType( Common::MsgEquipmentType& ) const
    { throw; }
    virtual bool ToGDH( Common::MsgDateTime& ) const
    { throw; }
    virtual bool ToGenObject( Common::MsgPlannedWork& ) const
    { throw; }
    virtual bool ToGenObjectList( Common::MsgPlannedWorkList& ) const
    { throw; }
    virtual bool ToIndirectFire( Common::MsgUnitFire& ) const
    { throw; }
    virtual bool ToLocation( Common::MsgLocation& ) const
    { throw; }
    virtual bool ToLocationList( Common::MsgLocationList& ) const
    { throw; }
    virtual bool ToMaintenancePriorities( Common::MsgLogMaintenancePriorities& ) const
    { throw; }
    virtual bool ToMedicalPriorities( Common::MsgLogMedicalPriorities& ) const
    { throw; }
    virtual bool ToNatureAtlas( Common::MsgAtlasNature& ) const
    { throw; }
    virtual bool ToNumeric( float& ) const
    { throw; }
    virtual bool ToObjectiveList( Common::MsgMissionObjectiveList& ) const
    { throw; }
    virtual bool ToObjectKnowledge( Common::MsgObjectKnowledge& ) const
    { throw; }
    virtual bool ToObjectKnowledgeList( Common::MsgObjectKnowledgeList& ) const
    { throw; }
    virtual bool ToPath( Common::MsgPath& ) const
    { throw; }
    virtual bool ToPathList( Common::MsgPathList& ) const
    { throw; }
    virtual bool ToPopulationKnowledge( Common::MsgPopulationKnowledge& ) const
    { throw; }
    virtual bool ToString( std::string& result ) const
    {
        result = param_;
        return true;
    }
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const
    { throw; }
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const
    { throw; }
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw; }
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const
    { throw; }
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
    { throw; }
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const
    { throw; }
    virtual bool ToLocationList( std::vector< boost::shared_ptr<TER_Localisation > >& ) const
    { throw; }
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw; }
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const
    { throw; }
    virtual bool ToNatureAtlas( int& ) const
    { throw; }
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const
    { throw; }
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const
    { throw; }
    virtual bool ToAgent( DEC_Decision_ABC*& ) const
    { throw; }
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& ) const
    { throw; }
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const
    { throw; }
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const
    { throw; }
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const
    { throw; }
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const
    { throw; }
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const
    { throw; }
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const
    { throw; }
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const
    { throw; }
    virtual bool ToGDH( float& ) const
    { throw; }
    virtual bool ToGenObject( boost::shared_ptr<DEC_Gen_Object >& ) const
    { throw; }
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const
    { throw; }
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const
    { throw; }
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const
    { throw; }
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const
    { throw; }
    virtual bool ToUrbanBlock( Common::MsgUrbanBlock& ) const
    { throw; }
    virtual bool ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& ) const
    { throw; }
    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation )
    { throw; }
    virtual void Append( boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject )
    { throw; }
    virtual void Append( boost::shared_ptr< DEC_Gen_Object > pGenObject )
    { throw; }
    //@}

private:
    std::string param_;
};

#endif // __StubMIL_MissionParameter_ABC_h_
