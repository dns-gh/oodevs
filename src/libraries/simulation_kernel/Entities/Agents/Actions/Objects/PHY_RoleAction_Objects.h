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
#include "MT_Tools/Role_ABC.h"

class DEC_Knowledge_Object;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_DotationCategory;
class UrbanObjectWrapper;
class DEC_Gen_Object;

// =============================================================================
// @class  PHY_RoleAction_Objects
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Objects RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_Objects( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_Objects();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    bool HasChanged() const;
    //@}

    //! @name Operations
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode() const;
    int Construct( MIL_Object_ABC* pObject, boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int ResumeWork( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int ResumeWork( UrbanObjectWrapper* pUrbanBlock );
    int Destroy( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Mine( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Demine( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Bypass( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Extinguish( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StartAnimateObject( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StopAnimateObject( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StartOccupyingObject( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StopOccupyingObject( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int SupplyStock( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity );
    int ExtractFromStock( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity );
    int Distribute( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, double quantity );
    void SetCreator( MIL_Object_ABC& object );

    void ResumeWorkSuspended();
    void ConstructSuspended();
    void DestroySuspended();
    void MineSuspended();
    void DemineSuspended();
    void BypassSuspended();
    void ExtinguishSuspended();
    //@}

    //! @name Capabilities
    //@{
    bool CanConstructWithReinforcement( const std::string& objectType, bool bWithLoaded ) const;
    bool CanConstructWithoutReinforcement( const std::string& objectType, bool bWithLoaded ) const;
    bool CanBypassWithReinforcement( const MIL_ObjectType_ABC& object ) const;
    bool CanDestroyWithReinforcement( const MIL_ObjectType_ABC& object ) const;
    bool CanMineWithReinforcement( const MIL_ObjectType_ABC& object ) const;
    bool EnoughtDotationForBuilding( const std::string& objectType, MIL_Agent_ABC& pion, bool bWithLoaded ) const;

    //Getter
    double GetAgentDotationForBuildingObstacle( const std::string& objectType, MIL_Agent_ABC& pion ) const; 
    double GetAgentMissingDotationForBuildingObstacle( const DEC_Gen_Object* object, MIL_Agent_ABC& pion ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning        , // -> l'action est en cours d'ex�cution
        eFinished       , // -> l'action est termin�e
        eImpossible     , // -> l'action est impossible (cas arrivant, par exemple, quand un essaye de valoriser un objet qui ne peut pas l'�tre)
        eNoMoreDotation , // -> l'action est arr�t�e car l'unit� ne poss�de pas assez de dotations (mines, explosifs ...) pour la mener � bien
        eNoCapacity     , // -> l'action est arr�t�e car l'unit� ne poss�de pas les capacit�s pour l'effectuer
    };
    //@}

private:
    //! @name Tools
    //@{
    MIL_Object_ABC* GetObject( const boost::shared_ptr< DEC_Knowledge_Object >& object );
    int Construct( MIL_Object_ABC& object );
    int Mine( MIL_Object_ABC& object );
    int Demine( MIL_Object_ABC& object );
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_Objects )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion& owner_;
    double         rWeightCapacity_;
    double         rVolumeCapacity_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_Objects )
INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_Objects, MIL_AgentPion )

#endif // __PHY_RoleAction_Objects_h_
