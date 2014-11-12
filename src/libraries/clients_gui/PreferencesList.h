// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencesList_h_
#define __PreferencesList_h_

#include "RichTreeView.h"

namespace gui
{
    class KeyModel;
    class PreferencePanel_ABC;

// =============================================================================
/** @class  PreferencesList
    @brief  PreferencesList
*/
// Created: SBO 2007-01-03
// =============================================================================
class PreferencesList : public RichTreeView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesList( const QString& objectName,
                              QStackedWidget& pages,
                              QWidget* parent = 0 );
    virtual ~PreferencesList();
    //@}

    //! @name Operations
    //@{
    void AddPage( const QString& name, PreferencePanel_ABC* widget );
    PreferencePanel_ABC* GetCurrentWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelect( const QItemSelection& selected, const QItemSelection& deselected );
    //@}

private:
    //! @name Helpers
    //@{
    QString GetFullName( QStandardItem* item );
    //@}

private:
    //! @name Member data
    //@{
    QStackedWidget& pages_;
    KeyModel* model_;
    std::map< QString, PreferencePanel_ABC* > widgets_;
    //@}
};

} //! namespace gui

#endif // __PreferencesList_h_
