//*****************************************************************************
//
// $Created: JDY 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Combo_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 15:47 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Combo_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Combo_ABC_h_
#define __ADN_Connector_Combo_ABC_h_

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
class ADN_Connector_Combo_ABC
{
public:
    explicit ADN_Connector_Combo_ABC() {}
    virtual ~ADN_Connector_Combo_ABC() {}
    virtual void* SetNdxChanged( int ndx ) = 0;
    virtual bool IsConnected() const = 0;

protected:
    virtual void ClearPrivate( bool bInConnection = false ) = 0;
    virtual void SetDataPrivate( void* data ) = 0;
};

#endif // __ADN_Connector_Combo_ABC_h_
