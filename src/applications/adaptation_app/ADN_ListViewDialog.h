// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListViewDialog.h $
// $Author: Ape $
// $Modtime: 20/04/05 15:56 $
// $Revision: 2 $
// $Workfile: ADN_ListViewDialog.h $
//
// *****************************************************************************

#ifndef __ADN_ListViewDialog_h_
#define __ADN_ListViewDialog_h_

#include <qdialog.h>

class ADN_ListView;


// =============================================================================
/** @class  ADN_ListViewDialog
    @brief  ADN_ListViewDialog
*/
// Created: APE 2005-04-04
// =============================================================================
class ADN_ListViewDialog
: public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_ListViewDialog )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_ListViewDialog( QWidget* pParent, const QString& strCaption, ADN_ListView* pListView );
    ~ADN_ListViewDialog();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnContextMenu();
    void PrintListView();
    //@}

private:
    //! @name Member data
    //@{
    ADN_ListView* pListView_;
    //@}
};

#endif // __ADN_ListViewDialog_h_
