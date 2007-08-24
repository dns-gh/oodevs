// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Synchronisable_h_
#define __Synchronisable_h_

namespace dispatcher
{
    class ClientPublisher_ABC;
    class ModelVisitor_ABC;
    class Synchroniser;
    class Model;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: AGE 2007-04-12
// =============================================================================
class Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Operations
    //@{
    void StartSynchronisation( bool create );
    void EndSynchronisation  ( Synchroniser& synch );

    virtual void SendFullUpdate   ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendCreation     ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendDestruction  ( ClientPublisher_ABC& publisher ) const;
    virtual void SendSpecialUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void Accept           ( ModelVisitor_ABC& visitor );
    //@}

protected:
    //! @name Operations
    //@{
    bool FlagUpdate( bool special = false );
    void StartSynchronisation( Entity_ABC& next, bool create );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    bool synching_ : 1;
    bool updated_  : 1;
    bool created_  : 1;
    bool special_  : 1;
    //@}
};

}

#endif // __Synchronisable_h_
