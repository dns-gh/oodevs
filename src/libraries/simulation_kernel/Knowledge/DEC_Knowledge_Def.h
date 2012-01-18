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
class DEC_Knowledge_Urban;
class DEC_ResourceNetwork;
class UrbanObjectWrapper;
class PHY_Volume;
class DEC_Knowledge_AgentComposante;

//! @name Types
//@{
typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > T_KnowledgeAgentVector;
typedef T_KnowledgeAgentVector::const_iterator                CIT_KnowledgeAgentVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > T_ConstKnowledgeAgentVector;
typedef T_ConstKnowledgeAgentVector::const_iterator           CIT_ConstKnowledgeAgentVector;

typedef std::vector< DEC_Knowledge_ObjectCollision* >       T_KnowledgeObjectCollisionVector;
typedef T_KnowledgeObjectCollisionVector::iterator         IT_KnowledgeObjectCollisionVector;
typedef T_KnowledgeObjectCollisionVector::const_iterator  CIT_KnowledgeObjectCollisionVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectVector;
typedef T_KnowledgeObjectVector::const_iterator                CIT_KnowledgeObjectVector;

typedef std::vector< UrbanObjectWrapper* >    T_UrbanObjectVector;
typedef T_UrbanObjectVector::const_iterator CIT_UrbanObjectVector;

typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectDiaIDVector;
typedef T_KnowledgeObjectDiaIDVector::const_iterator           CIT_KnowledgeObjectDiaIDVector;

typedef std::vector< DEC_Knowledge_PopulationCollision* >      T_KnowledgePopulationCollisionVector;
typedef T_KnowledgePopulationCollisionVector::const_iterator CIT_KnowledgePopulationCollisionVector;

typedef std::vector< unsigned int > T_KnowledgePopulationDiaIDVector;

typedef std::set< const PHY_Volume* >           T_ComposanteVolumeSet;
typedef T_ComposanteVolumeSet::const_iterator CIT_ComposanteVolumeSet;

typedef std::vector< DEC_Knowledge_AgentComposante >  T_KnowledgeComposanteVector;
typedef T_KnowledgeComposanteVector::const_iterator CIT_KnowledgeComposanteVector;

typedef std::vector< DEC_Knowledge_Population* >      T_KnowledgePopulationVector;
typedef T_KnowledgePopulationVector::const_iterator CIT_KnowledgePopulationVector;

typedef std::vector< boost::shared_ptr< DEC_ResourceNetwork > >       T_ResourceNetworkVector;
typedef T_ResourceNetworkVector::const_iterator                     CIT_ResourceNetworkVector;
//@}

#endif // __DEC_Knowledge_Def_h_
