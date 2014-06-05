// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Languages_Dialog_h_
#define __ADN_Languages_Dialog_h_

#include "clients_gui/RichWidget.h"

class ADN_Languages_Data;
class ADN_Languages_MasterDialog;

// =============================================================================
/** @class  ADN_Languages_Dialog
    @brief  ADN_Languages_Dialog
*/
// Created: ABR 2013-07-08
// =============================================================================
class ADN_Languages_Dialog : public gui::RichWidget< QDialog >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Languages_Dialog( ADN_Languages_Data& data );
    virtual ~ADN_Languages_Dialog();
    //@}

signals:
    //! @name Signals
    //@{
    void LanguagesEdited();
    void MasterChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual int exec();
    virtual void accept();
    virtual void OnSelectionChanged();

    virtual void OnUp();
    virtual void OnDown();
    virtual void OnAdd();
    virtual void OnRemove();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Languages_Data& data_;
    std::unique_ptr< ADN_Languages_MasterDialog > masterDialog_;
    QLabel* master_;
    QTreeWidget* availables_;
    QTreeWidget* actives_;
    QToolButton* add_;
    QToolButton* remove_;
    QToolButton* up_;
    QToolButton* down_;
    //@}
};

#endif // __ADN_Languages_Dialog_h_
