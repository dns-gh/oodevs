// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:04 $
// $Revision: 12 $
// $Workfile: PHY_RoleAction_Objects.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Objects.h"
#include "MIL_AgentServer.h"
#include "PHY_RoleAction_Objects_DataComputer.h"
#include "PHY_RoleAction_Objects_CapabilityComputer.h"
#include "AlgorithmsFactories.h"
#include "DotationComputer_ABC.h"
#include "DotationComputerFactory_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/BuildableCapacity.h"
#include "Entities/Objects/ImprovableCapacity.h"
#include "Entities/Objects/WorkableCapacity.h"
#include "Entities/Objects/SpawnCapacity.h"
#include "Entities/Objects/OccupantAttribute.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/FireAttribute.h"
#include "Entities/Objects/BurnAttribute.h"
#include "Entities/Objects/ExtinguishableCapacity.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RoleAction_Objects )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Objects* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Objects* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_Objects( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::PHY_RoleAction_Objects( MIL_AgentPion& pion )
    : pion_( pion )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::~PHY_RoleAction_Objects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Objects::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetObject
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_Object_ABC* PHY_RoleAction_Objects::GetObject( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return 0;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( pObject )
        return pObject;
    pion_.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Construct
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Construct( MIL_Object_ABC& object )
{
    if( !object().CanBeConstructed() )
        return eImpossible;
    if( object().IsBuilt() )
        return eFinished;
    PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eConstruct, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;

    // $$$$ TODO: refactor to handle more than a single resource
    const ConstructionAttribute* pAttribute = object.RetrieveAttribute< ConstructionAttribute >();
    const PHY_DotationCategory* pDotationCategory = 0;
    unsigned int nDotationNeeded = 0;
    if( pAttribute )
    {
        nDotationNeeded = pAttribute->GetDotationNeededForConstruction( rDeltaPercentage );
        if( nDotationNeeded )
        {
            pDotationCategory = object.Get< BuildableCapacity >().GetDotationCategory();
            if( pDotationCategory && !dataComputer.HasDotations( *pDotationCategory, nDotationNeeded ) )
                return eNoMoreDotation;
        }
    }

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Construct( rDeltaPercentage );
    double rDotationNeeded = nDotationNeeded;
    if( pDotationCategory )
        dataComputer.ConsumeDotations( *pDotationCategory, rDotationNeeded );
    assert( 0. == rDotationNeeded );
    if( object().IsBuilt() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Construct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Construct( MIL_Object_ABC* pObject, boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    if( !pObject )
    {
        pKnowledge.reset();
        return eImpossible;
    }
    pKnowledge = pion_.GetArmy().GetKnowledge().GetKnowledgeObject( *pObject );
    return Construct( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Destroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Destroy( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return 0;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
    {
        pion_.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
        return 0;
    }

    MIL_Object_ABC& object = *pObject;
    if( !object().CanBeDestroyed() )
        return eImpossible;
    if( object().IsMined() )
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eDemine, object );

        const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< double >::max() )
            return eNoCapacity;

        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().Demine( rDeltaPercentage );
        return eRunning;
    }
    else
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eDestroy, object );

        const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< double >::max() )
            return eNoCapacity;

        // $$$$ TODO: refactor to handle more than a single resource
        const ConstructionAttribute& attribute = object.GetAttribute< ConstructionAttribute >();
        const unsigned int nDotationRecovered = attribute.GetDotationRecoveredWhenDestroying( rDeltaPercentage );
        object().Destroy( rDeltaPercentage );

        if( nDotationRecovered && pion_.GetArmy() == *pObject->GetArmy() )
        {
            BuildableCapacity* pCapacity = object.Retrieve< BuildableCapacity >();
            if( pCapacity )
            {
                const PHY_DotationCategory* pDotationCategory  = pCapacity->GetDotationCategory();
                dataComputer.RecoverDotations( *pDotationCategory, nDotationRecovered );
            }
        }
        if( attribute.GetState() == 0. )
        {
            pion_.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
            return eFinished;
        }
        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        return eRunning;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( MIL_Object_ABC& object )
{
    if( !object().CanBeMined() )
        return eImpossible;
    PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eMine, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;

    const MineAttribute& attribute = object.GetAttribute< MineAttribute >();
    unsigned int nDotationNeeded = attribute.GetDotationNeededForConstruction( rDeltaPercentage );
    const PHY_DotationCategory* pDotationCategory = 0;
    if( nDotationNeeded )
    {
        pDotationCategory = object.Get< ImprovableCapacity >().GetDotationCategory();
        if( pDotationCategory && !dataComputer.HasDotations( *pDotationCategory, nDotationNeeded ) )
            return eNoMoreDotation;
    }

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Mine( rDeltaPercentage );
    double rDotationNeeded = nDotationNeeded;
    if( pDotationCategory )
        dataComputer.ConsumeDotations( *pDotationCategory, rDotationNeeded );
    if( object().IsFullyMined() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    return Mine( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Demine
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Demine( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    return Demine( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Demine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Demine( MIL_Object_ABC& object )
{
    if( !object().CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eDemine, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;
    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Demine( rDeltaPercentage );
    if( object.GetAttribute< MineAttribute >().GetState() == 0. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Bypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Bypass( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( !object().CanBeBypassed() )
        return eImpossible;
    if( object().IsBypassed() )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eBypass, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;
    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Bypass( rDeltaPercentage );
    if( object().IsBypassed() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Extinguish
// Created: RFT 28/05/2008
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Extinguish( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;

    MIL_Object_ABC& object = *pObject;
    ExtinguishableCapacity* capacity = object.Retrieve< ExtinguishableCapacity >();
    if( !capacity )
        return eNoCapacity;

    FireAttribute* fireAttribute = object.RetrieveAttribute< FireAttribute >();
    if( !fireAttribute )
        return eImpossible;

    BurnAttribute* burnAttribute = object.RetrieveAttribute< BurnAttribute >();
    if( !burnAttribute )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( pion_, eExtinguish, object );
    const PHY_DotationCategory* pExtinguisherAgent = fireAttribute->FindBestExtinguisherAgent( boost::bind( &PHY_RoleAction_Objects_DataComputer::HasDotations, &dataComputer, _1, 1 ) );
    if( !pExtinguisherAgent )
        return eNoMoreDotation;

    burnAttribute->Extinguish( object, *pExtinguisherAgent );
    double nDotation = 1.;
    dataComputer.ConsumeDotations( *pExtinguisherAgent, nDotation );

    if( burnAttribute->IsExtinguished() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::SupplyStock
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::SupplyStock( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
{
    MIL_Object_ABC* pObject = GetObject( objectKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    StockAttribute* attribute = object.RetrieveAttribute< StockAttribute >();
    if( !attribute )
        return eImpossible;
    PHY_RoleInterface_Supply* supplyRole = pion_.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return eImpossible;

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
    {
        PHY_DotationStock& stock = pion_.GetRole< PHY_RolePion_Composantes >().GetOrAddStock( *supplyRole, *pDotation );
        double consumed = stock.Consume( quantity );
        double supplied = attribute->Supply( *pDotation, consumed );
        stock.SupplyUntilFull( std::max( consumed - supplied, 0.0 ) );
    }

    BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
    {
        if( PHY_DotationStock* pStock = supplyRole->GetStock( *pDotation ) )
            if( !pStock->IsEmpty() && attribute->CanBeSuppliedWith( *pDotation ) )
                return eRunning;
    }

    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ExtractFromStock
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ExtractFromStock( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
{
    MIL_Object_ABC* pObject = GetObject( objectKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    StockAttribute* attribute = object.RetrieveAttribute< StockAttribute >();
    if( !attribute )
        return eImpossible;
    PHY_RoleInterface_Supply* supplyRole = pion_.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return eImpossible;
    PHY_RolePion_Composantes& composantes = pion_.GetRole< PHY_RolePion_Composantes >();

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
    {
        PHY_DotationStock& stock = composantes.GetOrAddStock( *supplyRole, *pDotation );
        double distributed = attribute->Distribute( *pDotation, quantity );
        double stocked = stock.SupplyUntilFull( distributed ) ;
        attribute->Supply( *pDotation, std::max( distributed - stocked, 0.0 ) );
    }

    BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
    {
        if( attribute->CanDistribute( *pDotation ) && composantes.CanStockMoreOf( *supplyRole, *pDotation ) )
            return eRunning;
    }

    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Distribute
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Distribute( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, double quantity )
{
    MIL_Object_ABC* pObject = GetObject( objectKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    StockAttribute* stockAttribute = object.RetrieveAttribute< StockAttribute >();
    if( !stockAttribute )
        return eImpossible;
    ResourceNetworkCapacity* resourceNetworkCapacity = object.Retrieve< ResourceNetworkCapacity >();
    if( !resourceNetworkCapacity )
        return eImpossible;

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    typedef std::vector< std::pair< const PHY_DotationCategory*, double > > T_SelectionVector;
    typedef T_SelectionVector::const_iterator IT_SelectionVector;
    T_SelectionVector selection;
    stockAttribute->DeprecatedSelectDotations( selection, true );
    for ( IT_SelectionVector it = selection.begin(); it != selection.end(); ++it )
    {
            double toBeDistributed = stockAttribute->Distribute( *it->first, quantity );
            double distributed = resourceNetworkCapacity->AddToStock( *it->first, toBeDistributed );
            stockAttribute->Supply( *it->first, std::max( toBeDistributed - distributed, 0.0 ) );
    }

    for ( IT_SelectionVector it = selection.begin(); it != selection.end(); ++it )
    {
        if( stockAttribute->CanDistribute( *it->first ) )
            return eRunning;
    }

    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( ! object().IsBuilt() )
        return Construct( object );
    else if( object().CanBeMined() && !object().IsFullyMined() )
        return Mine( object );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( UrbanObjectWrapper* pUrbanBlock )
{
    MIL_Object_ABC* pObject = pUrbanBlock;
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( ! object().IsBuilt() )
        return Construct( object );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartAnimateObject( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;
    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if( capacity )
    {
        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().AddAnimator( pion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::SetCreato
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::SetCreator( MIL_Object_ABC& object )
{
    SpawnCapacity* capacity = object.Retrieve< SpawnCapacity >();
    if( capacity )
        object().AddCreator( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopAnimateObject( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;

    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if( capacity )
    {
        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().ReleaseAnimator( pion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartOccupyingObject( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if( pAttribute )
    {
        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->AddOccupant( pion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopOccupyingObject( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if( pAttribute )
    {
        pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->ReleaseOccupant( pion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanConstructWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanConstructWithReinforcement( const std::string& strType, bool bWithLoaded ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( pion_, eConstruct, type, bWithLoaded );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( pion_, eBypass, object, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( pion_, eDestroy, object, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanMineWithReinforcement
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanMineWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( pion_, eMine, object, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::EnoughtDotationForBuilding
// Created: LMT 2010-07-07
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::EnoughtDotationForBuilding( const std::string& objectType, MIL_Agent_ABC& pion ) const
{
    bool result = false;
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( objectType );
    const BuildableCapacity* capacity = type.GetCapacity< BuildableCapacity >();
    if ( capacity == 0   )
        return true;
    const PHY_DotationCategory* pDotationCategory = capacity->GetDotationCategory();
    if ( pDotationCategory  == 0 )
        return true;
    std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pion.GetAlgorithms().dotationComputerFactory_->Create() );
    pion.Execute( *dotationComputer );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        result = result || EnoughtDotationForBuilding( objectType, **itReinforcement );
    return result || dotationComputer->GetDotationValue( *pDotationCategory ) > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetInitialReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetFinalReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWorkSuspended
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ResumeWorkSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ConstructSuspended
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ConstructSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::DestroySuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::DestroySuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::MineSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::MineSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::DemineSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::DemineSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::BypassSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::BypassSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ExtinguishSuspended
// Created: RFT 28/05/2008
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ExtinguishSuspended()
{
    // NOTHING
}
