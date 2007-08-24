// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Synchroniser_h_
#define __Synchroniser_h_

namespace dispatcher
{
    class Entity_ABC;
    class ClientPublisher_ABC;
    class Model;

// =============================================================================
/** @class  Synchroniser
    @brief  Synchroniser
*/
// Created: AGE 2007-04-25
// =============================================================================
class Synchroniser
{

public:
    //! @name Constructors/Destructor
    //@{
             Synchroniser();
    virtual ~Synchroniser();
    //@}

    //! @name Operations
    //@{
    void FlagForCreation     ( Entity_ABC& entity );
    void FlagForUpdate       ( Entity_ABC& entity );
    void FlagForSpecialUpdate( Entity_ABC& entity );
    void FlagForDestruction  ( Entity_ABC& entity );

    void Commit( ClientPublisher_ABC& publisher, Model& model );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Synchroniser( const Synchroniser& );            //!< Copy constructor
    Synchroniser& operator=( const Synchroniser& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Entity_ABC* >   T_Entities;
    typedef T_Entities::const_iterator CIT_Entities;
    //@}

private:
    //! @name Member data
    //@{
    T_Entities toCreate_;
    T_Entities toUpdate_;
    T_Entities toSpecial_;
    T_Entities toDestroy_;
    //@}
};

}

#endif // __Synchroniser_h_
