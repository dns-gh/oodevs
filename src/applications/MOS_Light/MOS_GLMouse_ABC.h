//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GLMouse_ABC.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_GLMouse_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_GLMouse_ABC_h_
#define __MOS_GLMouse_ABC_h_

#include "MOS_Types.h"


//*****************************************************************************
// Created: FBD 03-01-10
//*****************************************************************************
class MOS_GLMouse_ABC
{
public:
    explicit MOS_GLMouse_ABC();
    virtual ~MOS_GLMouse_ABC();

public:

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Draw                   () = 0;
    virtual bool IsActive               ( bool bMove ) = 0;
    virtual bool RMBPressEvent          ( const MT_Vector2D& vPos ) = 0;
    virtual bool LMBPressEvent          ( const MT_Vector2D& vPos ) = 0;
    virtual void MouseMoveEvent         ( const MT_Vector2D& vPos ) = 0;
    virtual void RMBReleaseEvent        ( const MT_Vector2D& vPos ) = 0;
    virtual void LMBReleaseEvent        ( const MT_Vector2D& vPos ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

private:

};

#ifdef MOS_USE_INLINE
#	include "MOS_GLMouse_ABC.inl"
#endif


#endif // __MOS_GLMouse_ABC_h_