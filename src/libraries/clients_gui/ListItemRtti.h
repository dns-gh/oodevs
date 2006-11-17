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

#include "boost/preprocessor/inc.hpp"

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
    class PopulationType;
    class NBCAgent;
    class Entity_ABC;
    class Automat_ABC;
    class AutomatComposition;
}

class Dotation;
class Equipment;
class AgentKnowledge_ABC;
class PopulationKnowledge_ABC;
class ObjectKnowledge_ABC;
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

namespace detail {
    template< unsigned Value >
    struct EnsureUnique;
}

#define DECLARE_RTTI( Type, Value )     \
template< >                             \
struct ListItemRtti< Type* > {          \
    enum { rtti = 1000 + Value };       \
};                                      \
namespace detail {                      \
    template< >                         \
    struct EnsureUnique< Value > {};    \
}

DECLARE_RTTI( const kernel::Team_ABC                     , 1  )
DECLARE_RTTI( const kernel::KnowledgeGroup_ABC           , 2  )
DECLARE_RTTI( const kernel::Agent_ABC                    , 3  )
DECLARE_RTTI( const kernel::ObjectType                   , 4  )
DECLARE_RTTI( const kernel::Object_ABC                   , 5  )
DECLARE_RTTI( const kernel::Population_ABC               , 6  )
DECLARE_RTTI( const kernel::PopulationFlow_ABC           , 7  )
DECLARE_RTTI( const Dotation                             , 8  )
DECLARE_RTTI( const Equipment                            , 9  )
DECLARE_RTTI( const AgentKnowledge_ABC                   , 10 )
DECLARE_RTTI( const PopulationKnowledge_ABC              , 11 )
DECLARE_RTTI( const ObjectKnowledge_ABC                  , 12 )
DECLARE_RTTI( const PopulationPartKnowledge_ABC          , 13 )
DECLARE_RTTI( const DotationRequest                      , 14 )
DECLARE_RTTI( const LogSupplyConsign                     , 15 )
DECLARE_RTTI( const LogMaintenanceConsign                , 16 )
DECLARE_RTTI( const LogMedicalConsign                    , 17 )
DECLARE_RTTI( const Report_ABC                           , 18 )
DECLARE_RTTI( const PopulationFireResult                 , 19 )
DECLARE_RTTI( const AgentFireResult                      , 20 )
DECLARE_RTTI( const Lima                                 , 21 )
DECLARE_RTTI( const PopulationConcentration              , 22 )
DECLARE_RTTI( const PopulationFlow                       , 23 )
DECLARE_RTTI( const PopulationConcentrationKnowledge     , 24 )
DECLARE_RTTI( const PopulationFlowKnowledge              , 25 )
DECLARE_RTTI( const kernel::NBCAgent                     , 26 )
DECLARE_RTTI(       ParamObstacle                        , 27 )
DECLARE_RTTI(       ParamPath                            , 28 )
DECLARE_RTTI( const kernel::PopulationConcentration_ABC  , 29 )
DECLARE_RTTI( const kernel::AgentType                    , 30 )
DECLARE_RTTI( const kernel::AutomatType                  , 31 )
DECLARE_RTTI( const kernel::PopulationType               , 32 )
DECLARE_RTTI( const kernel::Entity_ABC                   , 33 )
DECLARE_RTTI( const kernel::AutomatComposition           , 34 )
DECLARE_RTTI( const kernel::Automat_ABC                  , 35 )

}

#endif // __ListItemRtti_h_
