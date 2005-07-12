//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Manager.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Manager.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Manager_h_
#define __MOS_Meteo_Manager_h_

#include "MOS_Meteo_ABC.h"

class MOS_Meteo_Local;

//*****************************************************************************
// Created: AGN 03-08-06
//*****************************************************************************
class MOS_Meteo_Manager
{
public:
    typedef std::vector< MOS_Meteo_Local* > T_Meteo_Vector;
    typedef T_Meteo_Vector::iterator        IT_Meteo_Vector;
    typedef T_Meteo_Vector::const_iterator  CIT_Meteo_Vector;

public:
    explicit MOS_Meteo_Manager();
    virtual ~MOS_Meteo_Manager();

    MOS_Meteo_ABC& GetGlobalWeather();

    QWidget* CreateWeatherGUI( QWidget* pParent );

    MOS_Meteo_Local& CreateLocalWeather();
    void DeleteLocalWeather( MOS_Meteo_Local& weather );

private:
    MOS_Meteo_ABC globalWeather_;
    T_Meteo_Vector localWeatherVector_;

    static MT_IdentifierManager idManager_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_Manager.inl"
#endif


#endif // __MOS_Meteo_Manager_h_