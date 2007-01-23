//*****************************************************************************
//
// $Created: JDY 03-07-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 15:54 $
// $Revision: 13 $
// $Workfile: ADN_Launchers_Data.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Launchers_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ENT/ENT_Tr.h"
#include <memory.h>

//-----------------------------------------------------------------------------
// Name: LauncherInfos::LauncherInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos::LauncherInfos()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, strName_              ()
, tabModifs_            ( eNbrUnitPosture, eNbrUnitPosture )
, bDirect_              ( false )
, bIndirect_            ( false )
{
    strName_.SetParentNode( *this );
    strName_.SetDataName( "le nom" );
    bIndirect_.SetParentNode( *this );
    bIndirect_.SetDataName( "la capacité de tir indirect" );

    for( int i = 0; i < eNbrUnitPosture; ++i )
    {
        for( int j = 0; j < eNbrUnitPosture; ++j )
        {
            ModifPhsInfos& modifPh = tabModifs_.Get( i, j );
            std::string strModifName( "le modificateur de PH pour les postures (" );
            strModifName += ENT_Tr::ConvertFromUnitPosture( (E_UnitPosture)i, ENT_Tr_ABC::eToTr ) + "," + ENT_Tr::ConvertFromUnitPosture( (E_UnitPosture)j, ENT_Tr_ABC::eToTr ) + ")";
            modifPh.SetDataName( strModifName );
            modifPh.SetParentNode( *this );
            tabModifs_.Get( i, j ) = 0.0;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: LauncherInfos::LauncherInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos::~LauncherInfos()
{
}


// -----------------------------------------------------------------------------
// Name: LauncherInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Launchers_Data::LauncherInfos::GetNodeName()
{
    std::string strResult( "du lanceur " );
    return strResult + strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: LauncherInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Launchers_Data::LauncherInfos::GetItemName()
{
    return strName_.GetData();
}


// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Lanceur" );
    input.ReadAttribute( "nom", strName_ );

    if( input.Section( "Direct", ADN_XmlInput_Helper::eNothing ) )
    {
        bDirect_ = true;
        input.Section( "ModificateursPH" );

        for( int iPostureTireur = 0; iPostureTireur < eNbrUnitPosture; ++iPostureTireur )
        {
            std::string szPostureTireur = "TireurEn" + ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureTireur);
            input.Section( szPostureTireur );

            for( int iPostureCible=0; iPostureCible< eNbrUnitPosture;++iPostureCible)
            {
                std::string szPostureCible = "CibleEn" + ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureCible);

                double rModifier;
                input.ReadField( szPostureCible, rModifier );
                tabModifs_.Get(iPostureTireur,iPostureCible) = rModifier * 100;
            }

            input.EndSection();    // TireurEn...

        }
        input.EndSection();  // ModificateursPH
        input.EndSection();  // Direct
    }

    if( input.Section( "Indirect", ADN_XmlInput_Helper::eNothing ) )
    {
        bIndirect_ = true;
        input.EndSection();
    }

    input.EndSection(); // Lanceur
}


// -----------------------------------------------------------------------------
// Name: LauncherInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Lanceur" );
    output.WriteAttribute( "nom", strName_.GetData() );

    if( bDirect_.GetData() == true )
    {
        output.Section( "Direct" );
        output.Section( "ModificateursPH" );

        for( int iPostureTireur=0; iPostureTireur< eNbrUnitPosture;++iPostureTireur)
        {
            std::string szPostureTireur= "TireurEn" + ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureTireur);
            output.Section( szPostureTireur );

            for( int iPostureCible=0; iPostureCible< eNbrUnitPosture;++iPostureCible)
            {
                std::string strPostureCible= "CibleEn" + ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureCible);
                double rModifier = tabModifs_.Get(iPostureTireur,iPostureCible).GetData() / 100.0;
                output.WriteField( strPostureCible, rModifier );
            }

            output.EndSection();    // TireurEn...

        }
        output.EndSection();  // ModificateursPH
        output.EndSection();  // Direct
    }

    if( bIndirect_.GetData() == true )
    {
        output.Section( "Indirect" );
        output.EndSection();
    }

    output.EndSection(); // Lanceur
}


// -----------------------------------------------------------------------------
// Name: LauncherInfos::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos* ADN_Launchers_Data::LauncherInfos::CreateCopy()
{
    LauncherInfos* pCopy = new LauncherInfos();
    for( uint i = 0; i < eNbrUnitPosture; ++i )
    {
        for( uint j = 0; j < eNbrUnitPosture; ++j )
        {
            pCopy->tabModifs_.Get( i, j ) = tabModifs_.Get( i, j ).GetData();
        }
    }
    pCopy->bDirect_ = this->bDirect_.GetData();
    pCopy->bIndirect_ = this->bIndirect_.GetData();
    return pCopy;
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data constructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_Data::ADN_Launchers_Data()
: ADN_Data_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_Data::~ADN_Launchers_Data()
{
    Reset();
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Launchers_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLaunchers_.GetData());
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::Reset
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
void ADN_Launchers_Data::Reset()
{
    vLaunchers_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Lanceurs" );

    while( input.NextListElement() )
    {
        std::auto_ptr<LauncherInfos> spNew( new LauncherInfos() );
        spNew->ReadArchive( input );
        vLaunchers_.AddItem( spNew.release() );
    }
    input.EndList();    // Lanceurs
}


// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Lanceurs", vLaunchers_.size() );
    for( T_LauncherInfos_Vector::iterator it = vLaunchers_.begin(); it != vLaunchers_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList();    // Lanceurs
}
