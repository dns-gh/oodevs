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

#include "simulation_kernel_pch.h"

#include "MIL_RealObject_ABC.h"

#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"

#include "MIL_RealObjectType.h"
#include "PHY_ObjectExplosionFireResult.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Tools/MIL_MOSIDManager.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"

#include "Decision/DEC_Tools.h"

#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_DynamicData.h"

#include "HLA/HLA_Object_ABC.h"
#include "HLA/HLA_UpdateFunctor.h"
#include "HLA/Deserializer.h"
#include "HLA/AttributeIdentifier.h"

MT_Random MIL_RealObject_ABC::random_;

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::MIL_RealObject_ABC( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Object_ABC                     ( army )
    , pType_                             ( &type )
    , pObstacleType_                     ( 0 )
    , rSizeCoef_                         ( 0. ) 
    , nFullNbrDotationForConstruction_   ( type.GetNbrDotationForConstruction() ) 
    , nFullNbrDotationForMining_         ( type.GetNbrDotationForMining      () )
    , nCurrentNbrDotationForConstruction_( 0 )
    , nCurrentNbrDotationForMining_      ( 0 )
    , xAttrToUpdate_                     ( eAttrUpdate_All )
    , xAttrToUpdateForHLA_               ( eAttrUpdate_All )
    , nID_                               ( nID )
    , strName_                           ( type.GetName() )
    , rConstructionPercentage_           ( 0. )
    , rMiningPercentage_                 ( 0. )
    , rBypassPercentage_                 ( 0. )
    , nLastValueConstructionPercentage_  ( 0 )
    , nLastValueMiningPercentage_        ( 0 )
    , nLastValueBypassPercentage_        ( 0 )
    , bReservedObstacleActivated_        ( false )
    , avoidanceLocalisation_             ()
    , pPathfindData_                     ( 0 )
    , pOccupier_                         ( 0 )
    , pView_                             ( 0 )
    , rExitingPopulationDensity_         ( type.GetExitingPopulationDensity() )
{
    GetArmy().RegisterObject( *this );
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::MIL_RealObject_ABC()
    : MIL_Object_ABC                     ()
    , pType_                             ( 0 )
    , pObstacleType_                     ( 0 )
    , rSizeCoef_                         ( 0. ) 
    , nFullNbrDotationForConstruction_   ( 0 )
    , nFullNbrDotationForMining_         ( 0 )
    , nCurrentNbrDotationForConstruction_( 0 )
    , nCurrentNbrDotationForMining_      ( 0 )
    , xAttrToUpdate_                     ( eAttrUpdate_All )
    , xAttrToUpdateForHLA_               ( eAttrUpdate_All )
    , nID_                               ( 0 )
    , strName_                           ()
    , rConstructionPercentage_           ( 0. )
    , rMiningPercentage_                 ( 0. )
    , rBypassPercentage_                 ( 0. )
    , nLastValueConstructionPercentage_  ( 0 )
    , nLastValueMiningPercentage_        ( 0 )
    , nLastValueBypassPercentage_        ( 0 )
    , bReservedObstacleActivated_        ( false )
    , avoidanceLocalisation_             ()
    , pPathfindData_                     ( 0 )
    , pOccupier_                         ( 0 )
    , pView_                             ( 0 )
    , rExitingPopulationDensity_         ( std::numeric_limits< MT_Float >::max() )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC destructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_RealObject_ABC::~MIL_RealObject_ABC()
{
    delete pView_;

    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );

    GetArmy().UnregisterObject( *this );

    if( nID_ == 0 )
        return;

    //$$$$ NAZE
    assert( pType_ );
    MIL_MOSIDManager& idManager = pType_->GetIDManager();
    if( idManager.IsMosIDValid( nID_ ) )
        idManager.ReleaseMosID( nID_ );
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
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> rSizeCoef_
         >> nFullNbrDotationForConstruction_
         >> nFullNbrDotationForMining_
         >> nCurrentNbrDotationForConstruction_
         >> nCurrentNbrDotationForMining_
         >> xAttrToUpdate_
         >> xAttrToUpdateForHLA_
         >> nID_;

    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_RealObjectType::Find( nTypeID );
    assert( pType_ );

    uint nObstacleTypeID;
    file >> nObstacleTypeID;
    pObstacleType_ = 0;
    if( nObstacleTypeID != (uint)-1 )
    {
        pObstacleType_ = MIL_ObstacleType::Find( nObstacleTypeID );
        assert( pObstacleType_ );
    }
    file >> strName_
         >> rConstructionPercentage_
         >> rMiningPercentage_
         >> rBypassPercentage_
         >> nLastValueConstructionPercentage_
         >> nLastValueMiningPercentage_
         >> nLastValueBypassPercentage_
         >> bReservedObstacleActivated_
         >> avoidanceLocalisation_
         >> pView_
         >> rExitingPopulationDensity_;

    InitializeAvoidanceLocalisation();
    if ( pType_->GetIDManager().IsMosIDValid( nID_ ) )
    {
        bool bOut = pType_->GetIDManager().LockMosID( nID_ );
        assert( bOut );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    unsigned type     = pType_->GetID(),
             obstacle = ( pObstacleType_ ? pObstacleType_->GetID() : (uint)-1 );
    file << rSizeCoef_
         << nFullNbrDotationForConstruction_
         << nFullNbrDotationForMining_
         << nCurrentNbrDotationForConstruction_
         << nCurrentNbrDotationForMining_
         << xAttrToUpdate_
         << xAttrToUpdateForHLA_
         << nID_
         << type
         << obstacle
         << strName_
         << rConstructionPercentage_
         << rMiningPercentage_
         << rBypassPercentage_
         << nLastValueConstructionPercentage_
         << nLastValueMiningPercentage_
         << nLastValueBypassPercentage_
         << bReservedObstacleActivated_
         << avoidanceLocalisation_
         << pView_
         << rExitingPopulationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    assert( pType_ );

    //$$$ TMP
    if( *pType_ == MIL_RealObjectType::nuageNBC_ )
        return;

    archive.Section( "object" );

    archive.WriteAttribute( "id"      , nID_ );
    archive.WriteAttribute( "name"    , strName_ );
    archive.WriteAttribute( "type"    , pType_  ->GetName() );

    if( pType_->CanBeReservedObstacle() )
    {
        assert( pObstacleType_ );
        archive.WriteAttribute( "obstacle-type", pObstacleType_->GetName() );
        if( pObstacleType_->CouldBeActivated() )
            archive.WriteAttribute( "reserved-obstacle-activated", bReservedObstacleActivated_ );
    }
    
    GetLocalisation().Write( archive );

    WriteSpecificAttributes( archive );

    archive.EndSection(); // object
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
        if( pPathfindData_ )
            TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
        avoidanceLocalisation_.Reset( GetLocalisation() );
        avoidanceLocalisation_.Scale( pType_->GetAvoidanceDistance() );

        const T_PointVector points = avoidanceLocalisation_.GetPoints();
        assert( points.size() > 3 );

        pPathfindData_ = new TER_DynamicData( points, TER_PathFindManager::DefaultTerrainData() );
        TER_PathFindManager::GetPathFindManager().AddDynamicData( *pPathfindData_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::InitializeCommon
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::InitializeCommon( const TER_Localisation& localisation )
{
    MIL_Object_ABC::Initialize( localisation );
    InitializeAvoidanceLocalisation();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Initialize( MIL_InputArchive& archive )
{
    std::string strName;
    archive.ReadAttribute( "name", strName_, MIL_InputArchive::eNothing );
    if( pType_->CanBeReservedObstacle() )
    {
        std::string strObstacleTypeName;
        archive.ReadAttribute( "obstacle-type", strObstacleTypeName );
        pObstacleType_ = MIL_ObstacleType::Find( strObstacleTypeName );
        if( !pObstacleType_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown obstacle type", archive.GetContext() ); 
        if( pObstacleType_->CouldBeActivated() )
            archive.ReadAttribute( "reserved-obstacle-activated", bReservedObstacleActivated_ );
    }

    TER_Localisation localisation;
    localisation.Read( archive );

    InitializeCommon( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_RealObject_ABC::Initialize( const ASN1T_MagicActionCreateObject& asn )
{
    assert( pType_ );    
    TER_Localisation localisation; 
    if( !NET_ASN_Tools::ReadLocation( asn.localisation, localisation ) )
        return EnumObjectErrorCode::error_invalid_specific_attributes;

    if( asn.m.nomPresent ) 
        strName_ = asn.nom;

    if( pType_->CanBeReservedObstacle() )
    {
        if( !asn.m.type_obstaclePresent )
            return EnumObjectErrorCode::error_invalid_localisation;
        pObstacleType_ = MIL_ObstacleType::Find( asn.type_obstacle );
        if( !pObstacleType_ )
            return EnumObjectErrorCode::error_invalid_specific_attributes;
        if( pObstacleType_->CouldBeActivated() )
        {
            if( !asn.m.obstacle_de_manoeuvre_activePresent )
                return EnumObjectErrorCode::error_invalid_specific_attributes;
            bReservedObstacleActivated_ = asn.obstacle_de_manoeuvre_active;
        }
    }
    InitializeCommon( localisation );
    return EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( pType_->CanBeReservedObstacle() )
    {
        pObstacleType_              = &obstacleType;
        bReservedObstacleActivated_ = false;
    }
        
    assert( DEC_Tools::CheckTypeLocalisation( diaParameters[ nCurrentParamIdx ] ) );

    const TER_Localisation* pLocalisation = diaParameters[ nCurrentParamIdx++ ].ToUserPtr( pLocalisation );
    assert( pLocalisation );
    InitializeCommon( *pLocalisation );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::Initialize( const TER_Localisation& localisation, const std::string& /*strOption*/, const std::string& /*strExtra*/, double rCompletion, double rMining, double rBypass )
{
    assert( false );  // $$$$ NLD 2007-05-22: Gerer pObstacleType_ et bReservedObstacleActivated_

    if( pType_->CanBeReservedObstacle() )
    {
        pObstacleType_              = &MIL_ObstacleType::initial_;
        bReservedObstacleActivated_ = false;
    }
    InitializeCommon( localisation );
//    bPrepared_ = rCompletion > 100;
//    if( bPrepared_ )
//        rCompletion -= 100;
    ChangeConstructionPercentage( rCompletion );
    ChangeMiningPercentage      ( rMining );
    rBypassPercentage_ = rBypass; //$$$ POURRI
    NotifyAttributeUpdated( eAttrUpdate_BypassPercentage );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::Initialize
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::Initialize( const TER_Localisation& localisation )
{
    if( pType_->CanBeReservedObstacle() )
    {
        pObstacleType_              = &MIL_ObstacleType::initial_;
        bReservedObstacleActivated_ = false;
    }
    InitializeCommon( localisation );
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendCreation
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendCreation()
{
    if( pView_ && pView_->HideObject() )
        return;

    NET_ASN_MsgObjectCreation asn;

    assert( pType_ );

    asn().oid  = nID_;
    asn().nom  = strName_.c_str(); // !! pointeur sur const char*
    asn().type = pType_->GetAsnID();
    asn().camp = GetArmy().GetID();
    
    NET_ASN_Tools::WriteLocation( GetLocalisation(), asn().localisation );

    if( pObstacleType_ )
    {
        asn().m.type_obstaclePresent = 1;
        asn().type_obstacle          = pObstacleType_->GetAsnID();
        if( pObstacleType_->CouldBeActivated() )
        {
            asn().m.obstacle_de_manoeuvre_activePresent = 1;
            asn().obstacle_de_manoeuvre_active          = bReservedObstacleActivated_;
        }
    }

    if( pType_->GetDotationCategoryForConstruction() )
    {
        asn().m.type_dotation_constructionPresent = 1;
        asn().type_dotation_construction          = pType_->GetDotationCategoryForConstruction()->GetMosID();
    }

    if( pType_->GetDotationCategoryForMining() )
    {
        asn().m.type_dotation_valorisationPresent = 1;
        asn().type_dotation_valorisation          = pType_->GetDotationCategoryForMining()->GetMosID();
    }

    asn().m.attributs_specifiquesPresent = 0;
    WriteSpecificAttributes( asn );
    
    asn.Send();

    NET_ASN_Tools::Delete( asn().localisation );
}


//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendDestruction
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendDestruction()
{
    if( pView_ && pView_->HideObject() )
        return;
    NET_ASN_MsgObjectDestruction asn;
    asn() = nID_;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::SendFullState
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::SendFullState()
{
    NotifyAttributeUpdated( eAttrUpdate_All );
    SendMsgUpdate();
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

    NET_ASN_MsgObjectUpdate asn;
    asn().oid = nID_;

    if( IsAttributeUpdated( eAttrUpdate_ConstructionPercentage, rConstructionPercentage_, nLastValueConstructionPercentage_ ) )
    {
        nLastValueConstructionPercentage_     = (uint)( rConstructionPercentage_ * 100. );
        asn().m.pourcentage_constructionPresent = 1;
        asn().pourcentage_construction          = nLastValueConstructionPercentage_;
        if( pType_->GetDotationCategoryForConstruction() )
        {
            asn().m.nb_dotation_constructionPresent = 1;
            asn().nb_dotation_construction          = nCurrentNbrDotationForConstruction_;
        }
    }

    if( IsAttributeUpdated( eAttrUpdate_MiningPercentage, rMiningPercentage_, nLastValueMiningPercentage_ ) )
    {
        nLastValueMiningPercentage_ = (uint)( rMiningPercentage_ * 100. );
        asn().m.pourcentage_valorisationPresent = 1;
        asn().pourcentage_valorisation          = nLastValueMiningPercentage_;
        if( pType_->GetDotationCategoryForMining() )
        {
            asn().m.nb_dotation_valorisationPresent = 1;
            asn().nb_dotation_valorisation          = nCurrentNbrDotationForMining_;
        }
    }

    if( IsAttributeUpdated( eAttrUpdate_BypassPercentage, rBypassPercentage_, nLastValueBypassPercentage_ ) )
    {
        nLastValueBypassPercentage_ = (uint)( rBypassPercentage_ * 100. );
        asn().m.pourcentage_creation_contournementPresent = 1;
        asn().pourcentage_creation_contournement  = nLastValueBypassPercentage_;
    }

    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
    {
        asn().m.localisationPresent = 1;
        NET_ASN_Tools::WriteLocation( GetLocalisation(), asn().localisation );
    }

    if( xAttrToUpdate_ & eAttrUpdate_SpecificAttributes )
        WriteSpecificAttributes( asn );

    if( xAttrToUpdate_ & eAttrUpdate_ReservedObstacleActivated )
    {
        if( IsReservedObstacle() )
        {
            asn().m.obstacle_de_manoeuvre_activePresent = 1;
            asn().obstacle_de_manoeuvre_active          = bReservedObstacleActivated_;
        }
    }

    xAttrToUpdateForHLA_ = xAttrToUpdate_;
    xAttrToUpdate_ = 0;
    asn.Send();

    if( asn().m.localisationPresent )
        NET_ASN_Tools::Delete( asn().localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::OnReceiveMagicActionUpdate
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_RealObject_ABC::OnReceiveMagicActionUpdate( const ASN1T_MagicActionUpdateObject& asn )
{
    if( asn.m.pourcentage_constructionPresent )
    {
        ChangeConstructionPercentage( asn.pourcentage_construction / 100. );
        NotifyAttributeUpdated( eAttrUpdate_ConstructionPercentage );
    }

    if( asn.m.pourcentage_valorisationPresent )
    {
        ChangeMiningPercentage( asn.pourcentage_valorisation / 100. );
        NotifyAttributeUpdated( eAttrUpdate_MiningPercentage );
    }
        
    if( asn.m.pourcentage_creation_contournementPresent )
    {
        rBypassPercentage_ = asn.pourcentage_creation_contournement / 100.;
        NotifyAttributeUpdated( eAttrUpdate_BypassPercentage );
    }

    if( asn.m.obstacle_de_manoeuvre_activePresent && IsReservedObstacle() )
    {
        bReservedObstacleActivated_ = asn.obstacle_de_manoeuvre_active;
        NotifyAttributeUpdated( eAttrUpdate_ReservedObstacleActivated );
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
           /* if( ! bPrepared_ )
            {
                bPrepared_ = true;
                NotifyAttributeUpdated( eAttrUpdate_Prepared );
            }*/
            
        }
        else
        {
            /*if( bPrepared_ )
            {
                bPrepared_ = false;
                NotifyAttributeUpdated( eAttrUpdate_Prepared );
            }*/
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
//    if( bPrepared_ )
//        rCompletion += 100;
    bool bUpdateCompletion = ( xAttrToUpdateForHLA_ & ( eAttrUpdate_ConstructionPercentage | eAttrUpdate_ReservedObstacleActivated ) ) != 0;
    functor.Serialize( "completion",    bUpdateCompletion, rCompletion );
    functor.Serialize( "valorisation",  ( xAttrToUpdateForHLA_ & eAttrUpdate_MiningPercentage ) != 0,       rMiningPercentage_ );
    functor.Serialize( "contournement", ( xAttrToUpdateForHLA_ & eAttrUpdate_BypassPercentage ) != 0,       rBypassPercentage_ );
    xAttrToUpdateForHLA_ = 0;
}

//=============================================================================
// TOOLS
//=============================================================================

// ----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanInteractWith
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    if( rConstructionPercentage_ == 0 || ( IsReservedObstacle() && !bReservedObstacleActivated_ ) )
        return false;

    if( agent.GetRole< PHY_RoleInterface_Location >().GetHeight() > pType_->GetMaxInteractionHeight() )
        return false;

    return MIL_Object_ABC::CanInteractWith( agent ); // NB : Call MIL_Object_ABC::CanCollideWithEntity()
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanInteractWith
// Created: NLD 2005-12-10
// -----------------------------------------------------------------------------
bool MIL_RealObject_ABC::CanInteractWith( const MIL_Population& population ) const
{
    if( rConstructionPercentage_ == 0 || ( IsReservedObstacle() && !bReservedObstacleActivated_ ) )
        return false;

    return MIL_Object_ABC::CanInteractWith( population ); // NB : Call MIL_Object_ABC::CanCollideWithEntity()
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

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ApplyAttrition
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::ApplyAttrition( MIL_PopulationElement_ABC& target )
{
    assert( pType_ );

    if( pType_->GetPopulationAttritionSurface() > 0. ) //$$$
    {   
        PHY_ObjectExplosionFireResult fireResult( *this );
        target.ApplyExplosion( *pType_, fireResult );

        // Modifie l'état de construction de l'objets
//        if( nFullNbrDotationForConstruction_ && nCurrentNbrDotationForConstruction_ > 0 )
//            ChangeConstructionPercentage( (MT_Float)( nCurrentNbrDotationForConstruction_ - 1 ) / (MT_Float)nFullNbrDotationForConstruction_ );
//        else
//            ChangeConstructionPercentage( 0. );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
MT_Float MIL_RealObject_ABC::GetExitingPopulationDensity() const
{   
    if( IsMarkedForDestruction() || rConstructionPercentage_ == 0 || ( IsReservedObstacle() && !bReservedObstacleActivated_ ) )
        return std::numeric_limits< MT_Float >::max();

    return rExitingPopulationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::ResetExitingPopulationDensity()
{
    assert( pType_ );
    rExitingPopulationDensity_ = pType_->GetExitingPopulationDensity();
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
        itArmy->second->GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *this );   
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
    
    if( pType_->CanBeMined() )
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
    assert( CanBeActivated() );  // $$$$ NLD 2007-05-22: n'importe quoi les asserts
    bReservedObstacleActivated_ = false;
    NotifyAttributeUpdated( eAttrUpdate_ReservedObstacleActivated );
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

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_RealObject_ABC::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    MIL_Object_ABC::ProcessAgentMovingInside( agent );
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


