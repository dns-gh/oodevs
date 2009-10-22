// *****************************************************************************
//
// $Created: NLD 2004-03-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_Def.h $
// $Author: Nld $
// $Modtime: 27/01/05 17:53 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_Def.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Def_h_
#define __DEC_Knowledge_Def_h_

#include "DEC_Knowledge_AgentComposante.h"

class DEC_Knowledge_Agent;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_Object;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationCollision;
class PHY_Volume;

//! @name Types
//@{
typedef std::vector< DEC_Knowledge_AgentPerception* >     T_KnowledgeAgentPerceptionVector;
typedef T_KnowledgeAgentPerceptionVector::iterator        IT_KnowledgeAgentPerceptionVector;
typedef T_KnowledgeAgentPerceptionVector::const_iterator  CIT_KnowledgeAgentPerceptionVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > T_KnowledgeAgentVector;
typedef T_KnowledgeAgentVector::iterator                        IT_KnowledgeAgentVector;
typedef T_KnowledgeAgentVector::const_iterator                  CIT_KnowledgeAgentVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > T_ConstKnowledgeAgentVector;
typedef T_ConstKnowledgeAgentVector::iterator                   IT_ConstKnowledgeAgentVector;
typedef T_ConstKnowledgeAgentVector::const_iterator             CIT_ConstKnowledgeAgentVector;

typedef std::vector< DEC_Knowledge_ObjectCollision* >     T_KnowledgeObjectCollisionVector;
typedef T_KnowledgeObjectCollisionVector::iterator        IT_KnowledgeObjectCollisionVector;
typedef T_KnowledgeObjectCollisionVector::const_iterator  CIT_KnowledgeObjectCollisionVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectVector;
typedef T_KnowledgeObjectVector::iterator                        IT_KnowledgeObjectVector;
typedef T_KnowledgeObjectVector::const_iterator                  CIT_KnowledgeObjectVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectDiaIDVector;
typedef T_KnowledgeObjectDiaIDVector::iterator                   IT_KnowledgeObjectDiaIDVector;
typedef T_KnowledgeObjectDiaIDVector::const_iterator             CIT_KnowledgeObjectDiaIDVector;

typedef std::vector< DEC_Knowledge_PopulationCollision* >     T_KnowledgePopulationCollisionVector;
typedef T_KnowledgePopulationCollisionVector::iterator        IT_KnowledgePopulationCollisionVector;
typedef T_KnowledgePopulationCollisionVector::const_iterator  CIT_KnowledgePopulationCollisionVector;

typedef std::vector< unsigned int >                       T_KnowledgePopulationDiaIDVector;
typedef T_KnowledgePopulationDiaIDVector::iterator        IT_KnowledgePopulationDiaIDVector;
typedef T_KnowledgePopulationDiaIDVector::const_iterator  CIT_KnowledgePopulationDiaIDVector;

typedef std::set< const PHY_Volume* >        T_ComposanteVolumeSet;
typedef T_ComposanteVolumeSet::const_iterator CIT_ComposanteVolumeSet;

typedef std::vector< DEC_Knowledge_AgentComposante >    T_KnowledgeComposanteVector;
typedef T_KnowledgeComposanteVector::iterator           IT_KnowledgeComposanteVector;
typedef T_KnowledgeComposanteVector::const_iterator     CIT_KnowledgeComposanteVector;

typedef std::vector< void* >                                   T_PopulationKnowledgeAgentDiaIDVector;
typedef T_PopulationKnowledgeAgentDiaIDVector::iterator        IT_PopulationKnowledgeAgentDiaIDVector;
typedef T_PopulationKnowledgeAgentDiaIDVector::const_iterator  CIT_PopulationKnowledgeAgentDiaIDVector;

typedef std::vector< DEC_Knowledge_Population* >     T_KnowledgePopulationVector;
typedef T_KnowledgePopulationVector::iterator        IT_KnowledgePopulationVector;
typedef T_KnowledgePopulationVector::const_iterator  CIT_KnowledgePopulationVector;

typedef std::vector< void* >                                   T_PopulationKnowledgeObjectDiaIDVector;
typedef T_PopulationKnowledgeObjectDiaIDVector::iterator       IT_PopulationKnowledgeObjectDiaIDVector;
typedef T_PopulationKnowledgeObjectDiaIDVector::const_iterator CIT_PopulationKnowledgeObjectDiaIDVector;
//@}

//! @name DIA Queries
//@{
enum E_QueryValidity
{
    eQueryValid,
    eQueryInvalid 
};
//@}

#endif // __DEC_Knowledge_Def_h_
