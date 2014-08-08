// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_LEGACY_ROLE_PION_DECISION_H
#define SWORD_LEGACY_ROLE_PION_DECISION_H

#include "simulation_kernel/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include <boost/serialization/export.hpp>

namespace sword
{
namespace legacy
{
// =============================================================================
/** @class  RolePion_Decision
    @brief  Role pion Decision
*/
// Created: SLI 2012-02-01
// =============================================================================
class RolePion_Decision : public DEC_RolePion_Decision
{
public:
    //! @name Constructors/Destructor
    //@{
             RolePion_Decision( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult, DEC_Logger* logger );
    virtual ~RolePion_Decision();
    //@}

    //! @name Checkpoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive > void load( Archive&, const unsigned int );
    template< typename Archive > void save( Archive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Reload( bool force, bool doInitBrain );
    //@}

private:
    //! @name Helpers
    //@{
    void RegisterFunctions();
    void RegisterControlActions();
    void RegisterActions();
    void RegisterPath();
    void RegisterRepresentations();
    void RegisterPerception();
    void RegisterFire();
    void RegisterKnowledge();
    void RegisterAgentKnowledgeFunctions();
    //@}

    //! @name Checkpoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const sword::legacy::RolePion_Decision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, sword::legacy::RolePion_Decision* role, const unsigned int /*version*/ );
    //@}
};

}
}

BOOST_CLASS_EXPORT_KEY( sword::legacy::RolePion_Decision )

#endif // SWORD_LEGACY_ROLE_PION_DECISION_H
