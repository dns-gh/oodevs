// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MultipleResolverEditor_h_
#define __MultipleResolverEditor_h_

#include "clients_gui/PropertyDialog.h"
#include "clients_kernel/ValueEditor.h"

// =============================================================================
/** @class  MultipleResolverEditor
    @brief  Multiple resolver editor
*/
// Created: ABR 2011-11-29
// =============================================================================
template< typename Entity, typename Resolver >
class MultipleResolverEditor : public gui::PropertyDialog
                             , public kernel::ValueEditor< std::vector< Entity* > >
{

public:
    //! @name Constructors/Destructor
    //@{
             MultipleResolverEditor( QWidget* parent, const Resolver& resolver );
    virtual ~MultipleResolverEditor();
    //@}

    //! @name Operations
    //@{
    void SetCurrentItem( std::vector< Entity* >& entity );
    virtual std::vector< Entity* > GetValue();
    //@}

private:
    //! @name Slots
    //@{
    virtual void accept();
    //@}

protected:
    //! @name Member data
    //@{
    std::vector< const Entity* > entities_;
    std::vector< Entity* >* values_;
    QListWidget* listBox_;
    //@}
};

#include "MultipleResolverEditor.inl"

#endif // __MultipleResolverEditor_h_
