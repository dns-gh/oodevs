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

#include <boost/noncopyable.hpp>
#include "clients_kernel/ValueEditor.h"

// =============================================================================
/** @class  MultipleResolverEditor
    @brief  MultipleResolverEditor
*/
// Created: ABR 2011-11-29
// =============================================================================
template< typename Entity, typename Resolver >
class MultipleResolverEditor : public QDialog
                             , public kernel::ValueEditor< std::vector< Entity* > >
                             , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             MultipleResolverEditor( QDialog*& self, QWidget* parent, const Resolver& resolver );
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
    std::vector< Entity* >*      values_;
    QDialog*&                    self_;
    Q3ListBox*                   listBox_;
    //@}
};


#include "MultipleResolverEditor.inl"

#endif // __MultipleResolverEditor_h_
