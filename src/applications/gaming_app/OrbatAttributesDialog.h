// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __OrbatAttributesDialog_h_
#define __OrbatAttributesDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Population_ABC;
}

// =============================================================================
/** @class  OrbatAttributesDialog
    @brief  OrbatAttributesDialog
*/
// Created: JSR 2010-10-11
// =============================================================================
class OrbatAttributesDialog : public QDialog
                            , public tools::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             OrbatAttributesDialog( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~OrbatAttributesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrbatAttributesDialog( const OrbatAttributesDialog& );            //!< Copy constructor
    OrbatAttributesDialog& operator=( const OrbatAttributesDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateSubMenu( kernel::ContextMenu& menu, const std::map< std::string, std::string >& extensions );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const std::map< std::string, std::string >* extensions_;
    std::vector< QLabel* > labels_;
    QGridLayout *layout_;
    //@}
};

#endif // __OrbatAttributesDialog_h_
