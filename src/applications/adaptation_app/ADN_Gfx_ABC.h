//*****************************************************************************
//
// $Created: JDY 03-07-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Gfx_ABC.h $
// $Author: Ape $
// $Modtime: 27/01/05 17:05 $
// $Revision: 2 $
// $Workfile: ADN_Gfx_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Gfx_ABC_h_
#define __ADN_Gfx_ABC_h_

#include "ADN_Checker.h"

class ADN_Connector_ABC;

//*****************************************************************************
// Created: JDY 03-07-10
//*****************************************************************************
class ADN_Gfx_ABC
{
public:
    explicit ADN_Gfx_ABC( bool bAuto = true );
    virtual ~ADN_Gfx_ABC();

    ADN_Connector_ABC& GetConnector();
    void SetAutoEnabled( bool bAuto );
    bool IsAutoEnabled();

protected:
    std::unique_ptr< ADN_Connector_ABC > pConnector_;
    bool bAutoEnabled_;
};

#endif // __ADN_Gfx_ABC_h_
