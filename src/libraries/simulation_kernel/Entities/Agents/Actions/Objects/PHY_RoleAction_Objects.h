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
class TER_Localisation;
class MIL_UrbanObject_ABC;
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
             PHY_RoleAction_Objects();
    explicit PHY_RoleAction_Objects( MIL_AgentPion& pion );
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
    int Construct( MIL_Object_ABC* pObject, boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, bool instantaneous );
    int ResumeWork( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int ResumeWork( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge, bool valorizeIt );
    int ResumeWork( MIL_UrbanObject_ABC* pUrbanBlock );
    int DeteriorateUrbanBlock( MIL_UrbanObject_ABC* pUrbanBlock, double percentage );
    int Destroy( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Mine( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Demine( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Bypass( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int Extinguish( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StartAnimateObject( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StopAnimateObject( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StartOccupyingObject( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    void StopOccupyingObject( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge );
    int SupplyStock( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity );
    int ExtractFromStock( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity );
    int Distribute( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, double quantity );
    void SetCreator( MIL_Object_ABC& object );

    void ResumeWorkSuspended();
    void DeteriorateUrbanBlockSuspended();
    void ConstructSuspended();
    void DestroySuspended();
    void MineSuspended();
    void DemineSuspended();
    void BypassSuspended();
    void ExtinguishSuspended();
    //@}

    //! @name Capabilities
    //@{
    bool CanConstructWithReinforcement( const std::string& objectType, const TER_Localisation* localisation, bool bWithLoaded ) const;
    bool CanConstructWithoutReinforcement( const std::string& objectType, const TER_Localisation* localisation, bool bWithLoaded ) const;
    bool CanBypassWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const;
    bool CanDestroyWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const;
    bool CanDemineWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const;
    bool CanMineWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const;
    bool EnoughDotationForBuilding( const std::string& objectType, MIL_Agent_ABC& pion, bool bWithLoaded ) const;
    bool CanMineTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation ) const;
    bool CanDestroyTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation ) const;
    bool CanBypassTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation ) const;
    bool CanExtinguish( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge ) const;

    //Getter
    double GetAgentDotationNumber( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory ) const; 
    std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForBuildingObject( const DEC_Gen_Object* object, MIL_Agent_ABC& pion ) const;
    std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForBuildingExistingObject( const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, MIL_Agent_ABC& pion ) const;
    std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForMiningObstacle( const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, MIL_Agent_ABC& pion ) const;
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
    int Destroy( MIL_Object_ABC& object, double finalState );
    int Mine( MIL_Object_ABC& object );
    int Demine( MIL_Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion* owner_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_Objects )

#endif // __PHY_RoleAction_Objects_h_
