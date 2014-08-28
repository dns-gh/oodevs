// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DrawingsBuilder_h_
#define __DrawingsBuilder_h_

#include "clients_gui/ModelObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

// =============================================================================
/** @class  DrawingsBuilder
    @brief  DrawingsBuilder
*/
// Created: LGY 2014-05-14
// =============================================================================
class DrawingsBuilder : public QObject
                      , public gui::ModelObserver_ABC
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawingsBuilder( kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~DrawingsBuilder();
    //@}

public:
    //! @name Operations
    //@{
    virtual void CreateCommunication();
    virtual void CreateTeam();
    virtual void ClearSelection();
    virtual void CreateFormation( int level );
    virtual kernel::Automat_ABC* ReplaceAutomat( kernel::Entity_ABC& original, const kernel::AutomatType& type );
    virtual void DeleteEntity( const kernel::Entity_ABC& entity );
    virtual void CreateCityOrDistrict( kernel::Entity_ABC* parent );
    virtual void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects );

    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnDelete();
    void OnConfirmDeletion( int result );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > toDelete_;
    const kernel::Profile_ABC& profile_;
    std::unique_ptr< QMessageBox > confirmation_;
    //@}
};

#endif // __DrawingsBuilder_h_
