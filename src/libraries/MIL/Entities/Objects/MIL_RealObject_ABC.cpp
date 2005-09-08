 //*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObject_ABC.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 11:39 $
// $Revision: 19 $
// $Workfile: MIL_RealObject_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_RealObject_ABC.h"

#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include "MIL_RealObjectType.h"
#include "PHY_ObjectExplosionFireResult.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"

#include "Decision/DEC_Tools.h"

#include "TER/TER_PathFindManager.h"
#include "TER/TER_DynamicData.h"

#include "HLA/HLA_Object_ABC.h"
#include "HLA/HLA_UpdateFunctor.h"
#include "HLA/Deserializer.h"
#include "HLA/AttributeIdentifier.h"

MT_Random MIL_RealObject_ABC::random_;

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::MIL_RealObject_ABC( const MIL_RealObjectType& type )
    : pType_                             ( &type )
    , rSizeCoef_                         ( 0. ) 
    , nFullNbrDotationForConstruction_   ( 0 )
    , nFullNbrDotationForMining_         ( 0 )
    , nCurrentNbrDotationForConstruction_( 0 )
    , nCurrentNbrDotationForMining_      ( 0 )
    , xAttrToUpdate_                     ( eAttrUpdate_All )
    , xAttrToUpdateForHLA_               ( eAttrUpdate_All )
    , nID_                               ( 0 )
    , nMosPlannedID_                     ( (uint)-1 )
    , rConstructionPercentage_           ( 0. )
    , rMiningPercentage_                 ( 0. )
    , rBypassPercentage_                 ( 0. )
    , nLastValueConstructionPercentage_  ( 0 )
    , nLastValueMiningPercentage_        ( 0 )
    , nLastValueBypassPercentage_        ( 0 )
    , bPrepared_                         ( false )
    , avoidanceLocalisation_             ()
    , pPathfindData_                     ( 0 )
    , pOccupier_                         ( 0 )
    , pView_                             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::MIL_RealObject_ABC()
    : pType_                             ( 0 )
    , rSizeCoef_                         ( 0. ) 
    , nFullNbrDotationForConstruction_   ( 0 )
    , nFullNbrDotationForMining_         ( 0 )
    , nCurrentNbrDotationForConstruction_( 0 )
    , nCurrentNbrDotationForMining_      ( 0 )
    , xAttrToUpdate_                     ( eAttrUpdate_All )
    , xAttrToUpdateForHLA_               ( eAttrUpdate_All )
    , nID_                               ( 0 )
    , nMosPlannedID_                     ( (uint)-1 )
    , rConstructionPercentage_           ( 0. )
    , rMiningPercentage_                 ( 0. )
    , rBypassPercentage_                 ( 0. )
    , nLastValueConstructionPercentage_  ( 0 )
    , nLastValueMiningPercentage_        ( 0 )
    , nLastValueBypassPercentage_        ( 0 )
    , bPrepared_                         ( false )
    , avoidanceLocalisation_             ()
    , pPathfindData_                     ( 0 )
    , pOccupier_                         ( 0 )
    , pView_                             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC destructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::~MIL_RealObject_ABC()
{
    delete pView_;

    if( nID_ == 0 )
        return;

    //$$$$ NAZE
    assert( pType_ );
    MIL_MOSIDManager& idManager = pType_->GetIDManager();
    if( idManager.IsMosIDValid( nID_ ) )
        idManager.ReleaseMosID( nID_ );
    else if( idManager.IsSimIDValid( nID_ ) )
        idManager.ReleaseSimID( nID_ );

    delete pPathfindData_;
}


// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( pType_ );

    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> rSizeCoef_
         >> nFullNbrDotationForConstruction_
         >> nFullNbrDotationForMining_
         >> nCurrentNbrDotationForConstruction_
         >> nCurrentNbrDotationForMining_
         >> xAttrToUpdate_
         >> xAttrToUpdateForHLA_
         >> nID_
         >> nMosPlannedID_
         >> rConstructionPercentage_
         >> rMiningPercentage_
         >> rBypassPercentage_
         >> nLastValueConstructionPercentage_
         >> nLastValueMiningPercentage_
         >> nLastValueBypassPercentage_
         >> bPrepared_
         >> avoidanceLocalisation_
         >> pView_;

    InitializeAvoidanceLocalisation();
    pType_->GetIDManager().LockMosID( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << rSizeCoef_
         << nFullNbrDotationForConstruction_
         << nFullNbrDotationForMining_
         << nCurrentNbrDotationForConstruction_
         << nCurrentNbrDotationForMining_
         << xAttrToUpdate_
         << xAttrToUpdateForHLA_
         << nID_
         << nMosPlannedID_
         << rConstructionPercentage_
         << rMiningPercentage_
         << rBypassPercentage_
         << nLastValueConstructionPercentage_
         << nLastValueMiningPercentage_
         << nLastValueBypassPercentage_
         << bPrepared_
         << avoidanceLocalisation_
         << pView_;
}


