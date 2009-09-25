// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_MissionParameterFactory_h_
#define __MIL_MissionParameterFactory_h_

#include <boost/shared_ptr.hpp>
#include "game_asn/generated/Order.h"

class DEC_AutomateDecision;
class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_RolePion_Decision;
class MIL_Mission_ABC;
class MIL_MissionParameter_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;

// =============================================================================
/** @class  MIL_MissionParameterFactory
    @brief  MIL_MissionParameterFactory
    Factory, creates appropriate MIL_MissionParameter_ABC with new.
*/
// Created: LDC 2009-04-29
// =============================================================================
class MIL_MissionParameterFactory
{

public:
    //! @name Operations
    //@{
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const ASN1T_MissionParameter& asn, const DEC_KnowledgeResolver_ABC& resolver );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_Knowledge_Agent* agentKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const PHY_DotationCategory* dotationType );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const PHY_ComposanteTypePion* equipmentType );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( float param );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( int id );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_Knowledge_Object* objectKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_RolePion_Decision* pion );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_AutomateDecision* automate );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_Knowledge_Population* populationKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const std::string& message );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateTir( int id );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const MT_Vector2D& point );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr< TER_Localisation > pLocation );

    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePath( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePath( boost::shared_ptr< MT_Vector2D > point );

    static void SetPawnParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Decision_ABC* pion );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePawn( DEC_Decision_ABC* pion );

    static void SetAutomatParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Decision_ABC* automat );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateAutomat( DEC_Decision_ABC* automat );

    static void SetBoolParameter( MIL_Mission_ABC* pMission, const std::string& parameter, bool value );
    static void SetObjectKnowledgeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Knowledge_Object* objectKnowledge );
    static void SetNatureAtlasTypeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, int natureAtlas );
    static void SetAutomatListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& automatList );
    static void SetAgentKnowledgeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Knowledge_Agent* agentKnowledge );
    static void SetPathListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& );
    static void SetEnumereParameter( MIL_Mission_ABC* pMission, const std::string& parameter, int value );
    static void SetLocationParameter( MIL_Mission_ABC* pMission, const std::string& parameter, boost::shared_ptr< TER_Localisation > pLocation );
    static void SetObjectKnowledgeListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Knowledge_Object* >& objectKnowledgeList );
    static void SetPointParameter( MIL_Mission_ABC* pMission, const std::string& parameter, MT_Vector2D& point );
    static void SetAgentKnowledgeListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Knowledge_Agent* >& agentKnowledgeList );
    static void SetGenObjectListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Gen_Object > >& list );
    static void SetPionListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& pionList );
    static void SetLocationListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< TER_Localisation > >& locationList );    

    static void Copy( const ASN1T_MissionParameters& asn, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __MIL_MissionParameterFactory_h_
