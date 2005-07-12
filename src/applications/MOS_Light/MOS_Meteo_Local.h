//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Local.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Local_h_
#define __MOS_Meteo_Local_h_

#include "MOS_Meteo_ABC.h"

//*****************************************************************************
// Created: AGN 03-08-06
//*****************************************************************************
class MOS_Meteo_Local
: public MOS_Meteo_ABC
{
public:
    MOS_Meteo_Local( int nId );
    virtual ~MOS_Meteo_Local();

    virtual void SendUpdateMsg();
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