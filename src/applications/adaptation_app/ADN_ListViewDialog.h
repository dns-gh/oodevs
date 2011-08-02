// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListViewDialog_h_
#define __ADN_ListViewDialog_h_

#include <boost/noncopyable.hpp>

class ADN_ListView;

// =============================================================================
/** @class  ADN_ListViewDialog
    @brief  ADN_ListViewDialog
*/
// Created: APE 2005-04-04
// =============================================================================
class ADN_ListViewDialog : public QDialog
                         , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListViewDialog( QWidget* pParent, const QString& strCaption, ADN_ListView* pListView );
    virtual ~ADN_ListViewDialog();
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
