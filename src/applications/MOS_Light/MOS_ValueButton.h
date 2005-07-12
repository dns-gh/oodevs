//*****************************************************************************
//
// $Created: FBD 03-01-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueButton.h $
// $Author: Fbd $
// $Modtime: 27/08/03 14:34 $
// $Revision: 6 $
// $Workfile: MOS_ValueButton.h $
//
//*****************************************************************************

#ifndef __MOS_ValueButton_h_
#define __MOS_ValueButton_h_

#include "MOS_Types.h"

#include <QRadioButton.h>

//*****************************************************************************
// Created: FBD 03-01-24
//*****************************************************************************

template <class T>
class MOS_ValueButton : public QRadioButton
{
    MT_COPYNOTALLOWED( MOS_ValueButton );

    
public:
    MOS_ValueButton( T value, QWidget* pWidget, const std::string& sName );
    ~MOS_ValueButton();

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( T& output, std::string& sParam );
    //@}
    
private:
    std::string sName_;
    T value_;
};

#	include "MOS_ValueButton.inl"


#endif // __MOS_ValueButton_h_