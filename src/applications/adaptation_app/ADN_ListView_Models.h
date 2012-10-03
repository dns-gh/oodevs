//*****************************************************************************
//
// $Created: JDY 03-07-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Models.h $
// $Author: Ape $
// $Modtime: 1/03/05 10:25 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Models.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Models_h_
#define __ADN_ListView_Models_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"
#include "ADN_Models_Data.h"

enum E_EntityType;

//*****************************************************************************
// Created: JDY 03-07-25
//*****************************************************************************
class ADN_ListView_Models
: public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_ListView_Models( E_EntityType eEntityType, QWidget* pParent );
    virtual ~ADN_ListView_Models();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}

private:
    E_EntityType eEntityType_;
};

#endif // __ADN_ListView_Models_h_
