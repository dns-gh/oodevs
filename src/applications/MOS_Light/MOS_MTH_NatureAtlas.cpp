// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_NatureAtlas.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 12:16 $
// $Revision: 4 $
// $Workfile: MOS_MTH_NatureAtlas.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_MTH_NatureAtlas.h"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_NatureAtlas.inl"
#endif

#include <qcheckbox.h>
#include <qbuttongroup.h>

// -----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas constructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_NatureAtlas::MOS_MTH_NatureAtlas( ASN1T_NatureAtlas& asnNatureAtlas, QWidget* pParent, const std::string& strName )
    : QWidget             ( pParent )
    , MOS_MissionTool_ABC ()
    , pAsnNatureAtlas_    ( &asnNatureAtlas )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, strName );
    pLocalLayout->addWidget( pLabel );


    QButtonGroup* pButtonGroup = new QButtonGroup( this, "Nature atlas" );

    pNatureAtlasBlindeCheckBox_         = new QCheckBox( "Blindé"          , pButtonGroup );
    pNatureAtlasASSCheckBox_            = new QCheckBox( "ASS"             , pButtonGroup );
    pNatureAtlasPCCheckBox_             = new QCheckBox( "PC"              , pButtonGroup );
    pNatureAtlasLogCheckBox_            = new QCheckBox( "Log"             , pButtonGroup );
    pNatureAtlasMortierCheckBox_        = new QCheckBox( "Mortier"         , pButtonGroup );
    pNatureAtlasLanceRoquetteCheckBox_  = new QCheckBox( "Lance roquette"  , pButtonGroup );
    pNatureAtlasVehiculeCheckBox_       = new QCheckBox( "Vehicule"        , pButtonGroup );
    pNatureAtlasPersonnelAPiedCheckBox_ = new QCheckBox( "Personnal à pied", pButtonGroup );
    pNatureAtlasHelicoptereCheckBox_    = new QCheckBox( "Helicoptère"     , pButtonGroup );
    pNatureAtlasIndefiniCheckBox_       = new QCheckBox( "Indéfini"        , pButtonGroup );
    pNatureAtlasASACheckBox_            = new QCheckBox( "ASA"             , pButtonGroup );
    pNatureAtlasNoneCheckBox_           = new QCheckBox( "None"            , pButtonGroup );

    QGridLayout* pGridLayout = new QGridLayout( pButtonGroup, 4, 3 );
    pGridLayout->addWidget( pNatureAtlasBlindeCheckBox_        , 0, 0 );
    pGridLayout->addWidget( pNatureAtlasASSCheckBox_           , 1, 0 );
    pGridLayout->addWidget( pNatureAtlasPCCheckBox_            , 2, 0 );
    pGridLayout->addWidget( pNatureAtlasLogCheckBox_           , 3, 0 );
    pGridLayout->addWidget( pNatureAtlasMortierCheckBox_       , 0, 1 );
    pGridLayout->addWidget( pNatureAtlasLanceRoquetteCheckBox_ , 1, 1 );
    pGridLayout->addWidget( pNatureAtlasVehiculeCheckBox_      , 2, 1 );
    pGridLayout->addWidget( pNatureAtlasPersonnelAPiedCheckBox_, 3, 1 );
    pGridLayout->addWidget( pNatureAtlasHelicoptereCheckBox_   , 0, 2 );
    pGridLayout->addWidget( pNatureAtlasIndefiniCheckBox_      , 1, 2 );
    pGridLayout->addWidget( pNatureAtlasASACheckBox_           , 2, 2 );    
    pGridLayout->addWidget( pNatureAtlasNoneCheckBox_          , 3, 2 );

    pLocalLayout->addWidget( pButtonGroup );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas destructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_NatureAtlas::~MOS_MTH_NatureAtlas()
{

}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::Terminate
// Created: FBD 02-12-17
//---------------------------------------------------------------------bi--------
void MOS_MTH_NatureAtlas::Terminate()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::WriteMsg(  std::string& sParam )
{
    assert( pAsnNatureAtlas_ );

    sParam += "NatureAtlas $$$$";

    pAsnNatureAtlas_->numbits = 11;
    pAsnNatureAtlas_->data[0] = 0;
    pAsnNatureAtlas_->data[1] = 0;

    pAsnNatureAtlas_->data[BytXblinde          ] |= ( pNatureAtlasBlindeCheckBox_        ->isChecked() ? BitMblinde : 0             );
    pAsnNatureAtlas_->data[BytXass             ] |= ( pNatureAtlasASSCheckBox_           ->isChecked() ? BitMass : 0                );
    pAsnNatureAtlas_->data[BytXpc              ] |= ( pNatureAtlasPCCheckBox_            ->isChecked() ? BitMpc : 0                 );
    pAsnNatureAtlas_->data[BytXlog             ] |= ( pNatureAtlasLogCheckBox_           ->isChecked() ? BitMlog : 0                );
    pAsnNatureAtlas_->data[BytXmortier         ] |= ( pNatureAtlasMortierCheckBox_       ->isChecked() ? BitMmortier : 0            );
    pAsnNatureAtlas_->data[BytXlance_roquette  ] |= ( pNatureAtlasLanceRoquetteCheckBox_ ->isChecked() ? BitMlance_roquette : 0     );
    pAsnNatureAtlas_->data[BytXvehicule        ] |= ( pNatureAtlasVehiculeCheckBox_      ->isChecked() ? BitMvehicule : 0           );
    pAsnNatureAtlas_->data[BytXpersonnel_a_pied] |= ( pNatureAtlasPersonnelAPiedCheckBox_->isChecked() ? BitMpersonnel_a_pied : 0   );
    pAsnNatureAtlas_->data[BytXhelicoptere     ] |= ( pNatureAtlasHelicoptereCheckBox_   ->isChecked() ? BitMhelicoptere : 0        );
    pAsnNatureAtlas_->data[BytXindefini        ] |= ( pNatureAtlasIndefiniCheckBox_      ->isChecked() ? BitMindefini : 0           );
    pAsnNatureAtlas_->data[BytXasa             ] |= ( pNatureAtlasASACheckBox_           ->isChecked() ? BitMasa : 0                );
    pAsnNatureAtlas_->data[BytXnone            ] |= ( pNatureAtlasNoneCheckBox_          ->isChecked() ? BitMnone : 0               );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::FillRandomParameters()
{
    pNatureAtlasBlindeCheckBox_        ->setChecked( rand() % 2 );
    pNatureAtlasASSCheckBox_           ->setChecked( rand() % 2 );
    pNatureAtlasPCCheckBox_            ->setChecked( rand() % 2 );
    pNatureAtlasLogCheckBox_           ->setChecked( rand() % 2 );
    pNatureAtlasMortierCheckBox_       ->setChecked( rand() % 2 );
    pNatureAtlasLanceRoquetteCheckBox_ ->setChecked( rand() % 2 );
    pNatureAtlasVehiculeCheckBox_      ->setChecked( rand() % 2 );
    pNatureAtlasPersonnelAPiedCheckBox_->setChecked( rand() % 2 );
    pNatureAtlasHelicoptereCheckBox_   ->setChecked( rand() % 2 );
    pNatureAtlasIndefiniCheckBox_      ->setChecked( rand() % 2 );
    pNatureAtlasNoneCheckBox_          ->setChecked( rand() % 2 );
    pNatureAtlasASACheckBox_           ->setChecked( rand() % 2 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    archive.WriteField( "sName", std::string( "MOS_MTH_NatureAtlas" ) );
    
    archive.WriteField( "NatureAtlasBlinde"        , pNatureAtlasBlindeCheckBox_        ->isChecked() );
    archive.WriteField( "NatureAtlasASS"           , pNatureAtlasASSCheckBox_           ->isChecked() );
    archive.WriteField( "NatureAtlasPC"            , pNatureAtlasPCCheckBox_            ->isChecked() );
    archive.WriteField( "NatureAtlasLog"           , pNatureAtlasLogCheckBox_           ->isChecked() );
    archive.WriteField( "NatureAtlasMortier"       , pNatureAtlasMortierCheckBox_       ->isChecked() );
    archive.WriteField( "NatureAtlasLanceRoquette" , pNatureAtlasLanceRoquetteCheckBox_ ->isChecked() );
    archive.WriteField( "NatureAtlasVehicule"      , pNatureAtlasVehiculeCheckBox_      ->isChecked() );
    archive.WriteField( "NatureAtlasPersonnelAPied", pNatureAtlasPersonnelAPiedCheckBox_->isChecked() );
    archive.WriteField( "NatureAtlasHelicoptere"   , pNatureAtlasHelicoptereCheckBox_   ->isChecked() );
    archive.WriteField( "NatureAtlasIndefini"      , pNatureAtlasIndefiniCheckBox_      ->isChecked() );
    archive.WriteField( "NatureAtlasNone"          , pNatureAtlasNoneCheckBox_          ->isChecked() );
    archive.WriteField( "NatureAtlasASA"           , pNatureAtlasASACheckBox_           ->isChecked() );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::ReloadParameters( MT_InputArchive_ABC& archive )
{
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_NatureAtlas" );
// -------------------------------------------------

    bool bTmp;
    archive.ReadField( "NatureAtlasBlinde"        , bTmp );
    pNatureAtlasBlindeCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasASS"           , bTmp );
    pNatureAtlasASSCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasPC"            , bTmp );
    pNatureAtlasPCCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasLog"           , bTmp );
    pNatureAtlasLogCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasMortier"       , bTmp );
    pNatureAtlasMortierCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasLanceRoquette" , bTmp );
    pNatureAtlasLanceRoquetteCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasVehicule"      , bTmp );
    pNatureAtlasVehiculeCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasPersonnelAPied", bTmp );
    pNatureAtlasPersonnelAPiedCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasHelicoptere"   , bTmp );
    pNatureAtlasHelicoptereCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasIndefini"      , bTmp );
    pNatureAtlasIndefiniCheckBox_->setChecked( bTmp );
    archive.ReadField( "NatureAtlasNone"          , bTmp );
    pNatureAtlasNoneCheckBox_->setChecked( bTmp );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::SetAgent( MOS_Agent* /*pAgent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}   

//-----------------------------------------------------------------------------
// Name: MOS_MTH_NatureAtlas::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_NatureAtlas::SetWidgetTab( QWidget* /*pWidget*/ )
{
}

