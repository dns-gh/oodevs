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

#include <tools/Exception.h>
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
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToId( int& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPoint( sword::Point& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPointList( sword::PointList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPolygon( sword::Polygon& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPolygonList( sword::PolygonList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgent( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentKnowledge( sword::UnitKnowledgeId& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentKnowledgeList( sword::IdList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentList( sword::IdList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAutomat( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAutomatList( sword::IdList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToDirection( sword::Heading& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToDotationType( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToEquipmentType( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGDH( sword::DateTime& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGenObject( sword::PlannedWork& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGenObjectList( sword::PlannedWorkList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToIndirectFire( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLocation( sword::Location& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLocationList( sword::LocationList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToMaintenancePriorities( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToMedicalPriorities( sword::LogMedicalPriorities& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToNatureAtlas( sword::Nature& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToNumeric( float& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToNumericList( std::vector< float >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectiveList( sword::MissionObjectiveList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectKnowledge( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectKnowledgeList( sword::IdList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPath( sword::Path& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPathList( sword::PathList& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPopulationKnowledge( sword::Id& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToString( std::string& result ) const
    {
        result = param_;
        return true;
    }
    virtual bool ToLima( sword::PhaseLinesOrder& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLimaList( sword::PhaseLinesOrder& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLocationList( std::vector< boost::shared_ptr<TER_Localisation > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToNatureAtlas( int& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgent( const DEC_Decision_ABC*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentList( std::vector< const DEC_Decision_ABC* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToPopulationKnowledge( boost::shared_ptr< DEC_Knowledge_Population >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToEquipmentTypeList( std::vector< const PHY_ComposanteTypePion* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGDH( float& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGenObject( boost::shared_ptr<DEC_Gen_Object >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToUrbanBlock( MIL_UrbanObject_ABC*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToUrbanBlockList( std::vector< MIL_UrbanObject_ABC* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToElement( sword::MissionParameter_Value& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToResourceNetworkNodeList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToResourceNetworkType( const PHY_ResourceNetworkType*& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual bool ToResourceNetworkTypeList( std::vector< const PHY_ResourceNetworkType* >& ) const
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param )
    { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    //@}

private:
    std::string param_;
};

#endif // __StubMIL_MissionParameter_ABC_h_
