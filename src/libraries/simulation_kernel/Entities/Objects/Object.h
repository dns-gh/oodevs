// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include <vector>
#include "MIL_Object_ABC.h"
#include "CapacityContainer_ABC.h"
#include "tools/Extendable.h"

namespace Common
{
    class MsgMissionParameter_Value;
}

namespace MsgsSimToClient
{
    enum MsgObjectMagicActionAck_ErrorCode;
}

class ObjectAttribute_ABC;
class MIL_ObjectType_ABC;
class MIL_Army_ABC;
class MIL_ObjectBuilder_ABC;
class DetectionCapacity;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: JCR 2008-04-18
// =============================================================================
class Object : public MIL_Object_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, bool reserved = true );
             Object( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, const std::string& name = std::string(), bool reserved = true );
             Object();
    virtual ~Object();
    //@}

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

    //! @name
    //@{
    virtual void Register( ObjectCapacity_ABC* capacity );
    virtual void Register( MIL_InteractiveContainer_ABC* capacity );
    //@}

    //! @name Construction
    //@{
    void Update( unsigned int time );
    void UpdateLocalisation( const TER_Localisation& location );
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

    //! @name Manipulators
    //@{
    const MIL_ObjectManipulator_ABC& operator()() const;
          MIL_ObjectManipulator_ABC& operator()();
    //@}

    //! @name Interaction
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;

    virtual void PreprocessAgent( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside( MIL_Agent_ABC& agent );
    virtual void PreprocessPopulation( MIL_PopulationElement_ABC& population );
    virtual void ProcessPopulationInside( MIL_PopulationElement_ABC& population );
    //@}

    //! @name Network
    //@{
    virtual void UpdateState();
    MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode OnUpdate( const Common::MsgMissionParameter_Value& list );
    void SendCreation() const;
    void SendDestruction() const;
    void SendFullState() const;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* GetHLAView() const;
    void SetHLAView( HLA_Object_ABC& view );
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name
    //@{
    unsigned int  GetID() const;
    virtual unsigned int  GetMaterial() const{ return std::numeric_limits< unsigned int >::max(); };   // $$$$ _TODO_ SLG 2010-06-24: mettre en place un système de material pour les objets
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void SendMsgUpdate() const;
    virtual void Register( ObjectAttribute_ABC* attribute );
    //@}

private:
    //! @name Types containers
    //@{
    typedef std::vector< ObjectAttribute_ABC* > T_Attributes;
    typedef T_Attributes::const_iterator      CIT_Attributes;
    typedef std::vector< ObjectCapacity_ABC* > T_Capacities;
    typedef std::vector< MIL_InteractiveContainer_ABC* > T_InteractiveCapacities;
    //@}

    //! @name Types
    //@{
    enum E_AttributeUpdate
    {
        eAttrUpdate_ConstructionPercentage    = 0x01,
        eAttrUpdate_MiningPercentage          = 0x02,
        eAttrUpdate_BypassPercentage          = 0x04,
        eAttrUpdate_ReservedObstacleActivated = 0x08,
        eAttrUpdate_Localisation              = 0x10,
        eAttrUpdate_SpecificAttributes        = 0x20,
        eAttrUpdate_All                       = 0xFF
    };
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned int id_;
    T_Capacities capacities_;
    T_InteractiveCapacities interactives_;
    T_Attributes attributes_;
    std::auto_ptr< Object > pChildObject_;
    //@}

    //! @name
    //@{
    std::auto_ptr< MIL_ObjectManipulator_ABC > manipulator_;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* pView_;
    //@}

    //! @name Network
    //@{
    mutable unsigned char xAttrToUpdate_;
    mutable unsigned char xAttrToUpdateForHLA_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( Object )

#endif
