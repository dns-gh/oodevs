// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_SearchListView_h_
#define __ADN_SearchListView_h_

#include "ADN_NavigationInfos.h"

class ADN_Connector_ABC;
class ADN_GUI_ABC;
class ADN_ListView;
class ADN_Ref_ABC;

namespace gui
{
    class SearchLineEdit;
}

// =============================================================================
/** @class  ADN_SearchListView
    @brief  ADN_SearchListView
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_SearchListView : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_SearchListView( const QString& objectName, ADN_GUI_ABC* gui, ADN_ListView& listView, ADN_Ref_ABC& reference, const std::vector< ADN_Connector_ABC* >& connector, int subTab = -1, QWidget* parent = 0 );
    virtual ~ADN_SearchListView();
    //@}

    //! @name Operations
    //@{
    virtual ADN_ListView* GetListView() const;
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnApplyFilterList( const ADN_NavigationInfos::UsedBy& usedByInfos );
    //@}

private:
    //! @name Member data
    //@{
    ADN_ListView*        listView_;
    gui::SearchLineEdit* lineEdit_;
    int                  subTab_;
    //@}
};

#endif // __ADN_SearchListView_h_
