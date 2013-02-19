// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Def_h_
#define __DEC_Knowledge_Def_h_

class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationCollision;
class DEC_ResourceNetwork;
class MIL_UrbanObject_ABC;
class DEC_Knowledge_AgentComposante;

//! @name Types
//@{
typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >  T_KnowledgeAgentVector;
typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >  T_ConstKnowledgeAgentVector;
typedef std::vector< DEC_Knowledge_ObjectCollision* >            T_KnowledgeObjectCollisionVector;
typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectVector;
typedef std::vector< const MIL_UrbanObject_ABC* >                T_UrbanObjectVector;
typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectDiaIDVector;
typedef std::vector< DEC_Knowledge_PopulationCollision* >        T_KnowledgePopulationCollisionVector;
typedef std::vector< unsigned int >                              T_KnowledgePopulationDiaIDVector;
typedef std::vector< DEC_Knowledge_AgentComposante >             T_KnowledgeComposanteVector;
typedef std::vector< DEC_Knowledge_Population* >                 T_KnowledgePopulationVector;
typedef std::vector< boost::shared_ptr< DEC_ResourceNetwork > >  T_ResourceNetworkVector;
//@}

#endif // __DEC_Knowledge_Def_h_
