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

class Team;
template< >
struct ListItemRtti< const Team* > {
    enum { rtti = 1001 };
};

class KnowledgeGroup;
template< >
struct ListItemRtti< const KnowledgeGroup* > {
    enum { rtti = 1002 };
};

class Agent;
template< >
struct ListItemRtti< const Agent* > {
    enum { rtti = 1003 };
};

class ObjectType;
template< >
struct ListItemRtti< const ObjectType* > {
    enum { rtti = 1004 };
};

class Object;
template< >
struct ListItemRtti< const Object* > {
    enum { rtti = 1005 };
};

class Population;
template< >
struct ListItemRtti< const Population* > {
    enum { rtti = 1006 };
};

class PopulationPart_ABC;
template< >
struct ListItemRtti< const PopulationPart_ABC* > {
    enum { rtti = 1007 };
};

class Dotation;
template< >
struct ListItemRtti< const Dotation* > {
    enum { rtti = 1008 };
};

class Equipment;
template< >
struct ListItemRtti< const Equipment* > {
    enum { rtti = 1009 };
};

class AgentKnowledge;
template< >
struct ListItemRtti< const AgentKnowledge* > {
    enum { rtti = 1010 };
};

class PopulationKnowledge;
template< >
struct ListItemRtti< const PopulationKnowledge* > {
    enum { rtti = 1011 };
};

class ObjectKnowledge;
template< >
struct ListItemRtti< const ObjectKnowledge* > {
    enum { rtti = 1012 };
};

class PopulationPartKnowledge_ABC;
template< >
struct ListItemRtti< const PopulationPartKnowledge_ABC* > {
    enum { rtti = 1013 };
};

class DotationRequest;
template< >
struct ListItemRtti< const DotationRequest* > {
    enum { rtti = 1014 };
};

class LogSupplyConsign;
template< >
struct ListItemRtti< const LogSupplyConsign* > {
    enum { rtti = 1015 };
};

class LogMaintenanceConsign;
template< >
struct ListItemRtti< const LogMaintenanceConsign* > {
    enum { rtti = 1016 };
};

class LogMedicalConsign;
template< >
struct ListItemRtti< const LogMedicalConsign* > {
    enum { rtti = 1017 };
};

class Report_ABC;
template< >
struct ListItemRtti< const Report_ABC* > {
    enum { rtti = 1018 };
};

class PopulationFireResult;
template< >
struct ListItemRtti< const PopulationFireResult* > {
    enum { rtti = 1019 };
};

class AgentFireResult;
template< >
struct ListItemRtti< const AgentFireResult* > {
    enum { rtti = 1020 };
};

class Lima;
template< >
struct ListItemRtti< const Lima* > {
    enum { rtti = 1021 };
};

class PopulationConcentration;
template< >
struct ListItemRtti< const PopulationConcentration* > {
    enum { rtti = 1022 };
};

class PopulationFlow;
template< >
struct ListItemRtti< const PopulationFlow* > {
    enum { rtti = 1023 };
};

class PopulationConcentrationKnowledge;
template< >
struct ListItemRtti< const PopulationConcentrationKnowledge* > {
    enum { rtti = 1024 };
};

class PopulationFlowKnowledge;
template< >
struct ListItemRtti< const PopulationFlowKnowledge* > {
    enum { rtti = 1025 };
};

struct sLoggerLayer;
template< >
struct ListItemRtti< const sLoggerLayer* > {
    enum { rtti = 1026 };
};

class NBCAgent;
template< >
struct ListItemRtti< const NBCAgent* > {
    enum { rtti = 1027 };
};

class ParamObstacle;
template< >
struct ListItemRtti< ParamObstacle* > {
    enum { rtti = 1028 };
};

template< typename T > class SafePointer;
template< typename T >
struct ListItemRtti< SafePointer< T > > : public ListItemRtti< const T* > 
{};

#endif // __ListItemRtti_h_
