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
#include "Decision/DEC_Gen_Object.h"
#include "DefaultDotationComputer.h"
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
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/OccupantAttribute.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/FireAttribute.h"
#include "Entities/Objects/BurnAttribute.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RoleAction_Objects )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::PHY_RoleAction_Objects()
    : owner_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::PHY_RoleAction_Objects( MIL_AgentPion& pion )
    : owner_( &pion )
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
    file & owner_;
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
    if( owner_->GetKnowledgeGroup() )
    {
        auto bbKg = owner_->GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
            bbKg->GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
    }
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
    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eConstruct, object );
    double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
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
            if( pDotationCategory )
            {
                if( !dataComputer.HasDotations( *pDotationCategory, nDotationNeeded ) )
                {
                    unsigned int availableDotations = dataComputer.GetDotationsNumber( *pDotationCategory );
                    if( !availableDotations )
                        return eNoMoreDotation;
                    double delta = double( availableDotations )/ double( nDotationNeeded );
                    rDeltaPercentage *= delta;
                    nDotationNeeded = availableDotations;
                }
            }
        }
    }

    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
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
int PHY_RoleAction_Objects::Construct( MIL_Object_ABC* pObject, boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge, bool instantaneous )
{
    if( !pObject )
    {
        pKnowledge.reset();
        return eImpossible;
    }
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = owner_->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        pKnowledge = container->GetKnowledgeObject( *pObject );
    if( instantaneous )
        return pKnowledge.get() ? eFinished : eRunning;
    else
        return Construct( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Destroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Destroy( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return eImpossible;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    auto bbKg = owner_->GetKnowledgeGroup()->GetKnowledge();
    if( !pObject )
    {
        if( bbKg )
            bbKg->GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
        return eImpossible;
    }

    MIL_Object_ABC& object = *pObject;

    int ret = Destroy( object, 0 );

    if( ret == eFinished )
    {
        const ConstructionAttribute* construction = object.RetrieveAttribute< ConstructionAttribute >();
        if( bbKg && construction && construction->GetState() == 0 )
            bbKg->GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( pKnowledge );
    }
    else if( ret == eRunning )
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Destroy
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Destroy( MIL_Object_ABC& object, double finalState )
{
    if( !object().CanBeDestroyed() )
        return eImpossible;
    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eDestroy, object );

    double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;

    // $$$$ TODO: refactor to handle more than a single resource
    unsigned int nDotationRecovered = 0;
    const ConstructionAttribute* construction = object.RetrieveAttribute< ConstructionAttribute >();
    const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
    if( construction )
    {
        if( construction->GetState() <= finalState )
            return eFinished;
        rDeltaPercentage = std::min( rDeltaPercentage, construction->GetState() - finalState );
        nDotationRecovered = construction->GetDotationRecoveredWhenDestroying( rDeltaPercentage );
    }
    if( structural )
    {
        if( structural->GetStructuralState() <= finalState )
            return eFinished;
        rDeltaPercentage = std::min( rDeltaPercentage, structural->GetStructuralState() - finalState );
    }

    object().Destroy( rDeltaPercentage );

    if( nDotationRecovered && object.GetArmy() && owner_->GetArmy() == *object.GetArmy() )
    {
        BuildableCapacity* pCapacity = object.Retrieve< BuildableCapacity >();
        if( pCapacity )
        {
            const PHY_DotationCategory* pDotationCategory  = pCapacity->GetDotationCategory();
            if( pDotationCategory )
                dataComputer.RecoverDotations( *pDotationCategory, nDotationRecovered );
        }
    }
    if( construction && construction->GetState() <= finalState )
        return eFinished;
    if( structural && structural->GetStructuralState() <= finalState )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( MIL_Object_ABC& object )
{
    if( !object().CanBeMined() )
        return eImpossible;
    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eMine, object );
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

    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
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
int PHY_RoleAction_Objects::Mine( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
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
int PHY_RoleAction_Objects::Demine( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
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

    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eDemine, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;
    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Demine( rDeltaPercentage );
    if( object.GetAttribute< MineAttribute >().GetState() == 0. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Bypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Bypass( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( !object().CanBeBypassed() )
        return eImpossible;
    if( object().IsBypassed() )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eBypass, object );
    const double rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< double >::max() )
        return eNoCapacity;
    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Bypass( rDeltaPercentage );
    if( object().IsBypassed() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Extinguish
// Created: RFT 28/05/2008
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Extinguish( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;

    MIL_Object_ABC& object = *pObject;

    FireAttribute* fireAttribute = object.RetrieveAttribute< FireAttribute >();
    if( !fireAttribute )
        return eImpossible;

    BurnAttribute* burnAttribute = object.RetrieveAttribute< BurnAttribute >();
    if( !burnAttribute )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eExtinguish, object );
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
int PHY_RoleAction_Objects::SupplyStock( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
{
    MIL_Object_ABC* pObject = GetObject( objectKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    StockAttribute* attribute = object.RetrieveAttribute< StockAttribute >();
    if( !attribute )
        return eImpossible;
    PHY_RoleInterface_Supply* supplyRole = owner_->RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return eImpossible;

    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
    {
        PHY_DotationStock& stock = owner_->GetRole< PHY_RolePion_Composantes >().GetOrAddStock( *supplyRole, *pDotation );
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
int PHY_RoleAction_Objects::ExtractFromStock( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
{
    MIL_Object_ABC* pObject = GetObject( objectKnowledge );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    StockAttribute* attribute = object.RetrieveAttribute< StockAttribute >();
    if( !attribute )
        return eImpossible;
    PHY_RoleInterface_Supply* supplyRole = owner_->RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return eImpossible;
    PHY_RolePion_Composantes& composantes = owner_->GetRole< PHY_RolePion_Composantes >();

    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

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
int PHY_RoleAction_Objects::Distribute( const boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, double quantity )
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

    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

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
int PHY_RoleAction_Objects::ResumeWork( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
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
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge, bool valorizeIt )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( ! object().IsBuilt() )
        return Construct( object );
    else if( valorizeIt && object().CanBeMined() && !object().IsFullyMined() )
        return Mine( object );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( MIL_UrbanObject_ABC* pUrbanBlock )
{
    if( !pUrbanBlock )
        return eImpossible;
    return Construct( *pUrbanBlock );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::DeteriorateUrbanBlock
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::DeteriorateUrbanBlock( MIL_UrbanObject_ABC* pUrbanBlock, double percentage )
{
    if( !pUrbanBlock )
        return eImpossible;
    int ret = Destroy( *pUrbanBlock, percentage );
    if( ret == eRunning )
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pUrbanBlock );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartAnimateObject( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;
    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if( capacity )
    {
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().AddAnimator( *owner_ );
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
        object().AddCreator( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopAnimateObject( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;

    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if( capacity )
    {
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().ReleaseAnimator( *owner_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartOccupyingObject( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if( pAttribute )
    {
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->AddOccupant( *owner_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopOccupyingObject( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
{
    MIL_Object_ABC* pObject = GetObject( pKnowledge );
    if( !pObject )
        return;

    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if( pAttribute )
    {
        owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->ReleaseOccupant( *owner_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanConstructWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanConstructWithReinforcement( const std::string& strType, const TER_Localisation* localisation, bool bWithLoaded ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eConstruct, type, localisation, bWithLoaded );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanConstructWithoutReinforcement
// Created: LGY 2011-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanConstructWithoutReinforcement( const std::string& objectType, const TER_Localisation* localisation, bool bWithLoaded ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( objectType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eConstruct, type, localisation, bWithLoaded, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eBypass, object, &localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eDestroy, object, &localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDemineWithReinforcement
// Created: DDA 2012-03-16
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDemineWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eDemine, object, &localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDemineTypeWithReinforcement
// Created: LMT 2013-03-27
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDemineTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation, bool bWithLoaded ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eDemine, type, localisation, bWithLoaded );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanMineWithReinforcement
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanMineWithReinforcement( const MIL_ObjectType_ABC& object, const TER_Localisation& localisation ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eMine, object, &localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanMineTypeWithReinforcement
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanMineTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eMine, type, localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyTypeWithReinforcement
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eDestroy, type, localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassTypeWithReinforcement
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassTypeWithReinforcement( const std::string& strType, const TER_Localisation* localisation  ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eBypass, type, localisation, false );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanExtinguish
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanExtinguish( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge ) const
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    MIL_Object_ABC* object = pKnowledge->GetObjectKnown();
    if( !object )
        return false;
    if( object->RetrieveAttribute< BurnAttribute >() == 0 )
        return false;
    FireAttribute* fire = object->RetrieveAttribute< FireAttribute >();
    if( !fire )
        return false;
    TER_Localisation localisation = object->GetLocalisation();
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *owner_, eExtinguish, object->GetType(), &localisation, false );
    if( !capabilityComputer.HasCapability() )
        return false;
    PHY_RoleAction_Objects_DataComputer dataComputer( *owner_, eExtinguish, *object );
    return fire->FindBestExtinguisherAgent( boost::bind( &PHY_RoleAction_Objects_DataComputer::HasDotations, &dataComputer, _1, 1 ) ) != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::EnoughDotationForBuilding
// Created: LMT 2010-07-07
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::EnoughDotationForBuilding( const std::string& objectType, MIL_Agent_ABC& pion, bool bWithLoaded ) const
{
    bool result = false;
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( objectType );
    const BuildableCapacity* capacity = type.GetCapacity< BuildableCapacity >();
    if ( capacity == 0   )
        return true;
    const PHY_DotationCategory* pDotationCategory = capacity->GetDotationCategory();
    if ( pDotationCategory  == 0 )
        return true;
    dotation::DefaultDotationComputer dotationComputer;
    pion.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    if( bWithLoaded )
    {
        const auto& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
        for( auto itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
            result = result || EnoughDotationForBuilding( objectType, **itReinforcement, bWithLoaded );
    }
    return result || dotationComputer.GetDotationValue( *pDotationCategory ) > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetAgentDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
double PHY_RoleAction_Objects::GetAgentDotationNumber( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory ) const
{
    if ( pDotationCategory  == 0 )
        return -1;
    dotation::DefaultDotationComputer dotationComputer;
    pion.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    return dotationComputer.GetDotationValue( *pDotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetAgentMissingDotationForBuildingObject
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > PHY_RoleAction_Objects::GetAgentMissingDotationForBuildingObject( const DEC_Gen_Object* object, MIL_Agent_ABC& pion ) const
{
    if( !object )
        throw MASA_EXCEPTION( "Invalid DEC_Gen_Object" );

    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( object->GetTypeName() );
    const BuildableCapacity* capacity = type.GetCapacity< BuildableCapacity >();
    const PHY_DotationCategory* pDotationCategory = capacity ? capacity->GetDotationCategory() : 0;
    if ( pDotationCategory == 0 )
        return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, -1 );

    dotation::DefaultDotationComputer dotationComputer;
    pion.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    double number = 0;
    int dotationNumber =  capacity->GetDotationNumber( object->GetLocalisation() );
    if ( dotationNumber != 0 )
        number = std::max((int) ( dotationNumber - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    else
        number = std::max((int) ( capacity->GetMaxDotation() - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, number );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetAgentMissingDotationForBuildingExistingObject
// Created: DDA 2012-04-05
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > PHY_RoleAction_Objects::GetAgentMissingDotationForBuildingExistingObject( const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, MIL_Agent_ABC& pion ) const
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "Invalid DEC_Knowledge_Object" );

    const MIL_ObjectType_ABC& type = pKnowledge->GetType();
    const BuildableCapacity* capacity = type.GetCapacity< BuildableCapacity >();
    const PHY_DotationCategory* pDotationCategory = capacity ? capacity->GetDotationCategory() : 0;
    if ( pDotationCategory == 0 )
        return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, -1 );

    dotation::DefaultDotationComputer dotationComputer;
    pion.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    double number;
    int dotationNumber =  capacity->GetDotationNumber( pKnowledge->GetLocalisation() );
    if ( dotationNumber != 0 )
        number = std::max((int) ( dotationNumber - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    else
        number = std::max((int) ( capacity->GetMaxDotation() - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    MIL_Object_ABC* object = pKnowledge->GetObjectKnown();
    if( object )
    {
        ConstructionAttribute* att = object->RetrieveAttribute< ConstructionAttribute >();
        if( att )
            number = att->GetDotationNeededForConstruction( 1. );
    }
    return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, number );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetAgentMissingDotationForMiningObstacle
// Created: LMT 2012-02-03
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > PHY_RoleAction_Objects::GetAgentMissingDotationForMiningObstacle( const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, MIL_Agent_ABC& pion ) const
{
    if ( pKnowledge == 0 )
    {
        const PHY_DotationCategory* pDotationCategory = 0;
        return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, -1 );
    }
    const MIL_ObjectType_ABC& type = pKnowledge->GetType();
    const ImprovableCapacity* capacity = type.GetCapacity< ImprovableCapacity >();
    const PHY_DotationCategory* pDotationCategory = capacity ? capacity->GetDotationCategory() : 0;
    if ( pDotationCategory == 0 )
        return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, -1 );

    dotation::DefaultDotationComputer dotationComputer;
    pion.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    double number;
    int dotationNumber =  capacity->GetDotationNumber( pKnowledge->GetLocalisation() );
    if ( dotationNumber != 0 )
        number = std::max((int) ( dotationNumber - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    else
        number = std::max((int) ( capacity->GetMaxDotation() - dotationComputer.GetDotationValue( *pDotationCategory )), 0);
    MIL_Object_ABC* object = pKnowledge->GetObjectKnown();
    if( object )
    {
        MineAttribute* att = object->RetrieveAttribute< MineAttribute >();
        if( att )
            number = att->GetDotationNeededForConstruction( 1. );
    }
    return std::pair< const PHY_DotationCategory*, double >( pDotationCategory, number );
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
// Name: PHY_RoleAction_Objects::DeteriorateUrbanBlockSuspended
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::DeteriorateUrbanBlockSuspended()
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
