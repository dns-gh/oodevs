// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreparationProfile_h_
#define __PreparationProfile_h_

#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
}

// =============================================================================
/** @class  PreparationProfile
    @brief  Preparation profile
*/
// Created: AGE 2006-10-16
// =============================================================================
class PreparationProfile : public kernel::Profile_ABC, public QObject
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             PreparationProfile( QObject* parent, kernel::Controllers& controllers );
    virtual ~PreparationProfile();
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible   ( const kernel::Entity_ABC& entity ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic  ( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

    //! @name Copy / Assignment
    //@{
    PreparationProfile( const PreparationProfile& );
    PreparationProfile& operator=( const PreparationProfile& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __PreparationProfile_h_
