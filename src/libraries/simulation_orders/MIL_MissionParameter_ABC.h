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
#include "game_asn/Simulation.h"

class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_Objective;
class MIL_ParameterType_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;
class PHY_ComposanteTypePion;
class PHY_HumanWound;
class TER_Localisation;

typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;
typedef std::vector< const PHY_HumanWound* >     T_MedicalPriorityVector;

// =============================================================================
/** @class  MIL_MissionParameter_ABC
    @brief  MIL_MissionParameter_ABC
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

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& )const = 0;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const = 0;
    virtual bool ToId( int& ) const = 0;
    virtual bool ToPoint( ASN1T_Point& ) const = 0;
    virtual bool ToPointList( ASN1T_PointList& ) const = 0;
    virtual bool ToPolygon( ASN1T_Polygon& ) const = 0;
    virtual bool ToPolygonList( ASN1T_PolygonList& ) const = 0;
    virtual bool ToAgent( ASN1T_Unit& ) const = 0;
    virtual bool ToAgentKnowledge( ASN1T_UnitKnowledge& ) const = 0;
    virtual bool ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const = 0;
    virtual bool ToAgentList( ASN1T_UnitList& asn ) const = 0;
    virtual bool ToAutomat( ASN1T_Automat& ) const = 0;
    virtual bool ToAutomatList( ASN1T_AutomatList& ) const = 0;
    virtual bool ToDirection( ASN1T_Heading& ) const = 0;
    virtual bool ToDotationType( ASN1T_DotationType& ) const = 0;
    virtual bool ToEquipmentType( ASN1T_EquipmentType& ) const = 0;
    virtual bool ToGDH( ASN1T_DateTime& ) const = 0;
    virtual bool ToGenObject( ASN1T_PlannedWork& ) const = 0;
    virtual bool ToGenObjectList( ASN1T_PlannedWorkList& ) const = 0;
    virtual bool ToIndirectFire( ASN1T_UnitFire& ) const = 0;
    virtual bool ToLocation( ASN1T_Location& ) const = 0;
    virtual bool ToLocationList( ASN1T_LocationList& ) const = 0;
    virtual bool ToMaintenancePriorities( ASN1T_LogMaintenancePriorities& ) const = 0;
    virtual bool ToMedicalPriorities( ASN1T_LogMedicalPriorities& ) const = 0;
    virtual bool ToNatureAtlas( ASN1T_AtlasNature& diaTo ) const = 0;
    virtual bool ToNumeric( ASN1REAL& ) const = 0;
    virtual bool ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const = 0;
    virtual bool ToObjectKnowledge( ASN1T_ObjectKnowledge& asn ) const = 0;
    virtual bool ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const = 0;
    virtual bool ToPath( ASN1T_Path& asn ) const = 0;
    virtual bool ToPathList( ASN1T_PathList& asn ) const = 0;
    virtual bool ToPopulationKnowledge( ASN1T_PopulationKnowledge& asn ) const = 0;
    virtual bool ToString( ASN1VisibleString& asn ) const = 0;

    virtual bool ToString( std::string& ) const = 0;
    // The lifecycle of pointers in all functions below must be handled by MIL_MissionParameter_ABC
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
    virtual bool ToAgentKnowledge( DEC_Knowledge_Agent*& ) const = 0;
    virtual bool ToAgentKnowledgeList( std::vector< DEC_Knowledge_Agent* >& ) const = 0;
    virtual bool ToObjectKnowledge( DEC_Knowledge_Object*& ) const = 0;
    virtual bool ToObjectKnowledgeList( std::vector< DEC_Knowledge_Object* >& ) const = 0;
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population* ) const = 0;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const = 0;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const = 0;
    virtual bool ToGDH( float& value ) const = 0;
    virtual bool ToNumeric( float& value ) const = 0;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const = 0;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const = 0;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const = 0;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const = 0;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const = 0;

    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation ) = 0;
    //@}
};

#endif // __MIL_MissionParameter_ABC_h_
