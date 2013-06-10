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

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/Logger_ABC.h"
#include "Types.h"
#include "tools/FileWrapper.h"

namespace kernel
{
    class Time_ABC;
    class ContextMenu;
    class Controllers;
}

namespace gui
{
    template< typename T > class RichWidget;
// =============================================================================
/** @class  Logger
    @brief  Captures log messages and displays them in a sorted list.
*/
// Created: APE 2004-06-02
// =============================================================================
class Logger : public gui::RichDockWidget
             , public kernel::Logger_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Logger( QWidget* pParent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, const tools::Path& filename );
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    virtual void Info( const std::string& message );
    virtual void Warning( const std::string& message );
    virtual void Error( const std::string& message );
    //@}

public slots:
    //! @name Slots
    //@{
    void Clear();
    //@}

protected:
    //! @name Operations
    //@{
    QSize sizeHint() const { return QSize( 400, 250 ); }
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    void WriteLog( const std::string& message, const QColor& color );
    //@}

private:
    //! @name Member data
    //@{
    RichWidget< QTreeWidget >* treeWidget_;
    const kernel::Time_ABC& simulation_;
    kernel::ContextMenu* popupMenu_;
    tools::Ofstream log_;
    unsigned int counter_;
    //@}
};

}

#endif // __Logger_h_
