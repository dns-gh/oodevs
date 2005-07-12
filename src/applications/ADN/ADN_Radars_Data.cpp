// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-05-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Radars_Data.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 10:24 $
// $Revision: 3 $
// $Workfile: ADN_Radars_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Radars_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"


// -----------------------------------------------------------------------------
// Name: DetectTimes::DetectTimes
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Radars_Data::DetectTimes::DetectTimes()
: bDetectTime_      ( false )
, rDetectTime_      ( 0 )
, bIdentTime_       ( false )
, rRecoTime_        ( 0 )
, bRecoTime_        ( false )
, rIdentTime_       ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: DetectTimes::~DetectTimes
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Radars_Data::DetectTimes::~DetectTimes()
{
}


// -----------------------------------------------------------------------------
// Name: DetectTimes::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Radars_Data::DetectTimes::ReadArchive( ADN_XmlInput_Helper& input )
{
    bDetectTime_ = input.ReadTimeField( "DureeDetection", rDetectTime_, ADN_XmlInput_Helper::eNothing );
    bRecoTime_ = input.ReadTimeField( "DureeReconnaissance", rRecoTime_, ADN_XmlInput_Helper::eNothing );
    bIdentTime_ = input.ReadTimeField( "DureeIdentification", rIdentTime_, ADN_XmlInput_Helper::eNothing );
}


// -----------------------------------------------------------------------------
// Name: DetectTimes::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Radars_Data::DetectTimes::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( bDetectTime_.GetData() )
        output.WriteField( "DureeDetection", ADN_Tools::SecondToString( rDetectTime_.GetData() ) );
    if( bRecoTime_.GetData() )
        output.WriteField( "DureeReconnaissance", ADN_Tools::SecondToString( rRecoTime_.GetData() ) );
    if( bIdentTime_.GetData() )
        output.WriteField( "DureeIdentification", ADN_Tools::SecondToString( rIdentTime_.GetData() ) );
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos::RadarInfos()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, strName_              ( tr( "New special sensor" ).ascii() )
, rRange_               ( 0 )
, bHasMaxHeight_        ( false )
, rMaxHeight_           ( 0 )
, bHasMinHeight_        ( false )
, rMinHeight_           ( 0 )
, bHasDetectableActivities_( false )
, bHasDetectTimes_      ( false )
, bHasHQDetectTimes_    ( false )
{
    for( int n = 0; n < eNbrConsumptionType; ++n )
        detectableActivities_[n] = false;
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::~RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos::~RadarInfos()
{
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::GetNodeName
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
std::string ADN_Radars_Data::RadarInfos::GetNodeName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::GetItemName
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
std::string ADN_Radars_Data::RadarInfos::GetItemName()
{
    return std::string( "" );
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::RadarInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Radar" );
    input.ReadAttribute( "nom", strName_ );
    input.ReadAttribute( "type", nType_, ADN_Tr::ConvertToRadarType, ADN_XmlInput_Helper::eThrow );

    input.Section( "Portee" );
    input.ReadField( "RayonAction", rRange_ );
    bHasMinHeight_ = input.ReadField( "HauteurMinimale", rMinHeight_, ADN_XmlInput_Helper::eNothing );
    bHasMaxHeight_ = input.ReadField( "HauteurMaximale", rMaxHeight_, ADN_XmlInput_Helper::eNothing );
    input.EndSection(); // Portee

    if( input.Section( "ActivitePionsDetectables", ADN_XmlInput_Helper::eNothing ) )
    {
        bHasDetectableActivities_ = true;
        for( int n = 0; n < eNbrConsumptionType; ++n )
        {
            if( input.Section( ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)n ), ADN_XmlInput_Helper::eNothing ) )
            {
                detectableActivities_[n] = true;
                input.EndSection();
            }
        }
        input.EndSection(); // ActivitePionsDetectables
    }

    if( input.Section( "DureesAcquisition", ADN_XmlInput_Helper::eNothing ) )
    {
        bHasDetectTimes_ = true;
        detectTimes_.ReadArchive( input );
        if( input.Section( "DureesSpecifiquesPionPC", ADN_XmlInput_Helper::eNothing ) )
        {
            bHasHQDetectTimes_ = true;
            hqDetectTimes_.ReadArchive( input );
            input.EndSection(); // DureesSpecifiquesPionPC
        }
        input.EndSection(); // DureesSpecifiquesPionPC
    }
    input.EndSection(); // Radar
}


// -----------------------------------------------------------------------------
// Name: RadarInfos::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::RadarInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Radar" );
    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromRadarType( nType_.GetData() ) );

    output.Section( "Portee" );
    output.WriteField( "RayonAction", rRange_.GetData() );
    if( bHasMinHeight_.GetData())
        output.WriteField( "HauteurMinimale", rMinHeight_.GetData() );
    if( bHasMaxHeight_.GetData())
        output.WriteField( "HauteurMaximale", rMaxHeight_.GetData() );
    output.EndSection(); // Portee

    if( bHasDetectableActivities_.GetData() ) 
    {
        output.Section( "ActivitePionsDetectables" );
        for( int n = 0; n < eNbrConsumptionType; ++n )
        {
            if( detectableActivities_[n].GetData() )
            {
                output.Section( ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)n ) );
                output.EndSection();
            }
        }
        output.EndSection(); // ActivitePionsDetectables
    }

    if( bHasDetectTimes_.GetData() ) 
    {
        output.Section( "DureesAcquisition" );
        detectTimes_.WriteArchive( output );
        if( bHasHQDetectTimes_.GetData() )
        {
            output.Section( "DureesSpecifiquesPionPC");
            hqDetectTimes_.WriteArchive( output );
            output.EndSection(); // DureesSpecifiquesPionPC
        }
        output.EndSection(); // DureesSpecifiquesPionPC
    }
    output.EndSection(); // Radar
}



// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data constructor
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::ADN_Radars_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data destructor
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::~ADN_Radars_Data()
{
    Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::FilesNeeded
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::FilesNeeded( T_StringList& /*files*/ ) const
{

}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::Reset
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::Reset()
{
    vRadars_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::FindRadar
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos* ADN_Radars_Data::FindRadar( const std::string& strName )
{
    IT_RadarInfos_Vector it = std::find_if( vRadars_.begin(), vRadars_.end(), ADN_Tools::NameCmp<RadarInfos>( strName ) );
    if( it == vRadars_.end() )
        return 0;
    return *it;
}


// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Radars" );
    while( input.NextListElement() )
    {
        std::auto_ptr<RadarInfos> spNew( new RadarInfos() );
        spNew->ReadArchive( input );
        vRadars_.AddItem( spNew.release() );
    }
    input.EndList(); // Radars
}


// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Radars", vRadars_.size() );
    for( IT_RadarInfos_Vector it = vRadars_.begin(); it != vRadars_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // Senseurs
}
