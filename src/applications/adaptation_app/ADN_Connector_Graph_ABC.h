// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Connector_Graph_ABC_h_
#define __ADN_Connector_Graph_ABC_h_

#include "ADN_Connector_Vector_ABC.h"

namespace gui
{
    class GQ_Plot;
}

class ADN_GraphValue;

// =============================================================================
/** @class  ADN_Connector_Graph_ABC
    @brief  ADN_Connector_Graph_ABC
    @par    Using example
    @code
    ADN_Connector_Graph_ABC;
    @endcode
*/
// Created: APE 2004-12-14
// =============================================================================
class ADN_Connector_Graph_ABC : public ADN_Connector_Vector_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Connector_Graph_ABC( gui::GQ_Plot& Graph );
    virtual ~ADN_Connector_Graph_ABC();
    //@}

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void SetDataPrivate( void* pData );
    virtual bool AddItemPrivate( void* pItem );
    virtual bool RemItemPrivate( void* pItem );
    virtual void ClearPrivate( bool bInConnection = false );

    virtual ADN_GraphValue* CreateValue( void* pObj ) = 0;

protected:
    gui::GQ_Plot& graph_;
};

#endif // __ADN_Connector_Graph_ABC_h_
