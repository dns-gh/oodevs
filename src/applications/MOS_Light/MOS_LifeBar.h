//*****************************************************************************
//
// $Created: FBD 03-01-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LifeBar.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_LifeBar.h $
//
//*****************************************************************************

#ifndef __MOS_LifeBar_h_
#define __MOS_LifeBar_h_

#include "MOS_Types.h"

#include <QProgressBar.h>


class QWidget;

//*****************************************************************************
// Created: FBD 03-01-16
//*****************************************************************************
class MOS_LifeBar : public QProgressBar
{
    MT_COPYNOTALLOWED( MOS_LifeBar );

public:
    explicit MOS_LifeBar( QWidget* pParent );
    virtual ~MOS_LifeBar();

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    void SetValueMin( MT_Float rValue );
    void SetValue( MT_Float rValue );
    void SetValueMax( MT_Float rValue );
    //@}

private:

    MT_Float rValueMin_;
    MT_Float rValue_;
    MT_Float rValueMax_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_LifeBar.inl"
#endif


#endif // __MOS_LifeBar_h_