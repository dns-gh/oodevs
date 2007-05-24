// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:21 $
// $Revision: 7 $
// $Workfile: PHY_RoleAction_Objects.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Objects_h_
#define __PHY_RoleAction_Objects_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

class NET_ASN_MsgUnitAttributes;
class DEC_Knowledge_Object;
class MIL_RealObject_ABC;
class MIL_RealObjectType;
class MIL_AgentPion;
class PHY_DotationCategory;
class PHY_RoleAction_Objects_DataComputer;
class PHY_RoleAction_Objects_CapabilityComputer;

// =============================================================================
// @class  PHY_RoleAction_Objects
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Objects )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Objects RoleInterface;
    //@}

public:
             PHY_RoleAction_Objects( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_Objects();
    virtual ~PHY_RoleAction_Objects();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Operations
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;

    int  Construct           ( MIL_RealObject_ABC* pObject, DEC_Knowledge_Object*& pKnowledge );
    int  ResumeWork          ( uint nKnowledgeObjectID );
    int  Destroy             ( uint nKnowledgeObjectID ); 
    int  Mine                ( uint nKnowledgeObjectID ); 
    int  Demine              ( uint nKnowledgeObjectID ); 
    int  Bypass              ( uint nKnowledgeObjectID ); 
    void StartAnimateObject  ( uint nKnowledgeObjectID );
    void StopAnimateObject   ( uint nKnowledgeObjectID );
    void StartOccupyingObject( uint nKnowledgeObjectID );
    void StopOccupyingObject ( uint nKnowledgeObjectID );

    void ResumeWorkSuspended();
    void ConstructSuspended ();
    void DestroySuspended   ();
    void MineSuspended      ();
    void DemineSuspended    ();
    void BypassSuspended    ();
    //@}

    //! @name Capabilities
    //@{
    bool CanConstructWithReinforcement( const MIL_RealObjectType& objectType ) const;
    bool CanBypassWithReinforcement   ( const MIL_RealObjectType& objectType ) const;
    bool CanDestroyWithReinforcement  ( const MIL_RealObjectType& objectType ) const;
    bool CanMineWithReinforcement     ( const MIL_RealObjectType& objectType ) const;    
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning        , // -> l'action est en cours d'exécution
        eFinished       , // -> l'action est terminée
        eImpossible     , // -> l'action est impossible (cas arrivant, par exemple, quand un essaye de valoriser un objet qui ne peut pas l'être)
        eNoMoreDotation , // -> l'action est arrêtée car l'unité ne posséde pas assez de dotations (mines, explosifs ...) pour la mener à bien
        eNoCapacity     , // -> l'action est arrêtée car l'unité ne posséde pas les capacités pour l'effectuer
    };
    //@}

private:
    //! @name Tools
    //@{
    MIL_RealObject_ABC* GetRealObject( uint nKnowledgeObjectID );

    int Construct( MIL_RealObject_ABC& object );
    int Mine     ( MIL_RealObject_ABC& object );
    //@}

private:
    MIL_AgentPion* pPion_;
};

#include "PHY_RoleAction_Objects.inl"

#endif // __PHY_RoleAction_Objects_h_
