//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Composantes.h $
// $Author: Ape $
// $Modtime: 25/04/05 11:14 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Composantes.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Composantes_h_
#define __ADN_ListView_Composantes_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_Composantes
    @brief  ADN_ListView_Composantes
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_ListView_Composantes
: public ADN_ListView
{
public:
    explicit ADN_ListView_Composantes( QWidget* pParent );
    virtual ~ADN_ListView_Composantes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ListView_Composantes_h_