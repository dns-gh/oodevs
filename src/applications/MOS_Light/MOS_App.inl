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
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_App.inl $
// $Author: Nld $
// $Modtime: 1/04/05 18:52 $
// $Revision: 19 $
// $Workfile: MOS_App.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_App::GetApp
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
inline
MOS_App& MOS_App::GetApp()
{
    assert( pTheApp_ );
	return *pTheApp_;
}

//-----------------------------------------------------------------------------
// Name: MOS_App::CreateApp
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
void MOS_App::CreateApp( int nArgc, char** ppArgv )
{
    assert( pTheApp_ == 0 );
    pTheApp_ = new MOS_App( nArgc, ppArgv);
}

//-----------------------------------------------------------------------------
// Name: MOS_App::DestroyApp
// Created: NLD 2002-09-09
//-----------------------------------------------------------------------------
inline
void MOS_App::DestroyApp()
{
    if( pTheApp_ )
    {
        delete pTheApp_;
        pTheApp_ = 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_App::GetMOSServer
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
inline
MOS_MOSServer& MOS_App::GetMOSServer() const
{
    assert( pMOSServer_ );
    return *pMOSServer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_App::GetMainWindow
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MOS_MainWindow& MOS_App::GetMainWindow() const
{
    assert( pMainWindow_ );
    return *pMainWindow_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetAgentManager
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MOS_AgentManager& MOS_App::GetAgentManager() const
{
    assert( pAgentManager_ );
    return *pAgentManager_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetTestManager
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
inline
MOS_TestManager& MOS_App::GetTestManager() const
{
    assert( pTestManager_ );
    return *pTestManager_;
}



//-----------------------------------------------------------------------------
// Name: MOS_App::GetLineManager
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_LineManager& MOS_App::GetLineManager() const
{
    assert( pLineManager_ );
    return *pLineManager_;
}

//-----------------------------------------------------------------------------
// Name: MOS_App::GetDynaObjectManager
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
MOS_DynaObjectManager& MOS_App::GetDynaObjectManager() const
{
    assert( pDynaObjectManager_ );
    return *pDynaObjectManager_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetWorld
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
MOS_World& MOS_App::GetWorld() const
{
    assert( pWorld_ );
    return *pWorld_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetTime
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
inline
int MOS_App::GetTime() const
{
    return nTimeSeconds_;
}
    

//-----------------------------------------------------------------------------
// Name: MOS_App::GetRawVisionData
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
inline
MOS_RawVisionData& MOS_App::GetRawVisionData()
{
    return rawData_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetWeatherManager
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
MOS_Meteo_Manager& MOS_App::GetWeatherManager() const
{
    assert( pWeatherManager_ != 0 );
    return *pWeatherManager_;
}

/*
//-----------------------------------------------------------------------------
// Name: std::string 
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
inline
std::string MOS_App::GetTransportDotationName( MIL_AgentID nID ) const
{
    switch ( nID )
    {
    case 0 : return "munition";
    case 1 : return "carburant";
    case 2 : return "mine";
    case 3 : return "explosif";
    case 4 : return "barbele";
    case 5 : return "humain";
    case 6 : return "composante";
    case 7 : return "piece de rechange";
    default: return "Arrete de dire des conneries";
    };
}*/

// -----------------------------------------------------------------------------
// Name: MOS_App::FindSensorType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const MOS_SensorType* MOS_App::FindSensorType( const std::string& strName ) const
{
    CIT_SensorTypeMap it = sensorTypes_.find( strName );
    if( it == sensorTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetEquipements
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
const MOS_App::T_MosId_String_Map& MOS_App::GetEquipements() const
{
    return equipementNameMap_;
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetRessources
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
inline
const MOS_App::T_MosId_String_Map& MOS_App::GetRessources() const
{
    return ressourcesNameMap_;
}

// -----------------------------------------------------------------------------
// Name: MOS_App::GetRandomAgentNBC
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
inline
uint MOS_App::GetRandomAgentNBC() const
{
    assert( !nbcAgentsMap_.empty() );
    return nbcAgentsMap_.begin()->first;
}
