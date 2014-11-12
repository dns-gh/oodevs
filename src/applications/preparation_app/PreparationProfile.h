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

#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/Profile_ABC.h"

// =============================================================================
/** @class  PreparationProfile
    @brief  Preparation profile
*/
// Created: AGE 2006-10-16
// =============================================================================
class PreparationProfile : public kernel::Profile_ABC
                         , public kernel::Filter_ABC
{
public:
    //! @name Statics
    //@{
    static PreparationProfile& GetProfile();
    //@}

public:
    //! @name Profile_ABC implementation
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

    //! @name Filter_ABC implementation
    //@{
    virtual QString GetFilter() const;
    virtual void SetFilter( const kernel::Entity_ABC* entity, bool update = true );
    virtual void SetFilter( const kernel::Profile_ABC& profile );
    virtual const kernel::Entity_ABC* GetFilteredEntity() const;
    //@}
};

#endif // __PreparationProfile_h_
