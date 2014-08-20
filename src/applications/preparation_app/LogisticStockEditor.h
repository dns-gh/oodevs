// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _LogisticStockEditor_h
#define _LogisticStockEditor_h

#include "LogisticEditor.h"

// =============================================================================
/** @class  LogisticStockEditor
@brief  Color editor
*/
// Created: MMC 2011-06-23
// =============================================================================
class LogisticStockEditor : public LogisticEditor
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticStockEditor( QWidget* parent, kernel::Controllers& controllers, const ::StaticModel& staticModel );
    virtual ~LogisticStockEditor();
    //@}

signals:
    //! @name Signals
    //@{
    void DotationsStocksComputed( const std::map< const kernel::DotationType*, unsigned int >&, const kernel::Entity_ABC& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SupplyHierarchy( const gui::LogisticHierarchiesBase& logHierarchy );
    //@}
};

#endif // _LogisticStockEditor_h