//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::InitializeAvoidanceLocalisation
// Created: NLD 2003-10-10
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::InitializeAvoidanceLocalisation()
{
    assert( pType_ );
    if ( pType_->GetAvoidanceDistance() == 0. )
        avoidanceLocalisation_.Reset();
    else
    {
        delete pPathfindData_;
        pPathfindData_ = & TER_PathFindManager::GetPathFindManager().CreateDynamicData();
        avoidanceLocalisation_.Reset( GetLocalisation() );
        avoidanceLocalisation_.Scale( pType_->GetAvoidanceDistance() );

        const T_PointVector points = avoidanceLocalisation_.GetPoints();
        assert( points.size() > 3 );

        CIT_PointVector itPoint = points.begin();
        const MT_Vector2D* pLastPoint = &*itPoint;
        for( ++itPoint; itPoint != points.end(); ++itPoint )
        {
            const MT_Vector2D* pCurPoint = &*itPoint;
            TER_PathFindManager::GetPathFindManager().CreateLineTree( *pLastPoint, *pCurPoint, *pPathfindData_ );
            pLastPoint = pCurPoint;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::InitializeCommon
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::InitializeCommon( MIL_Army& army, const TER_Localisation& localisation, uint nID, uint nMosPlannedID )
{
    MIL_Object_ABC::Initialize( army, localisation );

    nCurrentNbrDotationForConstruction_ = 0;
    nCurrentNbrDotationForMining_       = 0;
    nMosPlannedID_                      = nMosPlannedID;
    nID_                                = nID;
    InitializeAvoidanceLocalisation();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_RealObject_ABC::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    assert( pType_ );
    
    if ( !pType_->GetIDManager().IsMosIDValid( nID ) || !pType_->GetIDManager().LockMosID( nID ) )
        return EnumObjectErrorCode::error_invalid_id;

    MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asnCreateObject.camp );
    if( !pArmy )
        return EnumObjectErrorCode::error_invalid_camp;

    TER_Localisation localisation; 
    if( !NET_ASN_Tools::ReadLocation( asnCreateObject.localisation, localisation ) )
        return EnumObjectErrorCode::error_invalid_localisation;

    InitializeCommon( *pArmy, localisation, nID );
    GetArmy().GetKSObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *this );

    return EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    assert( DEC_Tools::CheckTypeID          ( diaParameters[ nCurrentParamIdx     ] ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaParameters[ nCurrentParamIdx + 1 ] ) );

          uint              nMosPlannedID = (uint)diaParameters[ nCurrentParamIdx++ ].ToPtr();
    const TER_Localisation* pLocalisation =       diaParameters[ nCurrentParamIdx++ ].ToUserPtr( pLocalisation );
    assert( pLocalisation );

    assert( pType_ );
    InitializeCommon( army, *pLocalisation, pType_->GetIDManager().GetFreeSimID(), nMosPlannedID );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::Initialize( uint nID, MIL_InputArchive& archive )
{
    assert( pType_ );
    
    nID = pType_->GetIDManager().ConvertSimIDToMosID( nID );
    pType_->GetIDManager().LockMosID( nID );

    // Armee
    std::string strArmy;
    archive.ReadField( "Armee", strArmy );
    MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( strArmy );
    if ( pArmy == 0 )
        throw MT_ScipioException( "MIL_RealObject_ABC::Initialize", __FILE__, __LINE__, "Unknown army", archive.GetContext() );

    // Localisation
    archive.Section( "Forme" );
    TER_Localisation localisation;
    localisation.Read( archive );
    archive.EndSection(); // Forme

    InitializeCommon( *pArmy, localisation, nID );
    GetArmy().GetKSObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Initialize( MIL_Army& army, const TER_Localisation& localisation )
{
    assert( pType_ );
    InitializeCommon( army, localisation, pType_->GetIDManager().GetFreeSimID() );
    GetArmy().GetKSObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *this );
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendMsgConstruction
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendMsgConstruction()
{
    if( pView_ && pView_->HideObject() )
        return;

    NET_ASN_MsgObjectCreation asnMsg;

    assert( pType_ );

    asnMsg.GetAsnMsg().oid  = nID_;
    asnMsg.GetAsnMsg().type = pType_->GetAsnID();
    asnMsg.GetAsnMsg().camp = GetArmy().GetID();
    
    NET_ASN_Tools::WriteLocation( GetLocalisation(), asnMsg.GetAsnMsg().localisation );

    if( pType_->GetDotationCategoryForConstruction() )
    {
        asnMsg.GetAsnMsg().m.type_dotation_constructionPresent = 1;
        asnMsg.GetAsnMsg().type_dotation_construction          = pType_->GetDotationCategoryForConstruction()->GetMosID();
    }

    if( pType_->GetDotationCategoryForMining() )
    {
        asnMsg.GetAsnMsg().m.type_dotation_valorisationPresent = 1;
        asnMsg.GetAsnMsg().type_dotation_valorisation          = pType_->GetDotationCategoryForMining()->GetMosID();
    }

    if( nMosPlannedID_ != (uint)-1 )
    {
        asnMsg.GetAsnMsg().m.oid_objet_planifiePresent = 1;
        asnMsg.GetAsnMsg().oid_objet_planifie          = nMosPlannedID_;
    }
    else
        asnMsg.GetAsnMsg().m.oid_objet_planifiePresent = 0;  

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent = 0;
    WriteSpecificAttributes( asnMsg );
    
    asnMsg.Send();

    NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().localisation );
}


//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendMsgDestruction
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendMsgDestruction()
{
    if( pView_ && pView_->HideObject() )
        return;
    NET_ASN_MsgObjectDestruction asnMsg;
    asnMsg.GetAsnMsg() = nID_;
    asnMsg.Send();
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::IsAttributeUpdated
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::IsAttributeUpdated( E_AttributeUpdate nAttrToUpdate, MT_Float rValue, uint nLastValue )
{
    if( !( xAttrToUpdate_ & nAttrToUpdate ) )
        return false;

    return ( xAttrToUpdate_ == eAttrUpdate_All ) || ( nLastValue != (uint)( rValue * 100. ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendMsgUpdate
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendMsgUpdate()
{
    if( pView_ && pView_->HideObject() )
        return;
    if( xAttrToUpdate_ == 0 )
        return;

    assert( nID_ != 0 );

    bool bFlag = false;

    NET_ASN_MsgObjectUpdate asnMsg;
    
    ASN1T_MsgObjectUpdate& asn = asnMsg.GetAsnMsg();
    asn.oid                    = nID_;
    asn.en_preparation         = bPrepared_;

    if( IsAttributeUpdated( eAttrUpdate_ConstructionPercentage, rConstructionPercentage_, nLastValueConstructionPercentage_ ) )
    {
        bFlag = true;
        nLastValueConstructionPercentage_     = (uint)( rConstructionPercentage_ * 100. );
        asn.m.pourcentage_constructionPresent = 1;
        asn.pourcentage_construction          = nLastValueConstructionPercentage_;
        if( pType_->GetDotationCategoryForConstruction() )
        {
            asn.m.nb_dotation_constructionPresent = 1;
            asn.nb_dotation_construction          = nCurrentNbrDotationForConstruction_;
        }
    }

    if( IsAttributeUpdated( eAttrUpdate_MiningPercentage, rMiningPercentage_, nLastValueMiningPercentage_ ) )
    {
        bFlag = true;
        nLastValueMiningPercentage_ = (uint)( rMiningPercentage_ * 100. );
        asn.m.pourcentage_valorisationPresent = 1;
        asn.pourcentage_valorisation          = nLastValueMiningPercentage_;
        if( pType_->GetDotationCategoryForMining() )
        {
            asn.m.nb_dotation_valorisationPresent = 1;
            asn.nb_dotation_valorisation          = nCurrentNbrDotationForMining_;
        }
    }

    if( IsAttributeUpdated( eAttrUpdate_BypassPercentage, rBypassPercentage_, nLastValueBypassPercentage_ ) )
    {
        bFlag = true;
        nLastValueBypassPercentage_ = (uint)( rBypassPercentage_ * 100. );
        asn.m.pourcentage_creation_contournementPresent = 1;
        asn.pourcentage_creation_contournement  = nLastValueBypassPercentage_;
    }

    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
    {
        bFlag = true;
        asn.m.localisationPresent = 1;
        NET_ASN_Tools::WriteLocation( GetLocalisation(), asnMsg.GetAsnMsg().localisation );
    }

    if( xAttrToUpdate_ & eAttrUpdate_SpecificAttributes )
    {
        bFlag = true;
        WriteSpecificAttributes( asnMsg );
    }

    if( !bFlag && !( xAttrToUpdate_ & eAttrUpdate_Prepared ))
    {
        xAttrToUpdate_ = 0;
        return;
    }

    xAttrToUpdateForHLA_ = xAttrToUpdate_;
    xAttrToUpdate_ = 0;
    asnMsg.Send();

    if( asnMsg.GetAsnMsg().m.localisationPresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendStateToNewClient
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendStateToNewClient()
{
    SendMsgConstruction();
    NotifyAttributeUpdated( eAttrUpdate_All );
    SendMsgUpdate();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::OnReceiveMagicActionUpdate
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_RealObject_ABC::OnReceiveMagicActionUpdate( const ASN1T_MagicActionUpdateObject& asnMsg )
{
    if( asnMsg.m.pourcentage_constructionPresent )
    {
        ChangeConstructionPercentage( asnMsg.pourcentage_construction / 100. );
        NotifyAttributeUpdated( eAttrUpdate_ConstructionPercentage );
    }

    if( asnMsg.m.pourcentage_valorisationPresent )
    {
        ChangeMiningPercentage( asnMsg.pourcentage_valorisation / 100. );
        NotifyAttributeUpdated( eAttrUpdate_MiningPercentage );
    }
        
    if( asnMsg.m.pourcentage_creation_contournementPresent )
    {
        rBypassPercentage_ = asnMsg.pourcentage_creation_contournement / 100.;
        NotifyAttributeUpdated( eAttrUpdate_BypassPercentage );
    }

    if( asnMsg.m.en_preparationPresent )
    {
        bPrepared_ = !!asnMsg.en_preparation;
        NotifyAttributeUpdated( eAttrUpdate_Prepared );
    }

    return EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetHLAView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_Object_ABC* MIL_RealObject_ABC::GetHLAView() const
{
    return pView_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SetView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::SetHLAView( HLA_Object_ABC& view )
{
    delete pView_;
    pView_ = &view;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::Initialize( const std::string&, const std::string&, double rCompletion, double rMining, double rBypass )
{
    bPrepared_ = rCompletion > 100;
    if( bPrepared_ )
        rCompletion -= 100;
    ChangeConstructionPercentage( rCompletion );
    ChangeMiningPercentage      ( rMining );
    rBypassPercentage_ = rBypass; //$$$ POURRI
    NotifyAttributeUpdated( eAttrUpdate_BypassPercentage );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Deserialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "coordonnees" )
    {
        TER_Localisation newLocalisation;
        deserializer >> newLocalisation;
        UpdateLocalisation( newLocalisation );
    }
    else if( attributeID == "completion" )
    {
        // $$$$ AGE 2005-02-24: Ca sux ! 
        // $$$$ AGE 2005-02-24: Rajouter un champ dans le FSOM
        double rConstruction;
        deserializer >> rConstruction;
        if( rConstruction > 100 )
        {
            rConstruction -= 100;
            if( ! bPrepared_ )
            {
                bPrepared_ = true;
                NotifyAttributeUpdated( eAttrUpdate_Prepared );
            }
            
        }
        else
        {
            if( bPrepared_ )
            {
                bPrepared_ = false;
                NotifyAttributeUpdated( eAttrUpdate_Prepared );
            }
        }
        ChangeConstructionPercentage( rConstruction );
    }
    else if( attributeID == "valorisation" )
    {
        double rMining;
        deserializer >> rMining;
        ChangeMiningPercentage( rMining );
    }
    else if( attributeID == "contournement" )
        deserializer >> rBypassPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy().GetName() );
    functor.Serialize( "type",  false, GetType().GetName() );
    functor.Serialize( "coordonnees",   ( xAttrToUpdateForHLA_ & eAttrUpdate_Localisation ) != 0,           GetLocalisation() );
    MT_Float rCompletion = rConstructionPercentage_;
    if( bPrepared_ )
        rCompletion += 100;
    bool bUpdateCompletion = ( xAttrToUpdateForHLA_ & ( eAttrUpdate_ConstructionPercentage | eAttrUpdate_Prepared ) ) != 0;
    functor.Serialize( "completion",    bUpdateCompletion, rCompletion );
    functor.Serialize( "valorisation",  ( xAttrToUpdateForHLA_ & eAttrUpdate_MiningPercentage ) != 0,       rMiningPercentage_ );
    functor.Serialize( "contournement", ( xAttrToUpdateForHLA_ & eAttrUpdate_BypassPercentage ) != 0,       rBypassPercentage_ );
    xAttrToUpdateForHLA_ = 0;
}

//=============================================================================
// TOOLS
//=============================================================================

// ----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanInteractWithAgent
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanInteractWithAgent( const MIL_Agent_ABC& agent ) const
{
    if( bPrepared_ || rConstructionPercentage_ == 0 )
        return false;

    if( agent.GetRole< PHY_RoleInterface_Location >().GetHeight() > pType_->GetMaxInteractionHeight() )
        return false;

    return MIL_Object_ABC::CanInteractWithAgent( agent ); // NB : Call MIL_Object_ABC::CanCollideWithAgent()
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBePrepared
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanBePrepared() const
{
    assert( pType_ );    
    return !IsMarkedForDestruction() && pType_->CanBePrepared();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeMined
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanBeMined() const
{
    assert( pType_ );
    return !IsMarkedForDestruction() && pType_->CanBeMined();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeDemined
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanBeDemined() const
{
    assert( pType_ );
    return !IsMarkedForDestruction() && pType_->CanBeMined();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeBypassed
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanBeBypassed() const
{
    assert( pType_ );
    return !IsMarkedForDestruction() && pType_->CanBeBypassed();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetDefaultMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
MT_Float MIL_RealObject_ABC::GetDefaultMaxSpeed() const
{
    assert( pType_ );
    return IsBypassed() ? pType_->GetDefaultSpeedWhenBypassed() : pType_->GetDefaultSpeedWhenNotBypassed();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ApplySpeedPolicy
// Created: NLD 2005-02-17
// -----------------------------------------------------------------------------
MT_Float MIL_RealObject_ABC::ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const
{
    assert( pType_ );
    return pType_->ApplySpeedPolicy( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ApplyAttrition
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::ApplyAttrition( MIL_Agent_ABC& target )
{
    assert( pType_ );
    
    PHY_ObjectExplosionFireResult fireResult( *this );
    target.GetRole< PHY_RoleInterface_Composantes >().ApplyExplosion( *pType_, fireResult );

    // Modifie l'état de construction de l'objet
    if( nFullNbrDotationForConstruction_ && nCurrentNbrDotationForConstruction_ > 0 )
        ChangeConstructionPercentage( (MT_Float)( nCurrentNbrDotationForConstruction_ - 1 ) / (MT_Float)nFullNbrDotationForConstruction_ );
    else
        ChangeConstructionPercentage( 0. );
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::UpdateState
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::UpdateState()
{
    SendMsgUpdate();
    MIL_Object_ABC::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Construct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Construct( MT_Float rDeltaPercentage )
{
    assert( !IsMarkedForDestruction() );
    assert( rDeltaPercentage >= 0. );
    rConstructionPercentage_            = std::max( 0., std::min( 1., rConstructionPercentage_ + rDeltaPercentage ) );
    nCurrentNbrDotationForConstruction_ = (uint)( rConstructionPercentage_ * nFullNbrDotationForConstruction_ );
    NotifyAttributeUpdated( eAttrUpdate_ConstructionPercentage );
    if( pView_ )
        pView_->Construct( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Construct
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Construct()
{
    ChangeConstructionPercentage( 1. );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Prepare
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Prepare( MT_Float rDeltaPercentage )
{   
    assert( !IsMarkedForDestruction() );
    if( !bPrepared_ )
    {
        bPrepared_ = true;
        NotifyAttributeUpdated( eAttrUpdate_Prepared );
        if( pView_ )
            pView_->Prepare();
    }
    Construct( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Destroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Destroy( MT_Float rDeltaPercentage )
{
    assert( !IsMarkedForDestruction() );
    assert( rMiningPercentage_ <= 0. );
    assert( rDeltaPercentage >= 0. );
    rConstructionPercentage_            = std::max( 0., std::min( 1., rConstructionPercentage_ - rDeltaPercentage ) );
    nCurrentNbrDotationForConstruction_ = (uint)( rConstructionPercentage_ * nFullNbrDotationForConstruction_ );
    NotifyAttributeUpdated( eAttrUpdate_ConstructionPercentage );    
    if( rConstructionPercentage_ <= 0. )
        MarkForDestruction();
    if( pView_ )
        pView_->Destroy( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Destroy
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Destroy()
{
    ChangeConstructionPercentage( 0. );
    MarkForDestruction();

    // All the knowledges associated to this object MUST be destroyed (for all the teams ..)
    const MIL_EntityManager::T_ArmyMap& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
    for( MIL_EntityManager::CIT_ArmyMap itArmy = armies.begin(); itArmy != armies.end(); ++itArmy )
        itArmy->second->GetKSObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *this );   
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Mine( MT_Float rDeltaPercentage )
{
    assert( !IsMarkedForDestruction() );
    assert( rDeltaPercentage >= 0. );
    rMiningPercentage_            = std::max( 0., std::min( 1., rMiningPercentage_ + rDeltaPercentage ) );
    nCurrentNbrDotationForMining_ = (uint)( rMiningPercentage_ * nFullNbrDotationForMining_ );
    NotifyAttributeUpdated( eAttrUpdate_MiningPercentage );
    if( pView_ )
        pView_->Mine( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Mine
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Mine()
{
    assert( pType_ );
    
    if ( pType_->CanBeMined() )
        ChangeMiningPercentage( 1. );
}


// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Demine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Demine( MT_Float rDeltaPercentage )
{
    assert( !IsMarkedForDestruction() );
    assert( rDeltaPercentage >= 0. );
    rMiningPercentage_            = std::max( 0., std::min( 1., rMiningPercentage_ - rDeltaPercentage ) );
    nCurrentNbrDotationForMining_ = (uint)( rMiningPercentage_ * nFullNbrDotationForMining_ );
    NotifyAttributeUpdated( eAttrUpdate_MiningPercentage );
    if( pView_ )
        pView_->Demine( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Bypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Bypass( MT_Float rDeltaPercentage )
{
    assert( !IsMarkedForDestruction() );
    assert( rDeltaPercentage >= 0. );
    rBypassPercentage_ = std::max( 0., std::min( 1., rBypassPercentage_ + rDeltaPercentage ) );
    NotifyAttributeUpdated( eAttrUpdate_BypassPercentage );
    if( pView_ )
        pView_->Bypass( rDeltaPercentage );
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Activate
// Created: NLD 2002-12-23
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::Activate()
{
    assert( bPrepared_ );
    assert( rConstructionPercentage_ > 0. );
    bPrepared_ = false;
    NotifyAttributeUpdated( eAttrUpdate_Prepared );
    if( pView_ )
        pView_->Activate();
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination::CanBeAnimatedBy
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanBeAnimatedBy( const MIL_AgentPion& agent ) const
{
    if( IsMarkedForDestruction() || GetConstructionPercentage() < 1. )
        return false;

    return    ( animators_.size() < GetType().GetNbrMaxAnimators() )
           || ( animators_.find( &agent ) != animators_.end() );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::UpdateLocalisation
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::UpdateLocalisation( const TER_Localisation& newLocalisation )
{
    MIL_Object_ABC::UpdateLocalisation( newLocalisation );
    NotifyAttributeUpdated( eAttrUpdate_Localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ProcessAgentInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_Object_ABC::ProcessAgentInside( agent );
    agent.GetRole< PHY_RoleInterface_Location >().NotifyObjectCollision( *this ); 
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_RealObject_ABC::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_Object( teamKnowing, *this );
}

