// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __IdManager_h_
#define __IdManager_h_

// =============================================================================
/** @class  IdManager
    @brief  IdManager
*/
// Created: SBO 2006-09-26
// =============================================================================
class IdManager
{
public:
    //! @name Constructors/Destructor
    //@{
             IdManager();
    virtual ~IdManager();
    //@}

    //! @name Operations
    //@{
    unsigned long GetNextId();
    void Reset();
    void Lock( unsigned long id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IdManager( const IdManager& );            //!< Copy constructor
    IdManager& operator=( const IdManager& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long max_;
    //@}
};

#endif // __IdManager_h_
