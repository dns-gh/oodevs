//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#ifndef __PHY_MeteoDataManager_h_
#define __PHY_MeteoDataManager_h_

#include "MIL.h"

#include "network/Simulation_Asn.h"

class MIL_Config;
class PHY_RawVisionData;
class PHY_Ephemeride;
class PHY_Meteo;
class MT_Ellipse;
class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 02-10-21
// Last modified: JVT 04-03-25
//*****************************************************************************
class PHY_MeteoDataManager
{
    MT_COPYNOTALLOWED( PHY_MeteoDataManager );

    friend class PHY_Meteo; // For UnregisterMeteo

public:
     PHY_MeteoDataManager( MIL_Config& config );
    ~PHY_MeteoDataManager();
    
    //! @name Raw Data management
    //@{
    const PHY_RawVisionData& GetRawVisionData() const;
    const PHY_Ephemeride&    GetEphemeride   () const;
          void               Update          ();
    //@}

    //! @name Weather effects
    //@{
    void RegisterWeatherEffect  ( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory ); 
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgGlobalMeteo( const ASN1T_MsgCtrlMeteoGlobale& asnMsg );
    void OnReceiveMsgLocalMeteo ( const ASN1T_MsgCtrlMeteoLocale&  asnMsg );
    //@}

private:
    //! @name Registration
    //@{
    void RegisterMeteo  ( PHY_Meteo& meteo );
    void UnregisterMeteo( PHY_Meteo& meteo );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< PHY_Meteo* >     T_MeteoSet;
    typedef T_MeteoSet::const_iterator CIT_MeteoSet;
    //@}

private:
    //! @name Init
    //@{
    void InitializeGlobalMeteo( MIL_InputArchive& archive );
    void InitializeLocalMeteos( MIL_InputArchive& archive );
    //@}

private:
    PHY_Ephemeride*     pEphemeride_;
    PHY_Meteo*          pGlobalMeteo_;
    T_MeteoSet          meteos_;    // Including global meteo
    PHY_RawVisionData*  pRawData_;        
};


#include "PHY_MeteoDataManager.inl"

#endif // __PHY_MeteoDataManager_h_
