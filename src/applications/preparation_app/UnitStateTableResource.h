// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableResource_h_
#define __UnitStateTableResource_h_

#include "clients_gui/UnitStateTableResource.h"

namespace kernel
{
    class Entity_ABC;
}

class StaticModel;

// =============================================================================
/** @class  UnitStateTableResource
    @brief  UnitStateTableResource
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableResource : public gui::UnitStateTableResource
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableResource( QWidget* parent, const StaticModel& staticModel );
    virtual ~UnitStateTableResource();
    //@}

    //! @name Operations
    //@{
    virtual void Load( kernel::Entity_ABC& selected );
    virtual void Commit( kernel::Entity_ABC& selected ) const;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void keyPressEvent( QKeyEvent * e );
    void AddItem( int id );
    bool IsDotationAlreadyPresent( const QString& name ) const;
    //@}

private slots:
    //! @name Member data
    //@{
    void OnRemoveCurrentItem();
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    unsigned int typeId_;
    mutable std::vector< int > rowChanged_;
    //@}
};

#endif // __UnitStateTableResource_h_
