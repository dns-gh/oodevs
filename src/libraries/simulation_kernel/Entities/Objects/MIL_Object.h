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
             MIL_Object( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned long forcedId );
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
    virtual void UpdateLocalisation( const TER_Localisation& location );
    //@}

    //! @name InteractiveContainer
    //@{
    virtual bool CanInteractWith         ( const MIL_Agent_ABC& agent ) const;
    virtual bool CanInteractWith         ( const MIL_Population& population ) const;
    virtual bool CanBeSeen               () const;
    virtual void Register                ( MIL_InteractiveContainer_ABC* capacity );
    virtual void PreprocessAgent         ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population );
    //@}

    //! @name StructuralStateNotifier
    //@{
    virtual void Register( MIL_StructuralStateNotifier_ABC& notifier );
    virtual void Unregister( MIL_StructuralStateNotifier_ABC& notifier );
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
    virtual void SetExtensions( const MIL_DictionaryExtensions& );
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group );
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group, const DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Manipulator
    //@{
    virtual const MIL_ObjectManipulator_ABC& operator()() const;
    virtual       MIL_ObjectManipulator_ABC& operator()();
    //@}

    //! @name Network
    //@{
    virtual void OnUpdate( const sword::MissionParameters& params );
    virtual void UpdateState();
    virtual void SendCreation() const = 0;
    virtual void SendDestruction() const = 0;
    virtual void SendFullState() const;
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetName() const = 0;
    virtual void SetName( const std::string& ) {};
    size_t CountAttributes() const;
    /*virtual*/ bool IsUniversal() const;
    const MIL_DictionaryExtensions* GetExtensions() const;
    //@}

protected:
    //! @name Tools
    //@{
    virtual void Update( unsigned int time );
    virtual void Register( ObjectAttribute_ABC* attribute );
    virtual void Register( ObjectCapacity_ABC* capacity );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_InteractiveContainer_ABC* > T_InteractiveCapacities;
    typedef std::vector< MIL_StructuralStateNotifier_ABC* > T_StructuralStateNotifiers;
    typedef std::vector< ObjectAttribute_ABC* > T_Attributes;
    typedef std::vector< ObjectCapacity_ABC* > T_Capacities;

    enum E_AttributeUpdate
    {
        eAttrUpdate_Localisation = 0x10,
        eAttrUpdate_All          = 0xFF
    };
    //@}

private:
    //! @name Helpers
    //@{
    void InstanciateAttributes( DEC_Knowledge_Object& knowledge );
    //@}

private:
    std::unique_ptr< MIL_ObjectManipulator_ABC > manipulator_;
    T_InteractiveCapacities interactives_;
    T_StructuralStateNotifiers structuralStateNotifiers_;
    T_Capacities capacities_;
    T_Attributes attributes_;
    std::unique_ptr< MIL_DictionaryExtensions > pExtensions_;

    //! @name Network
    //@{
    mutable unsigned char xAttrToUpdate_;
    //@}
};

#endif // __MIL_Object_h_
