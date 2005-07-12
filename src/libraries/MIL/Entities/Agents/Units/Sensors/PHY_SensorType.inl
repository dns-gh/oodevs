// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:12 $
// $Revision: 2 $
// $Workfile: PHY_SensorType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::FindSensorType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const PHY_SensorType* PHY_SensorType::FindSensorType( const std::string& strType )
{
    CIT_SensorTypeMap it = sensorTypes_.find( strType );

    return it == sensorTypes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_SensorType::FindSensorType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_SensorType* PHY_SensorType::FindSensorType( const uint nID )
{ 
    // $$$$ JVT : Recherche linéaire, mais n'est utilisé que lors de la reprise de la sim depuis un checkpoint
    CIT_SensorTypeMap it = std::find_if( sensorTypes_.begin(), sensorTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_SensorType::GetID ), std::select2nd< T_SensorTypeMap::value_type >() ) ) );

    return it == sensorTypes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetTypeObject
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
const PHY_SensorTypeObject* PHY_SensorType::GetTypeObject() const
{
    return pTypeObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetTypeAgent
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
const PHY_SensorTypeAgent* PHY_SensorType::GetTypeAgent() const
{
    return pTypeAgent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const std::string& PHY_SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
uint PHY_SensorType::GetID() const
{
    return nID_;
}

