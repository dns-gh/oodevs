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

#include "simulation_kernel/MIL.h"

#include "simulation_terrain/TER_Object_ABC.h"

#include "MIL_InteractiveContainer_ABC.h"
#include "MIL_ObjectInteraction.h"
#include "ObjectCapacity_ABC.h"
#include "ObjectAttribute_ABC.h"
#include "tools/Extendable.h"

class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_PopulationElement_ABC;
class TER_Localisation;
class MIL_ObjectManipulator_ABC;
class DEC_Knowledge_Object;
class MIL_ObjectType_ABC;

// HLA
namespace hla 
{
    class Deserializer;
    class AttributeIdentifier;
}

class HLA_Object_ABC;
class HLA_UpdateFunctor;


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
    explicit MIL_Object_ABC( MIL_Army_ABC& army, const MIL_ObjectType_ABC& type );
             MIL_Object_ABC();
    virtual ~MIL_Object_ABC();
    
    //! @name Init
    //@{
    virtual void Initialize( const TER_Localisation& localisation );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;    
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateState       ();
            void ProcessEvents     ();
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
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent ) = 0;
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population ) = 0;    
    //@}

    //! @name 
    //@{
    template< typename T> void ProcessAgentsInside( T& functor ) const;
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team ) = 0;
    //@}

    //! @name Manipulator
    //@{
    virtual const MIL_ObjectManipulator_ABC& operator()() const = 0;
    virtual       MIL_ObjectManipulator_ABC& operator()() = 0;
    //@}

    //! @name Accessors
    //@{
    bool IsMarkedForDestruction() const;
    bool IsReadyForDeletion    () const;
    //@}

    //! @name HLA
    //@{
    virtual HLA_Object_ABC* GetHLAView() const = 0;
    virtual void            SetHLAView( HLA_Object_ABC& view ) = 0;
    virtual void    Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer ) = 0;
    virtual void    Serialize  ( HLA_UpdateFunctor& functor ) const = 0;
    //@}
    
    //! @name Network
    //@{
    virtual ASN1T_EnumObjectErrorCode OnUpdate( const ASN1T_ObjectAttributes& asn ) = 0;
    virtual void    SendCreation() const = 0;
    virtual void    SendDestruction() const = 0;
    virtual void    SendFullState() const = 0;
    //@}

    //! @name Accessors
    //@{    
    virtual uint              GetID() const = 0;
    const MIL_ObjectType_ABC& GetType() const;
    const MIL_Army_ABC& GetArmy() const;
          MIL_Army_ABC& GetArmy();
    //@}

    //! @name Extensions
    //@{
    template< typename Extension > const Extension& GetAttribute() const;
    template< typename Extension >       Extension* RetrieveAttribute();
    //@}

protected:
    //! @name Tools
    //@{
    virtual bool CanCollideWithEntity() const;
    virtual void UpdateLocalisation( const TER_Localisation& newLocalisation ); 
    virtual void Update( uint time ) = 0;
    void Register();
    void Unregister();
    //@}

private:
    //! @name Members
    //@{
    const MIL_ObjectType_ABC*   pType_;    
          MIL_Army_ABC*         pArmy_;
    //@}

    //! @name Interaction proxy
    //@{
    MIL_ObjectInteraction       interaction_;
    //@}

    //! @name destruction management
    //@{
    bool                        bMarkedForDestruction_;
    bool                        bReadyForDeletion_;        
    //@}
};

#include "MIL_Object_ABC.inl"

#endif // __MIL_Object_ABC_h_

