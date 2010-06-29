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

class MIL_ObjectBuilder_ABC;

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
    explicit UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object );
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
    void UpdateCapacities( const std::string& capacity, xml::xistream& xis );
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

    //! @name Instanciate / Build / Copy object
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const; //<! create and register every prototyped capacity
    virtual void Finalize(); //<! finalize capacity instanciation : for instance once the object location has been defined
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
    virtual void UpdateState();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int              GetID() const;
    const urban::TerrainObject_ABC&   GetObject();
    const urban::TerrainObject_ABC&   GetObject() const;
    virtual unsigned int  GetMaterial() const;
    //@}

protected:
    //! @name Tools
    //@{
    virtual void Update( unsigned int time );
    virtual void Register( ObjectAttribute_ABC* attribute );
    virtual void Register( ObjectCapacity_ABC* capacity );
    //@}

private:
    //! @name Types containers
    //@{
    typedef std::vector< ObjectCapacity_ABC* >              T_Capacities;
    typedef std::vector< MIL_InteractiveContainer_ABC* >    T_InteractiveCapacities;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC& object_;
    unsigned int                    id_;
    MIL_ObjectManipulator_ABC& manipulator_;
    T_Capacities            capacities_;
    T_InteractiveCapacities interactives_;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* pView_;
    //@}
};

/*
BOOST_CLASS_EXPORT_KEY( Object )
*/
#endif