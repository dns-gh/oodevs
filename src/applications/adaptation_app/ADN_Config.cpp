//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_DialogLog.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_DialogLog.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Config.h"
#include "ADN_ConfigDialog.h"

// -----------------------------------------------------------------------------
// Name: ADN_Config constructor
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
ADN_Config::ADN_Config()
    : pDialog_        ( 0 )
    , strSimPath_     ()
    , strSimArguments_()
{
    Load();
}

// -----------------------------------------------------------------------------
// Name: ADN_Config destructor
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
ADN_Config::~ADN_Config()
{
    Save();
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::Configure
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_Config::Configure()
{
    pDialog_ = new ADN_ConfigDialog( *this );
    pDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::Save
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_Config::Save()
{
    // $$$$ SBO 2006-01-02: todo: save to registry
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::Load
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_Config::Load()
{
    // $$$$ SBO 2006-01-02: todo: load from registry
    strSimPath_      = "./simulation_app.exe";
    strSimArguments_ = "--testdata";
}
