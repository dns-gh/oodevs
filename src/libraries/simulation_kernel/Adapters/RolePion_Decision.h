// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_PION_DECISION_H
#define SWORD_ROLE_PION_DECISION_H

#include "simulation_kernel/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"

namespace core
{
    class Model;
}

namespace sword
{
    class Sink;

// =============================================================================
/** @class  RolePion_Decision
    @brief  Role Pion Decision
*/
// Created: SLI 2012-02-01
// =============================================================================
class RolePion_Decision : public DEC_RolePion_Decision
{
public:
    //! @name Constructors/Destructor
    //@{
             RolePion_Decision( MIL_AgentPion& pion, const core::Model& model, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger_ABC* logger, Sink& sink );
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
    virtual void Reload();
    //@}

private:
    //! @name Helpers
    //@{
    void RegisterFunctions();
    void RegisterControlActions();
    void RegisterActions();
    void RegisterPath();
    void RegisterRepresentations();
    void RegisterRepPoints();
    void RegisterItinerary();
    void RegisterPerception();
    void RegisterFire();
    void RegisterKnowledge();
    void RegisterAgentKnowledgeFunctions();

    boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const double time, bool bBoundOnPath ) const;
    boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const double time, bool bBoundOnPath ) const;

    template< typename Result, typename Function >
    void RegisterCommand( const char* const name, Function fun );
    template< typename Result, typename Function, typename Arg1 >
    void RegisterCommand( const char* const name, Function fun, Arg1 arg1 );
    template< typename Result, typename Function, typename Arg1, typename Arg2 >
    void RegisterCommand( const char* const name, Function fun, Arg1 arg1, Arg2 arg2 );
    template< typename Result, typename Function, typename Arg1, typename Arg2, typename Arg3 >
    void RegisterCommand( const char* const name, Function fun, Arg1 arg1, Arg2 arg2, Arg3 arg3 );
    //@}

    //! @name Checkpoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const sword::RolePion_Decision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, sword::RolePion_Decision* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    Sink& sink_;
    const core::Model& model_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::RolePion_Decision )

#endif // SWORD_ROLE_PION_DECISION_H
