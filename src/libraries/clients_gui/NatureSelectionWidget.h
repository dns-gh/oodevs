// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __NatureSelectionWidget_h_
#define __NatureSelectionWidget_h_

#include <qlistview.h>
#include "clients_kernel/SymbolVisitor_ABC.h"

namespace gui
{

// =============================================================================
/** @class  NatureSelectionWidget
    @brief  NatureSelectionWidget
*/
// Created: SBO 2007-10-12
// =============================================================================
class NatureSelectionWidget : public QListView
                            , public kernel::SymbolVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NatureSelectionWidget( QWidget* parent, const std::string& symbolFile );
    virtual ~NatureSelectionWidget();
    //@}

signals:
    //! @name Signals
    //@{
    void NatureSelected( const QString& nature );
    void StartDrag();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NatureSelectionWidget( const NatureSelectionWidget& );            //!< Copy constructor
    NatureSelectionWidget& operator=( const NatureSelectionWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void StartCategory( const std::string& title );
    virtual void AddChoice( kernel::SymbolRule* rule, const std::string& name, const std::string& value );
    virtual void startDrag();
    //@}

private:
    //! @name Member data
    //@{
    QListViewItem* category_;
    QListViewItem* current_;
    //@}
};

}

#endif // __NatureSelectionWidget_h_
