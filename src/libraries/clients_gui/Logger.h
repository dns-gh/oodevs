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

#include "RichTreeView.h"
#include "Types.h"
#include "clients_kernel/Logger_ABC.h"
#include <fstream>

namespace kernel
{
    class Time_ABC;
    class ContextMenu;
}

namespace gui
{

// =============================================================================
/** @class  Logger
    @brief  Captures log messages and displays them in a sorted list.
*/
// Created: APE 2004-06-02
// =============================================================================
class Logger : public RichTreeView
             , public kernel::Logger_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Logger( QWidget* pParent, const kernel::Time_ABC& simulation, const std::string& filename );
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    virtual LogElement Info();
    virtual LogElement Warning();
    virtual LogElement Error();
    //@}

public slots:
    //! @name Slots
    //@{
    void Clear();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void End( std::stringstream& output );
    QSize sizeHint() const { return QSize( 400, 250 ); }
    virtual void contextMenuEvent( QContextMenuEvent* event );
    virtual bool LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const;
    //@}

private:
    //! @name Helpers
    //@{
    LogElement StartLog( const QColor& color );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC& simulation_;
    kernel::ContextMenu* popupMenu_;
    std::ofstream log_;
    unsigned int counter_;
    //@}
};

}

#endif // __Logger_h_
