// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Side_h_
#define __Side_h_

namespace xml
{
    class xistream  ; 
}

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: RDS 2008-09-10
// =============================================================================
class Side
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Side( xml::xistream& xis );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    unsigned int        GetId() const; 
    const std::string   GetName() const ; 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_ ; 
    std::string name_ ; 
    //@}
};

#endif // __Side_h_
