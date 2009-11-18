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
    virtual bool ToPoint( ASN1T_Point& ) const
    { throw; }
    virtual bool ToPointList( ASN1T_PointList& ) const
    { throw; }
    virtual bool ToPolygon( ASN1T_Polygon& ) const
    { throw; }
    virtual bool ToPolygonList( ASN1T_PolygonList& ) const
    { throw; }
    virtual bool ToAgent( ASN1T_Unit& ) const
    { throw; }
    virtual bool ToAgentKnowledge( ASN1T_UnitKnowledge& ) const
    { throw; }
    virtual bool ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const
    { throw; }
    virtual bool ToAgentList( ASN1T_UnitList& asn ) const
    { throw; }
    virtual bool ToAutomat( ASN1T_Automat& ) const
    { throw; }
    virtual bool ToAutomatList( ASN1T_AutomatList& ) const
    { throw; }
    virtual bool ToDirection( ASN1T_Heading& ) const
    { throw; }
    virtual bool ToDotationType( ASN1T_DotationType& ) const
    { throw; }
    virtual bool ToEquipmentType( ASN1T_EquipmentType& ) const
    { throw; }
    virtual bool ToGDH( ASN1T_DateTime& ) const
    { throw; }
    virtual bool ToGenObject( ASN1T_PlannedWork& ) const
    { throw; }
    virtual bool ToGenObjectList( ASN1T_PlannedWorkList& ) const
    { throw; }
    virtual bool ToIndirectFire( ASN1T_UnitFire& ) const
    { throw; }
    virtual bool ToLocation( ASN1T_Location& ) const
    { throw; }
    virtual bool ToLocationList( ASN1T_LocationList& ) const
    { throw; }
    virtual bool ToMaintenancePriorities( ASN1T_LogMaintenancePriorities& ) const
    { throw; }
    virtual bool ToMedicalPriorities( ASN1T_LogMedicalPriorities& ) const
    { throw; }
    virtual bool ToNatureAtlas( ASN1T_AtlasNature& diaTo ) const
    { throw; }
    virtual bool ToNumeric( ASN1REAL& ) const
    { throw; }
    virtual bool ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const
    { throw; }
    virtual bool ToObjectKnowledge( ASN1T_ObjectKnowledge& asn ) const
    { throw; }
    virtual bool ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const
    { throw; }
    virtual bool ToPath( ASN1T_Path& asn ) const
    { throw; }
    virtual bool ToPathList( ASN1T_PathList& asn ) const
    { throw; }
    virtual bool ToPopulationKnowledge( ASN1T_PopulationKnowledge& asn ) const
    { throw; }
    virtual bool ToString( ASN1VisibleString& asn ) const
    { throw; }

    virtual bool ToString( std::string& result ) const
    { 
        result = param_;
        return true;
    }
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const
    { throw; }
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw; }
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const
    { throw; }
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const
    { throw; }
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const const
    { throw; }
    virtual bool ToLocationList( std::vector< boost::shared_ptr<TER_Localisation > >& ) const
    { throw; }
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const
    { throw; }
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const
    { throw; }
    virtual bool ToNatureAtlas( int& atlas ) const
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
    virtual bool ToGDH( float& value ) const
    { throw; }
    virtual bool ToNumeric( float& value ) const
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
    virtual bool ToUrbanBlock( ASN1T_UrbanBlock& ) const
    { throw; }
    virtual bool ToUrbanBlock( urban::Block*& ) const
    { throw; }
    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation )
    { throw; }
    //@}
    
private:
    std::string param_;
};

#endif // __StubMIL_MissionParameter_ABC_h_
