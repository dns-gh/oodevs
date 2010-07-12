// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AgentHLA_h_
#define __MIL_AgentHLA_h_

#include "MIL_Agent_ABC.h"

namespace hla
{
    class Deserializer;
    class AttributeIdentifier;
    class ObjectIdentifier;
}

class HLA_InteractionManager_ABC;

// =============================================================================
/** @class  MIL_AgentHLA
    @brief  HLA agent
*/
// Created: AGE 2004-11-09
// =============================================================================
class MIL_AgentHLA : public MIL_Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentHLA( const hla::ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager );
    virtual ~MIL_AgentHLA();
    //@}

    //! @name Operations
    //@{
    virtual       MIL_Army_ABC&       GetArmy          () const;
    virtual       MIL_KnowledgeGroup& GetKnowledgeGroup() const;
    virtual const MIL_AgentType_ABC&  GetType          () const;
    virtual bool                      IsDead           () const;
    virtual bool                      IsNeutralized    () const;
    virtual bool                      IsPC             () const;
    virtual bool                      IsAutonomous     () const;
    virtual const AlgorithmsFactories& GetAlgorithms() const;

    virtual const MIL_Automate&       GetAutomate() const;
    virtual       MIL_Automate&       GetAutomate();

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;

    virtual const MIL_PionOrderManager& GetOrderManager() const;
    virtual MIL_PionOrderManager& GetOrderManager();

    virtual void ChangeSuperior( MIL_Automate& newAutomate );

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Operations
    //@{
    virtual void    NotifyAttackedBy( MIL_AgentPion& pion );
    virtual void    NotifyAttackedBy( MIL_Population& population );
    virtual void    MagicMove( const MT_Vector2D& );
    virtual double  Distance( const MIL_Agent_ABC& ) const;
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup );
    virtual bool                                     IsPerceived    ( const MIL_Agent_ABC& agent ) const;
    virtual DEC_KnowledgeBlackBoard_AgentPion&       GetKnowledge   () const;
    //@}

    //! @name Accessors
    //@{
    bool IsValid() const;

    virtual       DEC_Decision_ABC& GetDecision   ();
    virtual const DEC_Decision_ABC& GetDecision   () const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AgentHLA( const MIL_AgentHLA& );            //!< Copy constructor
    MIL_AgentHLA& operator=( const MIL_AgentHLA& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
          MIL_Army_ABC*      pArmy_;
    const MIL_AgentType_ABC* pType_;
          bool               bPc_;
    //@}
};

#endif // __MIL_AgentHLA_h_
