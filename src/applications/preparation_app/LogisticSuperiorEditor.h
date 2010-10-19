// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSuperiorEditor_h_
#define __LogisticSuperiorEditor_h_

#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/ValueEditor.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "preparation/LogisticSuperior.h"
#include <boost/type_traits.hpp>

template <typename Superior>
struct LogisticSuperiorTraits
{
	typedef typename Superior::BaseType BaseType;
	typedef typename boost::remove_pointer<BaseType>::type BaseTypeNP;
	typedef typename boost::remove_const<BaseTypeNP>::type BaseTypeNCNP;
};

// =============================================================================
/** @class  LogisticSuperiorEditor
    @brief  LogisticSuperiorEditor
*/
// Created: SBO 2006-10-25
// =============================================================================
template< typename Superior >
class LogisticSuperiorEditor : public gui::ValuedComboBox< typename LogisticSuperiorTraits<Superior>::BaseType >
                             , public kernel::ValueEditor< Superior >
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< typename LogisticSuperiorTraits<Superior>::BaseTypeNCNP >
{
public:
	typedef typename LogisticSuperiorTraits<Superior>::BaseTypeNCNP SuperiorEntityType;
    //! @name Constructors/Destructor
    //@{
             LogisticSuperiorEditor( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< SuperiorEntityType >& resolver, const kernel::Entity_ABC& selected );
    virtual ~LogisticSuperiorEditor();
    //@}

    //! @name Operations
    //@{
    virtual Superior GetValue();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticSuperiorEditor( const LogisticSuperiorEditor& );            //!< Copy constructor
    LogisticSuperiorEditor& operator=( const LogisticSuperiorEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const SuperiorEntityType& automat );
    virtual void NotifyDeleted( const SuperiorEntityType& automat );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    LogisticSuperior filter_;
    //@}
};

#include "LogisticSuperiorEditor.inl"

#endif // __LogisticSuperiorEditor_h_
