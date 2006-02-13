// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypePion.cpp $
// $Author: Age $
// $Modtime: 14/04/05 11:04 $
// $Revision: 3 $
// $Workfile: TypePion.cpp $
//
// *****************************************************************************

#include "astec_pch.h"

#include "TypePion.h"
#include "App.h"
#include "AgentManager.h"
#include "Nature.h"
#include "TypeComposante.h"

// -----------------------------------------------------------------------------
// Name: TypePion constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
TypePion::TypePion( const std::string& strName, InputArchive& archive )
    : strName_( strName )
    , pModel_ ( 0 )
    , pNature_( 0 )
{
    archive.ReadField( "MosID", nID_ );
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );

    pNature_ = new Nature( archive );

    archive.BeginList( "Equipements" );
    while( archive.NextListElement() )
    {
        archive.Section( "Equipement" );
        std::string strName;
        archive.ReadAttribute( "nom", strName );

        const TypeComposante* pTypeComposante = App::GetApp().GetAgentManager().FindTypeComposante( strName );
        if( pTypeComposante )
        {
            bHasMaintenance_ |= pTypeComposante->HasMaintenance();
            bHasMedical_     |= pTypeComposante->HasMedical();
            bHasSupply_      |= pTypeComposante->HasSupply();
        }

        archive.EndSection(); // Equipement
    }
    archive.EndList(); // Equipements
    
  /*  if ( archive.Section( "Stocks", InputArchive::eNothing ) )
    {
        archive.EndSection(); // $$$$ _RC_ HME 2005-11-03: ugly !!!
        archive.BeginList( "Stocks" );
        while( archive.NextListElement() )
        {
            archive.Section( "Dotation" );
            archive.BeginList( "Categories" );
            while( archive.NextListElement() )
            {
                archive.Section( "Categorie" );
                std::string strName;
                archive.ReadAttribute( "nom", strName );
                uint qte = 0;
                archive.ReadAttribute( "contenance", qte );
                stocks_[ App::GetApp().GetRessourceID( strName ) ] = qte;
                archive.EndSection(); //Categorie
            }
            archive.EndList(); //Categories
            archive.EndSection(); //Dotation
        }
        archive.EndList(); //Stocks
    }*/

    pModel_ = App::GetApp().GetAgentManager().FindModel( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
}

// -----------------------------------------------------------------------------
// Name: TypePion constructor
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
TypePion::TypePion( const std::string& strName, const Nature& nature )
    : strName_( strName )
    , pModel_ ( 0 )
    , pNature_( new Nature( nature ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TypePion destructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
TypePion::~TypePion()
{
    delete pNature_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::GetSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char TypePion::GetSymbol() const
{
    return pNature_->symbol_;
}

// -----------------------------------------------------------------------------
// Name: TypePion::GetLevelSymbol
// Created: AGE 2005-04-08
// -----------------------------------------------------------------------------
char TypePion::GetLevelSymbol() const
{
    return pNature_->levelSymbol_;
}
