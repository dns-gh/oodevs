// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __IdManager_h_
#define __IdManager_h_

namespace messenger
{

// =============================================================================
/** @class  IdManager
    @brief  IdManager
*/
// Created: RDS 2008-04-04
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
    unsigned int nextId(); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IdManager( const IdManager& );            //!< Copy constructor
    IdManager& operator=( const IdManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nextId_ ;   
    //@}
};

}   // namespace messenger

#endif // __IdManager_h_
