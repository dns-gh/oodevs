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
#include "game_asn/Simulation.h"

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
    virtual bool ToPoint( ASN1T_Point& ) const;
    virtual bool ToPointList( ASN1T_PointList& ) const;
    virtual bool ToPolygon( ASN1T_Polygon& ) const;
    virtual bool ToPolygonList( ASN1T_PolygonList& ) const;
    virtual bool ToAgent( ASN1T_Unit& ) const;
    virtual bool ToAgentKnowledge( ASN1T_UnitKnowledge& ) const;
    virtual bool ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const;
    virtual bool ToAgentList( ASN1T_UnitList& asn ) const;
    virtual bool ToAutomat( ASN1T_Automat& ) const;
    virtual bool ToAutomatList( ASN1T_AutomatList& ) const;
    virtual bool ToDirection( ASN1T_Heading& ) const;
    virtual bool ToDotationType( ASN1T_DotationType& ) const;
    virtual bool ToEquipmentType( ASN1T_EquipmentType& ) const;
    virtual bool ToGDH( ASN1T_DateTime& ) const;
    virtual bool ToGenObject( ASN1T_PlannedWork& ) const;
    virtual bool ToGenObjectList( ASN1T_PlannedWorkList& ) const;
    virtual bool ToIndirectFire( ASN1T_UnitFire& ) const;
    virtual bool ToLocation( ASN1T_Location& ) const;
    virtual bool ToLocationList( ASN1T_LocationList& ) const;
    virtual bool ToMaintenancePriorities( ASN1T_LogMaintenancePriorities& ) const;
    virtual bool ToMedicalPriorities( ASN1T_LogMedicalPriorities& ) const;
    virtual bool ToNatureAtlas( ASN1T_AtlasNature& diaTo ) const;
    virtual bool ToNumeric( ASN1REAL& ) const;
    virtual bool ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const;
    virtual bool ToObjectKnowledge( ASN1T_ObjectKnowledge& asn ) const;
    virtual bool ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const;
    virtual bool ToPath( ASN1T_Path& asn ) const;
    virtual bool ToPathList( ASN1T_PathList& asn ) const;
    virtual bool ToPopulationKnowledge( ASN1T_PopulationKnowledge& asn ) const;
    virtual bool ToString( ASN1VisibleString& asn ) const;

    virtual bool ToString( std::string& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
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
    virtual bool ToNumeric( float& value ) const;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const;
    virtual bool ToUrbanBlock( ASN1T_UrbanBlock& ) const;
    virtual bool ToUrbanBlock( urban::Block*& ) const;
    
    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_BaseParameter( const MIL_BaseParameter& );            //!< Copy constructor
    MIL_BaseParameter& operator=( const MIL_BaseParameter& ); //!< Assignment operator
    //@}
};

#endif // __MIL_BaseParameter_h_
