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
MOS_TypeComposante::MOS_TypeComposante( const std::string& strName, MOS_InputArchive& archive )
    : strName_         ( strName )
    , bHasMaintenance_ ( false )
    , bHasMedical_     ( false )
    , bHasSupply_      ( false )
{
    archive.ReadField( "MosID", nID_ );
    if( !archive.Section( "FonctionsLogistiques", MOS_InputArchive::eNothing ) )
        return;

    if( archive.Section( "Maintenance", MOS_InputArchive::eNothing ) )
    {
        bHasMaintenance_ = true;
        archive.EndSection(); // Maintenance
    }

    if( archive.Section( "Sante", MOS_InputArchive::eNothing ) )
    {
        bHasMedical_ = true;
        archive.EndSection(); // Sante
    }

    if( archive.Section( "Ravitaillement", MOS_InputArchive::eNothing ) )
    {
        bHasSupply_ = true;
        archive.EndSection(); // Ravitaillement
    }

    archive.EndSection(); // FonctionsLogistiques
}

// -----------------------------------------------------------------------------
// Name: MOS_TypeComposante destructor
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
MOS_TypeComposante::~MOS_TypeComposante()
{

}
