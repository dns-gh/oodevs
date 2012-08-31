// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __EntityTreeView_h_
#define __EntityTreeView_h_

#include "clients_gui/EntityTreeView.h"

// =============================================================================
/** @class  EntityTreeView
    @brief  EntityTreeView
*/
// Created: ABR 2012-08-14
// =============================================================================
template< typename Entity >
class EntityTreeView : public gui::EntityTreeView< Entity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityTreeView( QWidget* parent = 0 );
    virtual ~EntityTreeView();
    //@}

    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void keyPressEvent( QKeyEvent* e );
    //@}

    // $$$$ ABR 2012-08-14: TODO D&D

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#include "EntityTreeView.inl"

typedef EntityTreeView< kernel::Inhabitant_ABC > InhabitantTreeView;
typedef EntityTreeView< kernel::Object_ABC >     ObjectTreeView;
typedef EntityTreeView< kernel::Population_ABC > PopulationTreeView;
typedef EntityTreeView< kernel::UrbanObject_ABC> UrbanTreeView;

#endif // __EntityTreeView_h_
