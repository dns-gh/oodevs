// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef LogisticSelectionDialog_ABC_h
#define LogisticSelectionDialog_ABC_h

#include "clients_gui/RichWidget.h"

#include <QtGui/QDialog>

class LogisticsConsign_ABC;

// =============================================================================
/** @class  LogisticSelectionDialog_ABC
    @brief  LogisticSelectionDialog definition
*/
// Created: ABR 2014-01-23
// =============================================================================
class LogisticSelectionDialog_ABC : public gui::RichWidget< QDialog >
{
public:
    //! @name Constructors/Destructor
    //@{
    LogisticSelectionDialog_ABC( const QString& objectName, QWidget* parent )
        : gui::RichWidget< QDialog >( objectName, parent )
    {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void Show( const LogisticsConsign_ABC& consign ) = 0;
    //@}
};

#endif // LogisticSelectionDialog_ABC_h
