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

namespace sword
{
    class MissionParameter;
    class MissionParameter_Value;
    class MissionParameters;
}

class DEC_AutomateDecision;
class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_KnowledgeResolver_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_ResourceNetwork;
class DEC_RolePion_Decision;
class MIL_Mission_ABC;
class MIL_MissionParameter_ABC;
class MIL_OrderType_ABC;
class MIL_OrderTypeParameter;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;
class TER_Localisation;
class UrbanObjectWrapper;

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
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const MIL_OrderTypeParameter& type );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const MIL_OrderTypeParameter& type, const sword::MissionParameter& asn, const DEC_KnowledgeResolver_ABC& resolver );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const sword::MissionParameter_Value& message, const DEC_KnowledgeResolver_ABC& resolver );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const PHY_DotationCategory* dotationType );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const PHY_ComposanteTypePion* equipmentType );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( float param );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( int id );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateInteger( int id );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_RolePion_Decision* pion );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_AutomateDecision* automate );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( DEC_Knowledge_Population* populationKnowledge );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const std::string& message );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateTir( int id );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( const MT_Vector2D& point );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr< TER_Localisation > pLocation );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( UrbanObjectWrapper* pUrbanblock );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePath( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePath( boost::shared_ptr< MT_Vector2D > point );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateDirection( boost::shared_ptr< MT_Vector2D > direction );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr< DEC_Gen_Object > param );
    static boost::shared_ptr<MIL_MissionParameter_ABC> Create( boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork );
    static void SetPawnParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* pion );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreatePawn( DEC_Decision_ABC* pion );

//    static void Copy( const MissionParameters& asn, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters, const DEC_KnowledgeResolver_ABC& resolver );

    static void SetAutomatParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* automat );
    static boost::shared_ptr<MIL_MissionParameter_ABC> CreateAutomat( DEC_Decision_ABC* automat );

    static void SetBoolParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, bool value );
    static void SetObjectKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static void SetNatureAtlasTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int natureAtlas );
    static void SetAutomatListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& automatList );
    static void SetAgentKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    static void SetPathListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& );
    static void SetEnumereParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int value );
    static void SetLocationParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< TER_Localisation > pLocation );
    static void SetObjectKnowledgeListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& objectKnowledgeList );
    static void SetPointParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, MT_Vector2D& point );
    static void SetAgentKnowledgeListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& agentKnowledgeList );
    static void SetGenObjectParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Gen_Object > object );
    static void SetGenObjectListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Gen_Object > >& list );
    static void SetPionListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& pionList );
    static void SetLocationListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< TER_Localisation > >& locationList );
    static void SetPointListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    static void SetPolygonListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< TER_Localisation > >& locationList );
    static void SetUrbanBlockParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, UrbanObjectWrapper* pUrbanblock );
    static void SetUrbanBlockListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< UrbanObjectWrapper* >& pUrbanBlockList );
    static void SetDirectionParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< MT_Vector2D > direction );
    static void SetDotationTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const PHY_DotationCategory* type );
    static void SetNumericTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, float value );
    static void SetResourceNetworkParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork );
    static void SetResourceNetworkListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr<class DEC_ResourceNetwork> >& resourceNetworkList );

    static void Copy( const MIL_OrderType_ABC& orderType, const sword::MissionParameters& asn, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const DEC_KnowledgeResolver_ABC& resolver );

    //@}
};

#endif // __MIL_MissionParameterFactory_h_
