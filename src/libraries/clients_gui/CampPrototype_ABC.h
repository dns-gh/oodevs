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

class Agent_ABC;
class Controllers;

// =============================================================================
/** @class  CampPrototype_ABC
    @brief  CampPrototype_ABC
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype_ABC : public ObjectPrototypeAttributes_ABC
                        , public Observer_ABC
                        , public ElementObserver_ABC< Agent_ABC >
                        , public ContextMenuObserver_ABC< Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype_ABC( QWidget* parent, Controllers& controllers );
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
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent_ABC& agent );
    virtual void NotifyDeleted( const Agent_ABC& agent );
    virtual void NotifyContextMenu( const Agent_ABC&, ContextMenu& );
    //@}

protected:
    //! @name Member Data
    //@{
    Controllers& controllers_;
    ValuedComboBox< const Agent_ABC* >* tc2s_;
    SafePointer< Agent_ABC > selected_;
    //@}
};

#endif // __CampPrototype_ABC_h_
