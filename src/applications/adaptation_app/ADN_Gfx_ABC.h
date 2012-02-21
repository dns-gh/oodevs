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

class ADN_Connector_ABC;

//*****************************************************************************
// Created: JDY 03-07-10
//*****************************************************************************
class ADN_Gfx_ABC
{
public:
    explicit ADN_Gfx_ABC(bool bAuto=true);
    virtual ~ADN_Gfx_ABC();

    ADN_Connector_ABC&     GetConnector();
    void                   SetAutoEnabled(bool bAuto);
    bool                   IsAutoEnabled();

    void SetIsEnabledOnlyInAdminMode( bool b );

protected:

    ADN_Connector_ABC* pConnector_;
    bool               bAutoEnabled_;
    bool               bEnabledOnlyInAdminMode_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::GetConnector
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
inline
ADN_Connector_ABC& ADN_Gfx_ABC::GetConnector()
{
    return *pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::SetAutoEnabled
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
inline
void ADN_Gfx_ABC::SetAutoEnabled(bool bAuto)
{
    bAutoEnabled_=bAuto;
}

//-----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::IsAutoEnabled
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
inline
bool ADN_Gfx_ABC::IsAutoEnabled()
{
    return bAutoEnabled_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Gfx_ABC::SetIsEnabledOnlyInAdminMode
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
inline
void ADN_Gfx_ABC::SetIsEnabledOnlyInAdminMode( bool b )
{
    bEnabledOnlyInAdminMode_ = b;
}

#endif // __ADN_Gfx_ABC_h_