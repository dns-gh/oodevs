// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Object_h_
#define __MIL_Object_h_

#include "MIL_Object_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-15
//=============================================================================
class MIL_Object : public MIL_Object_ABC
{
public:
             MIL_Object();
             MIL_Object( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type );
             MIL_Object( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int id );
    virtual ~MIL_Object();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Construction
    //@{
    void UpdateLocalisation( const TER_Localisation& location );
    //@}

    //! @name InteractiveContainer
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    virtual void Register                ( MIL_InteractiveContainer_ABC* capacity );
    virtual void PreprocessAgent         ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    virtual void PreprocessPopulation    ( MIL_PopulationElement_ABC& population );
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population );
    //@}

    //! @name StructuralStateNotifier
    //@{
    virtual void Register( MIL_StructuralStateNotifier_ABC& notifier );
    virtual void ApplyStructuralState( float structuralState ) const;
    //@}

    //! @name Fires
    //@{
    virtual void ApplyIndirectFire( const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation, MIL_Army_ABC* army );
    virtual void ApplyDestruction( const TER_Localisation& attritionSurface, const PHY_UrbanAttritionData& dotation );
    virtual void ApplyDirectFire() const;
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

    //! @name Network
    //@{
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );
    virtual void UpdateState();
    virtual void SendCreation() const = 0;
    virtual void SendDestruction() const = 0;
    virtual void SendFullState() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetName() const = 0;
    //@}

    //! @name 
    //@{
    /*virtual*/ bool IsUniversal() const;
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
    typedef std::vector< MIL_InteractiveContainer_ABC* > T_InteractiveCapacities;
    typedef std::vector< MIL_StructuralStateNotifier_ABC* > T_StructuralStateNotifiers;
    typedef std::vector< ObjectAttribute_ABC* > T_Attributes;
    typedef T_Attributes::const_iterator      CIT_Attributes;
    typedef std::vector< ObjectCapacity_ABC* > T_Capacities;
    //@}

    //! @name Types
    //@{
    enum E_AttributeUpdate
    {
        eAttrUpdate_Localisation              = 0x10,
        eAttrUpdate_All                       = 0xFF
    };
    //@}

private:
    MIL_ObjectManipulator_ABC& manipulator_;
    T_InteractiveCapacities interactives_;
    T_StructuralStateNotifiers structuralStateNotifiers_;
    T_Capacities capacities_;
    T_Attributes attributes_;

    //! @name Network
    //@{
    mutable unsigned char xAttrToUpdate_;
    //@}
};

#endif // __MIL_Object_h_
