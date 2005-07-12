//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Dialog_ABC.cpp $
// $Author: Fbd $
// $Modtime: 15/01/03 11:13 $
// $Revision: 5 $
// $Workfile: MOS_Dialog_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_Dialog_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Dialog_ABC.inl"
#endif

#include <qcombobox.h>


//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC constructor
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
MOS_Dialog_ABC::MOS_Dialog_ABC( QWidget* pParent )
   : QDialog( pParent, NULL, true ) 
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC destructor
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
MOS_Dialog_ABC::~MOS_Dialog_ABC()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC::Initialize
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_Dialog_ABC::Initialize()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC::Cleanup
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_Dialog_ABC::Cleanup()
{
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC::InitPositionComboBox
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
void MOS_Dialog_ABC::InitPositionComboBox( QComboBox& comboBox )
{
    comboBox.insertItem( "30TYT0662568272 ( 100,100 )" );
    comboBox.insertItem( "30TYT0656669939 ( 100,200 )" );
    comboBox.insertItem( "30TYT0650771606 ( 100,300 )" );
    comboBox.insertItem( "30TYT0644873273 ( 100,400 )" );
    comboBox.insertItem( "30TYT0638974940 ( 100,500 )" );
    comboBox.insertItem( "30TYT0633076607 ( 100,600 )" );
    comboBox.insertItem( "30TYT0627178274 ( 100,700 )" );
    comboBox.insertItem( "30TYT0621279941 ( 100,800 )" );
    comboBox.insertItem( "30TYT0615381608 ( 100,900 )" );
    comboBox.insertItem( "30TYT0609383274 ( 100,1000 )" );
    comboBox.insertItem( "30TYT0782268315 ( 200,100 )" );
    comboBox.insertItem( "30TYT0776369982 ( 200,200 )" );
    comboBox.insertItem( "30TYT0770371649 ( 200,300 )" );
    comboBox.insertItem( "30TYT0764473315 ( 200,400 )" );
    comboBox.insertItem( "30TYT0758574982 ( 200,500 )" );
    comboBox.insertItem( "30TYT0752576649 ( 200,600 )" );
    comboBox.insertItem( "30TYT0746678316 ( 200,700 )" );
    comboBox.insertItem( "30TYT0740679983 ( 200,800 )" );
    comboBox.insertItem( "30TYT0734781650 ( 200,900 )" );
    comboBox.insertItem( "30TYT0728883317 ( 200,1000 )" );
    comboBox.insertItem( "30TYT0901968358 ( 300,100 )" );
    comboBox.insertItem( "30TYT0896070024 ( 300,200 )" );
    comboBox.insertItem( "30TYT0890071691 ( 300,300 )" );
    comboBox.insertItem( "30TYT0884073358 ( 300,400 )" );
    comboBox.insertItem( "30TYT0878175025 ( 300,500 )" );
    comboBox.insertItem( "30TYT0872176692 ( 300,600 )" );
    comboBox.insertItem( "30TYT0866178359 ( 300,700 )" );
    comboBox.insertItem( "30TYT0860180026 ( 300,800 )" );
    comboBox.insertItem( "30TYT0854281693 ( 300,900 )" );
    comboBox.insertItem( "30TYT0848283360 ( 300,1000 )" );
    comboBox.insertItem( "30TYT1021668400 ( 400,100 )" );
    comboBox.insertItem( "30TYT1015670067 ( 400,200 )" );
    comboBox.insertItem( "30TYT1009671734 ( 400,300 )" );
    comboBox.insertItem( "30TYT1003673401 ( 400,400 )" );
    comboBox.insertItem( "30TYT0997675068 ( 400,500 )" );
    comboBox.insertItem( "30TYT0991676735 ( 400,600 )" );
    comboBox.insertItem( "30TYT0985678402 ( 400,700 )" );
    comboBox.insertItem( "30TYT0979680069 ( 400,800 )" );
    comboBox.insertItem( "30TYT0973681736 ( 400,900 )" );
    comboBox.insertItem( "30TYT0967683403 ( 400,1000 )" );
    comboBox.insertItem( "30TYT1141468444 ( 500,100 )" );
    comboBox.insertItem( "30TYT1135370111 ( 500,200 )" );
    comboBox.insertItem( "30TYT1129371777 ( 500,300 )" );
    comboBox.insertItem( "30TYT1123373444 ( 500,400 )" );
    comboBox.insertItem( "30TYT1117275111 ( 500,500 )" );
    comboBox.insertItem( "30TYT1111276778 ( 500,600 )" );
    comboBox.insertItem( "30TYT1105178445 ( 500,700 )" );
    comboBox.insertItem( "30TYT1099180112 ( 500,800 )" );
    comboBox.insertItem( "30TYT1093181779 ( 500,900 )" );
    comboBox.insertItem( "30TYT1087083446 ( 500,1000 )" );
    comboBox.insertItem( "30TYT1261168487 ( 600,100 )" );
    comboBox.insertItem( "30TYT1255070154 ( 600,200 )" );
    comboBox.insertItem( "30TYT1249071821 ( 600,300 )" );
    comboBox.insertItem( "30TYT1242973488 ( 600,400 )" );
    comboBox.insertItem( "30TYT1236875155 ( 600,500 )" );
    comboBox.insertItem( "30TYT1230776822 ( 600,600 )" );
    comboBox.insertItem( "30TYT1224778488 ( 600,700 )" );
    comboBox.insertItem( "30TYT1218680155 ( 600,800 )" );
    comboBox.insertItem( "30TYT1212581822 ( 600,900 )" );
    comboBox.insertItem( "30TYT1206483489 ( 600,1000 )" );
    comboBox.insertItem( "30TYT1380868531 ( 700,100 )" );
    comboBox.insertItem( "30TYT1374770198 ( 700,200 )" );
    comboBox.insertItem( "30TYT1368671865 ( 700,300 )" );
    comboBox.insertItem( "30TYT1362573531 ( 700,400 )" );
    comboBox.insertItem( "30TYT1356475198 ( 700,500 )" );
    comboBox.insertItem( "30TYT1350376865 ( 700,600 )" );
    comboBox.insertItem( "30TYT1344278532 ( 700,700 )" );
    comboBox.insertItem( "30TYT1338180199 ( 700,800 )" );
    comboBox.insertItem( "30TYT1332081866 ( 700,900 )" );
    comboBox.insertItem( "30TYT1325883533 ( 700,1000 )" );
    comboBox.insertItem( "30TYT1500568575 ( 800,100 )" );
    comboBox.insertItem( "30TYT1494470242 ( 800,200 )" );
    comboBox.insertItem( "30TYT1488371909 ( 800,300 )" );
    comboBox.insertItem( "30TYT1482173575 ( 800,400 )" );
    comboBox.insertItem( "30TYT1476075242 ( 800,500 )" );
    comboBox.insertItem( "30TYT1469876909 ( 800,600 )" );
    comboBox.insertItem( "30TYT1463778576 ( 800,700 )" );
    comboBox.insertItem( "30TYT1457680243 ( 800,800 )" );
    comboBox.insertItem( "30TYT1451481910 ( 800,900 )" );
    comboBox.insertItem( "30TYT1445383577 ( 800,1000 )" );
    comboBox.insertItem( "30TYT1620368619 ( 900,100 )" );
    comboBox.insertItem( "30TYT1614170286 ( 900,200 )" );
    comboBox.insertItem( "30TYT1607971953 ( 900,300 )" );
    comboBox.insertItem( "30TYT1601873620 ( 900,400 )" );
    comboBox.insertItem( "30TYT1595675287 ( 900,500 )" );
    comboBox.insertItem( "30TYT1589476953 ( 900,600 )" );
    comboBox.insertItem( "30TYT1583278620 ( 900,700 )" );
    comboBox.insertItem( "30TYT1577080287 ( 900,800 )" );
    comboBox.insertItem( "30TYT1570981954 ( 900,900 )" );
    comboBox.insertItem( "30TYT1564783621 ( 900,1000 )" );
    comboBox.insertItem( "30TYT1740068663 ( 1000,100 )" );
    comboBox.insertItem( "30TYT1733870330 ( 1000,200 )" );
    comboBox.insertItem( "30TYT1727671997 ( 1000,300 )" );
    comboBox.insertItem( "30TYT1721473664 ( 1000,400 )" );
    comboBox.insertItem( "30TYT1715275331 ( 1000,500 )" );
    comboBox.insertItem( "30TYT1709076998 ( 1000,600 )" );
    comboBox.insertItem( "30TYT1702778665 ( 1000,700 )" );
    comboBox.insertItem( "30TYT1696580332 ( 1000,800 )" );
    comboBox.insertItem( "30TYT1690381998 ( 1000,900 )" );
    comboBox.insertItem( "30TYT1684183665 (1000,1000 )" );
    
    comboBox.setAutoCompletion( TRUE );
}


//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC::InitItineraryComboBox
// Created: NLD 2002-09-25
//-----------------------------------------------------------------------------
void MOS_Dialog_ABC::InitItineraryComboBox( QComboBox& comboBox )
{
    comboBox.insertItem( "Iti 1" );
    comboBox.insertItem( "Iti 2" );
    comboBox.setAutoCompletion( TRUE );
}


//-----------------------------------------------------------------------------
// Name: MOS_Dialog_ABC::ConvertPositionValue
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
std::string& MOS_Dialog_ABC::ConvertPositionValue( std::string& strValue )
{
    strValue.erase( std::find( strValue.begin(), strValue.end(), ' ' ), strValue.end() );
    assert( strValue.size() == 15 );
    return strValue;
}