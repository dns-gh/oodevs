//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Local.h $
// $Author: Ape $
// $Modtime: 20/07/04 17:41 $
// $Revision: 2 $
// $Workfile: Meteo_Local.h $
//
//*****************************************************************************

#ifndef __Meteo_Local_h_
#define __Meteo_Local_h_

#include "Meteo_ABC.h"


// =============================================================================
/** @class  Meteo_Local
    @brief  Local weather data.
*/
// Created: APE 2004-07-19
// =============================================================================
class Meteo_Local : public Meteo_ABC
{
public:
     Meteo_Local( int nId );
    ~Meteo_Local();

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

#	include "Meteo_Local.inl"

#endif // __Meteo_Local_h_