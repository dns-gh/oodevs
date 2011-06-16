// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logger_h_
#define __Logger_h_

#include "Types.h"
#include "clients_kernel/Logger_ABC.h"
#include <fstream>

class Simulation;

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  Logger
    @brief  Captures log messages and displays them in a sorted list.
*/
// Created: APE 2004-06-02
// =============================================================================
class Logger : public QListView, public kernel::Logger_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Logger( QWidget* pParent, ItemFactory_ABC& factory, const Simulation& simulation, const std::string& filename );
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    virtual LogElement Info();
    virtual LogElement Warning();
    virtual LogElement Error();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void End( std::stringstream& output );
    QSize sizeHint() const { return QSize( 400, 250 ); }
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    //@}

signals:
    //! @name Signals
    //@{
    void EmitError();
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< QListViewItem*, bool >      T_Item;
    typedef std::map< std::stringstream*, T_Item > T_Items;
    //@}

    //! @name Helpers
    //@{
    LogElement StartLog( const QColor& color, bool popup );
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    const Simulation& simulation_;
    QPopupMenu popupMenu_;
    T_Items items_;
    std::ofstream log_;
    //@}
};

}

#endif // __Logger_h_
