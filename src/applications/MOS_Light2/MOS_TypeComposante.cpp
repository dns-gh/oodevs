// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeComposante.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeComposante.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_TypeComposante.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante constructor
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
MOS_TypeComposante::MOS_TypeComposante( const std::string& strName, MT_InputArchive_ABC& archive )
    : strName_         ( strName )
    , bHasMaintenance_ ( false )
    , bHasMedical_     ( false )
    , bHasSupply_      ( false )
{
    if( !archive.ReadField( "MosID", nID_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    //archive.EnableExceptions( false );
    if( !archive.Section( "FonctionsLogistiques" ) )
    {
        archive.ClearLastError();
        return;
    }

    if( archive.Section( "Maintenance" ) )
    {
        bHasMaintenance_ = true;
        archive.EndSection(); // Maintenance
    }
    else
        archive.ClearLastError();

    if( archive.Section( "Sante" ) )
    {
        bHasMedical_ = true;
        archive.EndSection(); // Sante
    }
    else
        archive.ClearLastError();

    if( archive.Section( "Ravitaillement" ) )
    {
        bHasSupply_ = true;
        archive.EndSection(); // Ravitaillement
    }
    else
        archive.ClearLastError();

    archive.EndSection(); // FonctionsLogistiques
    //archive.EnableExceptions( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante destructor
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
MOS_TypeComposante::~MOS_TypeComposante()
{

}
