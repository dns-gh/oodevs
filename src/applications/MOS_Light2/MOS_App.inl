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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_App.inl $
// $Author: Age $
// $Modtime: 14/03/05 14:01 $
// $Revision: 9 $
// $Workfile: MOS_App.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_App::GetApp
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_App& MOS_App::GetApp()
{
    assert( pInstance_ != 0 );
	return *pInstance_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetMOSServer
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_MOSServer& MOS_App::GetMOSServer() const
{
    assert( pMOSServer_ );
    return *pMOSServer_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetAgentManager
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_AgentManager& MOS_App::GetAgentManager() const
{
    assert( pAgentManager_ );
    return *pAgentManager_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetLineManager
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_LineManager& MOS_App::GetLineManager() const
{
    assert( pLineManager_ );
    return *pLineManager_;
}

//-----------------------------------------------------------------------------
// Name: MOS_App::GetDynaObjectManager
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_DynaObjectManager& MOS_App::GetDynaObjectManager() const
{
    assert( pDynaObjectManager_ );
    return *pDynaObjectManager_;
}
 

//-----------------------------------------------------------------------------
// Name: MOS_App::GetRawVisionData
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_RawVisionData& MOS_App::GetRawVisionData()
{
    return rawData_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetWeatherManager
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Meteo_Manager& MOS_App::GetWeatherManager() const
{
    assert( pWeatherManager_ != 0 );
    return *pWeatherManager_;
}


//-----------------------------------------------------------------------------
// Name: MOS_App::GetWorld
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_World& MOS_App::GetWorld() const
{
    assert( pWorld_ != 0 );
    return *pWorld_;
}


// -----------------------------------------------------------------------------
// Name: MOS_App::GetMainWindow
/** @return 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_MainWindow& MOS_App::GetMainWindow() const
{
    assert( pMainWindow_ != 0 );
    return *pMainWindow_;
}

//-----------------------------------------------------------------------------
// Name: MOS_App::GetTime
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
MOS_INLINE
uint MOS_App::GetTime() const
{
    return nTimeSeconds_;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
MOS_INLINE
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
    default: return "Unknown";
    };
}


// -----------------------------------------------------------------------------
// Name: MOS_App::GetNBCName
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
MOS_INLINE
std::string MOS_App::GetNBCName( uint nId ) const
{
    CIT_MosId_String_Map found = nbcNamesMap_.find( nId );
    if( found != nbcNamesMap_.end() )
        return found->second;
    else
        return itostring( nId );   
}


// -----------------------------------------------------------------------------
// Name: MOS_App::GetNBCNames
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
MOS_INLINE
const T_MosId_String_Map& MOS_App::GetNBCNames() const
{
    return nbcNamesMap_;
}


// -----------------------------------------------------------------------------
// Name: MOS_App::FindSensorType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_SensorType* MOS_App::FindSensorType( const std::string& strName ) const
{
    CIT_SensorTypeMap it = sensorTypes_.find( strName );
    if( it == sensorTypes_.end() )
        return 0;
    return it->second;
}


// -----------------------------------------------------------------------------
// Name: MOS_App::IsODBEdition
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_App::IsODBEdition()
{
    return bODBEditor_;
}