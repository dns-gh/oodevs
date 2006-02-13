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
// $Archive: /MVW_v10/Build/SDK/Light2/src/App.inl $
// $Author: Age $
// $Modtime: 14/03/05 14:01 $
// $Revision: 9 $
// $Workfile: App.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: App::GetApp
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
inline
App& App::GetApp()
{
    assert( pInstance_ != 0 );
	return *pInstance_;
}


//-----------------------------------------------------------------------------
// Name: App::GetMOSServer
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
inline
Network& App::GetNetwork() const
{
    assert( pMOSServer_ );
    return *pMOSServer_;
}


//-----------------------------------------------------------------------------
// Name: App::GetAgentManager
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
AgentManager& App::GetAgentManager() const
{
    assert( pAgentManager_ );
    return *pAgentManager_;
}


//-----------------------------------------------------------------------------
// Name: App::GetLineManager
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
LineManager& App::GetLineManager() const
{
    assert( pLineManager_ );
    return *pLineManager_;
}

//-----------------------------------------------------------------------------
// Name: App::GetObjectManager
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
ObjectManager& App::GetObjectManager() const
{
    assert( pObjectManager_ );
    return *pObjectManager_;
}
 

//-----------------------------------------------------------------------------
// Name: App::GetRawVisionData
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
inline
RawVisionData& App::GetRawVisionData()
{
    return rawData_;
}


//-----------------------------------------------------------------------------
// Name: App::GetWeatherManager
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
Meteo_Manager& App::GetWeatherManager() const
{
    assert( pWeatherManager_ != 0 );
    return *pWeatherManager_;
}


//-----------------------------------------------------------------------------
// Name: App::GetWorld
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
inline
World& App::GetWorld() const
{
    assert( pWorld_ != 0 );
    return *pWorld_;
}


// -----------------------------------------------------------------------------
// Name: App::GetMainWindow
/** @return 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
inline
MainWindow& App::GetMainWindow() const
{
    assert( pMainWindow_ != 0 );
    return *pMainWindow_;
}

//-----------------------------------------------------------------------------
// Name: App::GetTime
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
inline
uint App::GetTime() const
{
    return nTimeSeconds_;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
inline
std::string App::GetTransportDotationName( MIL_AgentID nID ) const
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
// Name: App::GetNBCName
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
inline
std::string App::GetNBCName( uint nId ) const
{
    CIT_MosId_String_Map found = nbcNamesMap_.find( nId );
    if( found != nbcNamesMap_.end() )
        return found->second;
    else
        return itostring( nId );
}


// -----------------------------------------------------------------------------
// Name: App::GetNBCNames
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
inline
const T_MosId_String_Map& App::GetNBCNames() const
{
    return nbcNamesMap_;
}

// -----------------------------------------------------------------------------
// Name: App::GetResourceNames
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
const T_MosId_String_Map& App::GetResourceNames() const
{
    return resourcesNameMap_;
}

// -----------------------------------------------------------------------------
// Name: App::GetEquipmentNames
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
inline
const T_MosId_String_Map& App::GetEquipmentNames() const
{
    return equipementNameMap_;
}

// -----------------------------------------------------------------------------
// Name: App::GetNBCID
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint App::GetNBCID( const std::string& strName ) const
{
    for( CIT_MosId_String_Map it = nbcNamesMap_.begin(); it != nbcNamesMap_.end(); ++it )
        if( it->second == strName )
            return it->first;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: App::FindSensorType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const SensorType* App::FindSensorType( const std::string& strName ) const
{
    CIT_SensorTypeMap it = sensorTypes_.find( strName );
    if( it == sensorTypes_.end() )
        return 0;
    return it->second;
}


// -----------------------------------------------------------------------------
// Name: App::IsODBEdition
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
inline
bool App::IsODBEdition()
{
    return bODBEditor_;
}

// -----------------------------------------------------------------------------
// Name: App::GetRootConfigFile
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
inline
const std::string& App::GetRootConfigFile() const
{
    return strRootConfigFile_;
}

// -----------------------------------------------------------------------------
// Name: App::GetResource
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
inline
const Resource& App::GetResource ( MIL_AgentID id ) const
{
    CIT_ResourceMap it = resourcesMap_.find( id );
    assert( it != resourcesMap_.end() );
    return  it->second;

}