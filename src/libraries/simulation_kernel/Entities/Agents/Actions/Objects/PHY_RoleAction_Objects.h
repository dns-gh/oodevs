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

#include "MT_Tools/Role_ABC.h"

class NET_ASN_MsgUnitAttributes;
class DEC_Knowledge_Object;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_AgentPion;
class PHY_DotationCategory;
class PHY_RoleAction_Objects_DataComputer;
class PHY_RoleAction_Objects_CapabilityComputer;

// =============================================================================
// @class  PHY_RoleAction_Objects
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects : public tools::Role_ABC
                             , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Objects RoleInterface;
    //@}

public:
             PHY_RoleAction_Objects( MIL_AgentPion& pion );
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

    int  Construct           ( MIL_Object_ABC* pObject, boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    int  ResumeWork          ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    int  Destroy             ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge ); 
    int  Mine                ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge ); 
    int  Demine              ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge ); 
    int  Bypass              ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    int  Extinguish          ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    void StartAnimateObject  ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    void StopAnimateObject   ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    void StartOccupyingObject( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );
    void StopOccupyingObject ( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge );

    void ResumeWorkSuspended();
    void ConstructSuspended ();
    void DestroySuspended   ();
    void MineSuspended      ();
    void DemineSuspended    ();
    void BypassSuspended    ();
    void ExtinguishSuspended();
    //@}

    //! @name Capabilities
    //@{
    bool CanConstructWithReinforcement( const std::string& objectType ) const;
    bool CanBypassWithReinforcement   ( const MIL_ObjectType_ABC& object ) const;
    bool CanDestroyWithReinforcement  ( const MIL_ObjectType_ABC& object ) const;
    bool CanMineWithReinforcement     ( const MIL_ObjectType_ABC& object ) const;    
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
    MIL_Object_ABC* GetObject( const boost::shared_ptr< DEC_Knowledge_Object >& object );

    int Construct( MIL_Object_ABC& object );
    int Mine     ( MIL_Object_ABC& object );
    int Demine   ( MIL_Object_ABC& object );
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_Objects* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_Objects* role, const unsigned int /*version*/ );
    //@}

private:
    MIL_AgentPion& pion_;
};

#endif // __PHY_RoleAction_Objects_h_
