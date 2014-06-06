// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_ABC_h_
#define __Profile_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Knowledge_ABC;

// =============================================================================
/** @class  Profile_ABC
    @brief  Profile definition
*/
// Created: AGE 2006-10-11
// =============================================================================
class Profile_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile_ABC() {}
    virtual ~Profile_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const QString& GetLogin() const = 0;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const = 0;
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const = 0;
    virtual bool CanBeOrdered ( const kernel::Entity_ABC& entity ) const = 0;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const = 0;
    virtual bool IsSupervision() const = 0;
    virtual bool HasTimeControl() const = 0;
    /** Returns true is either the entity or a knowledge of this entity is visible. */
    virtual bool IsPerceived( const kernel::Entity_ABC& ) const = 0;
    virtual bool IsKnowledgeVisibleNoSupervision( const kernel::Knowledge_ABC& knowledge ) const { return IsKnowledgeVisible( knowledge ); }
    //@}
};

}

#endif // __Profile_ABC_h_
