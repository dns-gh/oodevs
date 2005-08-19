// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypePion.cpp $
// $Author: Age $
// $Modtime: 14/04/05 11:04 $
// $Revision: 3 $
// $Workfile: MOS_TypePion.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_TypePion.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_Nature.h"
#include "MOS_TypeComposante.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypePion constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypePion::MOS_TypePion( const std::string& strName, MT_InputArchive_ABC& archive )
    : strName_( strName )
    , pModel_ ( 0 )
    , pNature_( 0 )
{
    if( !archive.ReadField( "MosID", nID_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strModel;
    if( !archive.ReadField( "ModeleDecisionnel", strModel ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    pNature_ = new MOS_Nature( archive );

    if( !archive.BeginList( "Equipements" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    while( archive.NextListElement() )
    {
        if( !archive.Section( "Equipement" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        std::string strName;
        if( !archive.ReadAttribute( "nom", strName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

        const MOS_TypeComposante* pTypeComposante = MOS_App::GetApp().GetAgentManager().FindTypeComposante( strName );
        if( pTypeComposante )
        {
            bHasMaintenance_ |= pTypeComposante->HasMaintenance();
            bHasMedical_     |= pTypeComposante->HasMedical();
            bHasSupply_      |= pTypeComposante->HasSupply();
        }

        archive.EndSection(); // Equipement
    }
    archive.EndList(); // Equipements

    pModel_ = MOS_App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion constructor
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
MOS_TypePion::MOS_TypePion( const std::string& strName, const MOS_Nature& nature )
    : strName_( strName )
    , pModel_ ( 0 )
    , pNature_( new MOS_Nature( nature ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion destructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_TypePion::~MOS_TypePion()
{
    delete pNature_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_TypePion::GetSymbol() const
{
    return pNature_->symbol_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePion::GetLevelSymbol
// Created: AGE 2005-04-08
// -----------------------------------------------------------------------------
char MOS_TypePion::GetLevelSymbol() const
{
    return pNature_->levelSymbol_;
}
