// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObjectWrapper_h_
#define __UrbanObjectWrapper_h_

#include "MIL_Object_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  UrbanObjectWrapper
    @brief  UrbanObjectWrapper
*/
// Created: SLG 2010-06-18
// =============================================================================
class UrbanObjectWrapper : public MIL_Object_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanObjectWrapper( urban::TerrainObject_ABC& object );
    virtual ~UrbanObjectWrapper();
    //@}

    /*//! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}*/

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Interaction
    //@{
    virtual void Register        ( MIL_InteractiveContainer_ABC* capacity );
    void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    void ProcessPopulationInside ( MIL_PopulationElement_ABC& population );
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team );
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group );
    //@}

    //! @name Manipulator
    //@{
    virtual const MIL_ObjectManipulator_ABC& operator()() const;
    virtual       MIL_ObjectManipulator_ABC& operator()();
    //@}

    //! @name HLA
    //@{
    virtual HLA_Object_ABC* GetHLAView() const;
    virtual void            SetHLAView( HLA_Object_ABC& view );
    virtual void    Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    virtual void    Serialize  ( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Network
    //@{
    virtual MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode OnUpdate( const Common::MsgMissionParameter_Value& asn );
    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    //@}

    //! @name Accessors
    //@{    
    virtual unsigned int              GetID() const;
    urban::TerrainObject_ABC&         GetObject();
    //@}

protected:
    //! @name Tools
    //@{
    virtual void Update( unsigned int time );
    virtual void Register( ObjectAttribute_ABC* attribute );
    virtual void Register( ObjectCapacity_ABC* capacity );
    //@}

private: 
    urban::TerrainObject_ABC& object_;
    MIL_ObjectManipulator_ABC& manipulator_;
};

/*
BOOST_CLASS_EXPORT_KEY( Object )
*/
#endif