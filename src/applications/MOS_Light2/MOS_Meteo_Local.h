//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_Local.h $
// $Author: Ape $
// $Modtime: 20/07/04 17:41 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Local_h_
#define __MOS_Meteo_Local_h_

#include "MOS_Meteo_ABC.h"


// =============================================================================
/** @class  MOS_Meteo_Local
    @brief  Local weather data.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Meteo_Local : public MOS_Meteo_ABC
{
public:
     MOS_Meteo_Local( int nId );
    ~MOS_Meteo_Local();

    void SendUpdateMsg();
    void SendDeleteMsg() const;

    int GetId() const;

    MT_Vector2D& GetUpperLeftCorner();
    MT_Vector2D& GetLowerRightCorner();

    void SetCorners( const MT_Vector2D& corner1, const MT_Vector2D& corner2 );

private:
    int nId_;
    MT_Vector2D vUpperLeftCorner_;
    MT_Vector2D vLowerRightCorner_;
};


#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_Local.inl"
#endif

#endif // __MOS_Meteo_Local_h_