// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeComposante.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeComposante.cpp $
//
// *****************************************************************************


#include "astec_pch.h"

#include "TypeComposante.h"
#include "App.h"
#include "AgentManager.h"

// -----------------------------------------------------------------------------
// Name: TypeComposante constructor
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
TypeComposante::TypeComposante( const std::string& strName, InputArchive& archive )
    : strName_         ( strName )
    , bHasMaintenance_ ( false )
    , bHasMedical_     ( false )
    , bHasSupply_      ( false )
{
    archive.ReadField( "MosID", nID_ );
    if( !archive.Section( "FonctionsLogistiques", InputArchive::eNothing ) )
        return;

    if( archive.Section( "Maintenance", InputArchive::eNothing ) )
    {
        bHasMaintenance_ = true;
        archive.EndSection(); // Maintenance
    }

    if( archive.Section( "Sante", InputArchive::eNothing ) )
    {
        bHasMedical_ = true;
        archive.EndSection(); // Sante
    }

    if( archive.Section( "Ravitaillement", InputArchive::eNothing ) )
    {
        bHasSupply_ = true;
        archive.EndSection(); // Ravitaillement
    }

    archive.EndSection(); // FonctionsLogistiques
}

// -----------------------------------------------------------------------------
// Name: TypeComposante destructor
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
TypeComposante::~TypeComposante()
{

}
