// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CampPrototype_ABC_h_
#define __CampPrototype_ABC_h_

#include "ObjectPrototypeAttributes_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  CampPrototype_ABC
    @brief  CampPrototype_ABC
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype_ABC : public ObjectPrototypeAttributes_ABC
                        , public kernel::Observer_ABC
                        , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype_ABC( QWidget* parent, kernel::Controllers& controllers );
    virtual ~CampPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Commit() = 0;
    virtual void Clean() = 0;
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    CampPrototype_ABC( const CampPrototype_ABC& );
    CampPrototype_ABC& operator=( const CampPrototype_ABC& );
    //@}

private slots:
    //! @name Slots
    //@{
    void SetSelected();
    void SelectionChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyContextMenu( const kernel::Agent_ABC&, kernel::ContextMenu& );
    //@}

protected:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    ValuedComboBox< const kernel::Agent_ABC* >* tc2s_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

}

#endif // __CampPrototype_ABC_h_
