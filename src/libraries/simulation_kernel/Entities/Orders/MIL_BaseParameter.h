// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_BaseParameter_h_
#define __MIL_BaseParameter_h_

#include "simulation_orders/MIL_MissionParameter_ABC.h"

namespace urban
{
    class Block;
}

// =============================================================================
/** @class  MIL_BaseParameter
    @brief  MIL_BaseParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_BaseParameter : public MIL_MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_BaseParameter();
    virtual ~MIL_BaseParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const;
    virtual bool ToId( int& ) const;
    virtual bool ToPoint( Common::MsgPoint& ) const;
    virtual bool ToPointList( Common::MsgPointList& ) const;
    virtual bool ToPolygon( Common::MsgPolygon& ) const;
    virtual bool ToPolygonList( Common::MsgPolygonList& ) const;
    virtual bool ToAgent( Common::UnitId& ) const;
    virtual bool ToAgentKnowledge( Common::UnitKnowledgeId& ) const;
    virtual bool ToAgentKnowledgeList( Common::UnitKnowledgeIdList& asn ) const;
    virtual bool ToAgentList( Common::UnitIdList& asn ) const;
    virtual bool ToAutomat( Common::AutomatId& ) const;
    virtual bool ToAutomatList( Common::AutomatIdList& ) const;
    virtual bool ToDirection( Common::MsgHeading& ) const;
    virtual bool ToDotationType( Common::ResourceType& ) const;
    virtual bool ToEquipmentType( Common::EquipmentType& ) const;
    virtual bool ToGDH( Common::MsgDateTime& ) const;
    virtual bool ToGenObject( Common::MsgPlannedWork& ) const;
    virtual bool ToGenObjectList( Common::MsgPlannedWorkList& ) const;
    virtual bool ToIndirectFire( Common::FireId& ) const;
    virtual bool ToLocation( Common::MsgLocation& ) const;
    virtual bool ToLocationList( Common::MsgLocationList& ) const;
    virtual bool ToMaintenancePriorities( Common::MsgLogMaintenancePriorities& ) const;
    virtual bool ToMedicalPriorities( Common::MsgLogMedicalPriorities& ) const;
    virtual bool ToNatureAtlas( Common::MsgAtlasNature& diaTo ) const;
    virtual bool ToNumeric( float& ) const;
    virtual bool ToObjectiveList( Common::MsgMissionObjectiveList& asn ) const;
    virtual bool ToObjectKnowledge( Common::ObjectKnowledgeId& asn ) const;
    virtual bool ToObjectKnowledgeList( Common::ObjectKnowledgeIdList& asn ) const;
    virtual bool ToPath( Common::MsgPath& asn ) const;
    virtual bool ToPathList( Common::MsgPathList& asn ) const;
    virtual bool ToPopulationKnowledge( Common::PopulationKnowledgeId& asn ) const;
    virtual bool ToUrbanBlock( Common::UrbanObjectId& ) const;

    virtual bool ToString( std::string& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToNatureAtlas( int& atlas ) const;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgent( DEC_Decision_ABC*& ) const;
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const;
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const;
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const;
    virtual bool ToGDH( float& value ) const;
//    virtual bool ToNumeric( float& value ) const;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const;
    virtual bool ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& ) const;

    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation );
    virtual void Append( boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject );
    virtual void Append( boost::shared_ptr< DEC_Gen_Object > pKnowledgeObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_BaseParameter( const MIL_BaseParameter& );            //!< Copy constructor
    MIL_BaseParameter& operator=( const MIL_BaseParameter& ); //!< Assignment operator
    //@}
};

#endif // __MIL_BaseParameter_h_
