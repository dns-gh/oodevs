// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Who_h_
#define __Who_h_

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent;
    class Automat;
}

namespace bml
{

// =============================================================================
/** @class  Who
    @brief  Who
*/
// Created: SBO 2008-05-22
// =============================================================================
class Who
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Who( const dispatcher::Agent& entity );
    explicit Who( const dispatcher::Automat& entity );
    virtual ~Who();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Who& who );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Who( const Who& );            //!< Copy constructor
    Who& operator=( const Who& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent* agent_;
    const dispatcher::Automat* automat_;
    //@}
};

}

#endif // __Who_h_
