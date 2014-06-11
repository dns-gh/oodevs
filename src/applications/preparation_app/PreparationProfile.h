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

// =============================================================================
/** @class  PreparationProfile
    @brief  Preparation profile
*/
// Created: AGE 2006-10-16
// =============================================================================
class PreparationProfile : public kernel::Profile_ABC
{
public:
    //! @name Statics
    //@{
    static const Profile_ABC& GetProfile();
    //@}

public:
    //! @name Operations
    //@{
    virtual const QString& GetLogin() const;
    virtual bool IsVisible   ( const kernel::Entity_ABC& entity ) const;
    virtual bool IsKnowledgeVisible  ( const kernel::Knowledge_ABC& entity ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic  ( const kernel::Entity_ABC& entity ) const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;
    virtual bool IsPerceived( const kernel::Entity_ABC& ) const;
    //@}
};

#endif // __PreparationProfile_h_
