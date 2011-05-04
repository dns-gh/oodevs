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

#include "Entities/Orders/MIL_MissionParameter_ABC.h"

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
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type )const
    { throw; }
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const
    { throw; }
    virtual bool ToId( int& ) const
    { throw; }
    virtual bool ToPoint( sword::Point& ) const
    { throw; }
    virtual bool ToPointList( sword::PointList& ) const
    { throw; }
    virtual bool ToPolygon( sword::Polygon& ) const
    { throw; }
    virtual bool ToPolygonList( sword::PolygonList& ) const
    { throw; }
    virtual bool ToAgent( sword::UnitId& ) const
    { throw; }
    virtual bool ToAgentKnowledge( sword::UnitKnowledgeId& ) const
    { throw; }
    virtual bool ToAgentKnowledgeList( sword::UnitKnowledgeIdList& ) const
    { throw; }
    virtual bool ToAgentList( sword::UnitIdList& ) const
    { throw; }
    virtual bool ToAutomat( sword::AutomatId& ) const
    { throw; }
    virtual bool ToAutomatList( sword::AutomatIdList& ) const
    { throw; }
    virtual bool ToDirection( sword::Heading& ) const
    { throw; }
    virtual bool ToDotationType( sword::ResourceType& ) const
    { throw; }
    virtual bool ToEquipmentType( sword::EquipmentType& ) const
    { throw; }
    virtual bool ToGDH( sword::DateTime& ) const
    { throw; }
    virtual bool ToGenObject( sword::PlannedWork& ) const
    { throw; }
    virtual bool ToGenObjectList( sword::PlannedWorkList& ) const
    { throw; }
    virtual bool ToIndirectFire( sword::FireId& ) const
    { throw; }
    virtual bool ToLocation( sword::Location& ) const
    { throw; }
    virtual bool ToLocationList( sword::LocationList& ) const
    { throw; }
    virtual bool ToMaintenancePriorities( sword::LogMaintenancePriorities& ) const
    { throw; }
    virtual bool ToMedicalPriorities( sword::LogMedicalPriorities& ) const
    { throw; }
    virtual bool ToNatureAtlas( sword::Nature& ) const
    { throw; }
    virtual bool ToNumeric( float& ) const
    { throw; }
    virtual bool ToNumericList( std::vector< float >& ) const
    { throw; }
    virtual bool ToObjectiveList( sword::MissionObjectiveList& ) const
    { throw; }
    virtual bool ToObjectKnowledge( sword::ObjectKnowledgeId& ) const
    { throw; }
    virtual bool ToObjectKnowledgeList( sword::ObjectKnowledgeIdList& ) const
    { throw; }
    virtual bool ToPath( sword::Path& ) const
    { throw; }
    virtual bool ToPathList( sword::PathList& ) const
    { throw; }
    virtual bool ToPopulationKnowledge( sword::CrowdKnowledgeId& ) const
    { throw; }
    virtual bool ToString( std::string& result ) const
    {
        result = param_;
        return true;
    }
    virtual bool ToLima( sword::PhaseLinesOrder& ) const
    { throw; }
    virtual bool ToLimaList( sword::PhaseLinesOrder& ) const
    { throw; }
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
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const
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
    virtual bool ToUrbanBlock( sword::UrbanObjectId& ) const
    { throw; }
    virtual bool ToUrbanBlock( UrbanObjectWrapper*& ) const
    { throw; }
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const
    { throw; }
    virtual bool ToElement( sword::MissionParameter_Value& ) const
    { throw; }
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const
    { throw; }
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const
    { throw; }
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
    { throw; }
    virtual bool ToResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork >& ) const
    { throw; }
    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param )
    { throw; }
    //@}

private:
    std::string param_;
};

#endif // __StubMIL_MissionParameter_ABC_h_
