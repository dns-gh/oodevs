// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemRtti_h_
#define __ListItemRtti_h_

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Agent_ABC;
    class AgentType;
    class AutomatType;
    class ObjectType;
    class Object_ABC;
    class Population_ABC;
    class PopulationFlow_ABC;
    class PopulationConcentration_ABC;
    class NBCAgent;
}

class Dotation;
class Equipment;
class AgentKnowledge;
class PopulationKnowledge;
class ObjectKnowledge;
class PopulationPartKnowledge_ABC;
class DotationRequest;
class LogSupplyConsign;
class LogMaintenanceConsign;
class LogMedicalConsign;
class Report_ABC;
class PopulationFireResult;
class AgentFireResult;
class Lima;
class PopulationConcentration;
class PopulationFlow;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class ParamObstacle;
class ParamPath;

namespace gui
{

// =============================================================================
/** @class  ListItemRtti
    @brief  ListItemRtti
*/
// Created: AGE 2006-02-15
// =============================================================================
template< typename T >
struct ListItemRtti
{
    // NOTHING
};

template< >
struct ListItemRtti< const kernel::Team_ABC* > {
    enum { rtti = 1001 };
};

template< >
struct ListItemRtti< const kernel::KnowledgeGroup_ABC* > {
    enum { rtti = 1002 };
};

template< >
struct ListItemRtti< const kernel::Agent_ABC* > {
    enum { rtti = 1003 };
};

template< >
struct ListItemRtti< const kernel::ObjectType* > {
    enum { rtti = 1004 };
};

template< >
struct ListItemRtti< const kernel::Object_ABC* > {
    enum { rtti = 1005 };
};

template< >
struct ListItemRtti< const kernel::Population_ABC* > {
    enum { rtti = 1006 };
};

template< >
struct ListItemRtti< const kernel::PopulationFlow_ABC* > {
    enum { rtti = 1007 };
};

template< >
struct ListItemRtti< const Dotation* > {
    enum { rtti = 1008 };
};

template< >
struct ListItemRtti< const Equipment* > {
    enum { rtti = 1009 };
};

template< >
struct ListItemRtti< const AgentKnowledge* > {
    enum { rtti = 1010 };
};

template< >
struct ListItemRtti< const PopulationKnowledge* > {
    enum { rtti = 1011 };
};

template< >
struct ListItemRtti< const ObjectKnowledge* > {
    enum { rtti = 1012 };
};

template< >
struct ListItemRtti< const PopulationPartKnowledge_ABC* > {
    enum { rtti = 1013 };
};

template< >
struct ListItemRtti< const DotationRequest* > {
    enum { rtti = 1014 };
};

template< >
struct ListItemRtti< const LogSupplyConsign* > {
    enum { rtti = 1015 };
};

template< >
struct ListItemRtti< const LogMaintenanceConsign* > {
    enum { rtti = 1016 };
};

template< >
struct ListItemRtti< const LogMedicalConsign* > {
    enum { rtti = 1017 };
};

template< >
struct ListItemRtti< const Report_ABC* > {
    enum { rtti = 1018 };
};

template< >
struct ListItemRtti< const PopulationFireResult* > {
    enum { rtti = 1019 };
};

template< >
struct ListItemRtti< const AgentFireResult* > {
    enum { rtti = 1020 };
};

template< >
struct ListItemRtti< const Lima* > {
    enum { rtti = 1021 };
};

template< >
struct ListItemRtti< const PopulationConcentration* > {
    enum { rtti = 1022 };
};

template< >
struct ListItemRtti< const PopulationFlow* > {
    enum { rtti = 1023 };
};

template< >
struct ListItemRtti< const PopulationConcentrationKnowledge* > {
    enum { rtti = 1024 };
};

template< >
struct ListItemRtti< const PopulationFlowKnowledge* > {
    enum { rtti = 1025 };
};

template< >
struct ListItemRtti< const kernel::NBCAgent* > {
    enum { rtti = 1026 };
};

template< >
struct ListItemRtti< ParamObstacle* > {
    enum { rtti = 1027 };
};

template< >
struct ListItemRtti< ParamPath* > {
    enum { rtti = 1028 };
};

template< >
struct ListItemRtti< const kernel::PopulationConcentration_ABC* > {
    enum { rtti = 1029 };
};

template< >
struct ListItemRtti< const kernel::AgentType* > {
    enum { rtti = 1030 };
};

template< >
struct ListItemRtti< const kernel::AutomatType* > {
    enum { rtti = 1031 };
};


}

#endif // __ListItemRtti_h_
