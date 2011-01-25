//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Object_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_Object_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_Object_ABC_h_
#define __MIL_Object_ABC_h_

#include "MIL.h"
#include "MIL_ObjectInteraction.h"
#include "ObjectCapacity_ABC.h"
#include "ObjectAttribute_ABC.h"
#include "simulation_terrain/TER_Object_ABC.h"
#include "tools/Extendable.h"
#include "Tools/MIL_IDManager.h"

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

namespace sword
{
    enum MagicActionAck_ErrorCode;
    enum ObjectMagicActionAck_ErrorCode;
    class MissionParameter_Value;
}

class DEC_Knowledge_Object;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_KnowledgeGroup;
class MIL_InteractiveContainer_ABC;
class MIL_StructuralStateNotifier_ABC;
class MIL_ObjectManipulator_ABC;
class MIL_ObjectType_ABC;
class MIL_PopulationElement_ABC;
class TER_Localisation;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-15
//=============================================================================
class MIL_Object_ABC : public TER_Object_ABC
                     , public tools::Extendable< ObjectCapacity_ABC >
                     , protected tools::Extendable< ObjectAttribute_ABC >
                     , private boost::noncopyable
{
public:
             MIL_Object_ABC();
             MIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type );
             MIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int id );
    virtual ~MIL_Object_ABC();

    //! @name Init
    //@{
    virtual void Initialize( const TER_Localisation& localisation );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateState();
    virtual void UpdateLocalisation( const TER_Localisation& newLocalisation );
    virtual bool IsInside( const MT_Vector2D& vPos ) const;
    void ProcessEvents();
    void MarkForDestruction();
    //@}

    //! @name Interactions
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    virtual bool CanInteractWith( const MIL_PopulationElement_ABC& population ) const;
    //@}

    //! @name Interaction events
    //@{
    void NotifyPopulationMovingInside ( MIL_PopulationElement_ABC& population );
    void NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population );

    void NotifyAgentMovingInside ( MIL_Agent_ABC& agent );
    void NotifyAgentMovingOutside( MIL_Agent_ABC& agent );
    void NotifyAgentPutInside    ( MIL_Agent_ABC& agent );
    void NotifyAgentPutOutside   ( MIL_Agent_ABC& agent );
    //@}

    //! @name InteractiveContainer
    //@{
    virtual void Register                ( MIL_InteractiveContainer_ABC* capacity ) = 0;
    virtual void PreprocessAgent         ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent ) = 0;
    virtual void PreprocessPopulation    ( MIL_PopulationElement_ABC& population ) = 0;
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population ) = 0;
    //@}

    //! @name Helpers
    //@{
    template< typename T> void ProcessAgentsInside( T functor ) const;
    template< typename T, typename I > void SetAttribute( const T& attribute );
    //@}

    //! @name StructuralStateNotifier
    //@{
    virtual void Register( MIL_StructuralStateNotifier_ABC& notifier ) = 0;
    virtual void ApplyStructuralState( unsigned int structuralState ) const = 0;
    //@}

    //! @name Instanciate / Build / Copy object
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const = 0; //<! create and register every prototyped capacity
    virtual void Finalize() = 0; //<! finalize capacity instanciation : for instance once the object location has been defined
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team ) = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group ) = 0;
    //@}

    //! @name Manipulator
    //@{
    virtual const MIL_ObjectManipulator_ABC& operator()() const = 0;
    virtual       MIL_ObjectManipulator_ABC& operator()() = 0;
    //@}

    //! @name Accessors
    //@{
    bool IsMarkedForDestruction() const;
    bool IsReadyForDeletion() const;
    virtual bool IsUniversal() const;
    //@}

    //! @name Network
    //@{
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes ) = 0;
    virtual sword::ObjectMagicActionAck_ErrorCode OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes ) = 0;
    virtual sword::MagicActionAck_ErrorCode OnUpdateResourceLinks( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list );

    virtual void SendCreation() const = 0;
    virtual void SendDestruction() const = 0;
    virtual void SendFullState() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetName() const = 0;
    unsigned int GetID() const;
    const MIL_ObjectType_ABC& GetType() const;
    const MIL_Army_ABC* GetArmy() const;
          MIL_Army_ABC* GetArmy();
    //@}

    //! @name Extensions
    //@{
    template< typename T >             T& GetAttribute();
    template< typename T >       const T& GetAttribute() const;
    template< typename T, typename I > T& GetAttribute();
    template< typename T >             T* RetrieveAttribute();
    template< typename T >       const T* RetrieveAttribute() const;
    template< typename T >           void AddCapacity( T* capacity );
    //@}

protected:
    //! @name Tools
    //@{
    virtual bool CanCollideWithEntity() const;
    virtual void Update( unsigned int time ) = 0;
    void Register();
    void Unregister();
    virtual void Register( ObjectAttribute_ABC* attribute ) = 0;
    virtual void Register( ObjectCapacity_ABC* capacity ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    static MIL_IDManager idManager_;
    unsigned int id_;
    const MIL_ObjectType_ABC* pType_;
    MIL_Army_ABC* pArmy_;
    MIL_ObjectInteraction interaction_;
    bool bMarkedForDestruction_;
    bool bReadyForDeletion_;
    //@}
};

#include "MIL_Object_ABC.inl"

#endif // __MIL_Object_ABC_h_
