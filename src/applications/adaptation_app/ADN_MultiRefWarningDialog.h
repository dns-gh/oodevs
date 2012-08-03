// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MultiRefWarningDialog_h_
#define __ADN_MultiRefWarningDialog_h_

#include "ADN_Workspace.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_MultiRefWarningDialog
    @brief  ADN_MultiRefWarningDialog
*/
// Created: ABR 2012-07-24
// =============================================================================
class ADN_MultiRefWarningDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MultiRefWarningDialog( QWidget* parent, ADN_Workspace::T_UsingElements& elementsToDelete, ADN_Workspace::T_UsingElements& usingElements );
    virtual ~ADN_MultiRefWarningDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    //@}

private:
    //! @name Helpers
    //@{
    QStandardItem* CreateItem( const QString& text );
    void CreateTable( QVBoxLayout& layout, const QString& title, const ADN_Workspace::T_UsingElements& elements );
    void FillModel( QStandardItemModel& model, const ADN_Workspace::T_UsingElements& elements );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ADN_MultiRefWarningDialog_h_
