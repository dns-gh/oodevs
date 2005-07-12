//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#ifndef __PHY_MeteoDataManager_h_
#define __PHY_MeteoDataManager_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"
#include "Tools/MIL_MOSIDManager.h"

class MT_InputArchive_ABC;
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

public:
     PHY_MeteoDataManager( MIL_InputArchive& archive );
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
    //! @name Types
    //@{
    typedef std::list< PHY_Meteo* >     T_MeteoList;
    typedef T_MeteoList::const_iterator CIT_MeteoList;
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
    T_MeteoList         localMeteos_;    
    PHY_RawVisionData*  pRawData_;        
};


#include "PHY_MeteoDataManager.inl"

#endif // __PHY_MeteoDataManager_h_
