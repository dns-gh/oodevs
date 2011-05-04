// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LivingAreaDialog_h_
#define __LivingAreaDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/LivingAreaVisitor_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include <boost/optional.hpp>
#include <map>

namespace kernel
{
    class Controllers;
    class Inhabitant_ABC;
    class LivingArea_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

class RcEntityResolver_ABC;

namespace
{
    class LivingAreaTable;
}

// =============================================================================
/** @class  LivingAreaDialog
    @brief  Living area dialog
*/
// Created: LGY 2011-04-29
// =============================================================================
class LivingAreaDialog : public QDialog
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                       , public tools::SelectionObserver< kernel::Inhabitant_ABC >
                       , public tools::ElementObserver_ABC< kernel::LivingArea_ABC >
                       , private kernel::LivingAreaVisitor_ABC

{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LivingAreaDialog( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const RcEntityResolver_ABC& rcResolver );
    virtual ~LivingAreaDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifySelected( const kernel::Inhabitant_ABC* entity );
    virtual void NotifyUpdated( const kernel::LivingArea_ABC& area );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Visit( unsigned int id, unsigned int resident, bool alerted, float angriness, bool confined, bool evacuated );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    //@}

private:
    //! @name Helpers
    //@{
    void Update();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, int > T_Rows;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    const RcEntityResolver_ABC& rcResolver_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    LivingAreaTable* table_;
    T_Rows rows_;
    boost::optional< unsigned long > current_;
    //@}
};

#endif // __LivingAreaDialog_h_
