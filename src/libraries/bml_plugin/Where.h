// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Where_h_
#define __Where_h_

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  Where
    @brief  Where
*/
// Created: SBO 2008-05-22
// =============================================================================
class Where
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Where( const dispatcher::Agent& agent );
    virtual ~Where();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Where& data );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Where( const Where& );            //!< Copy constructor
    Where& operator=( const Where& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent& agent_;
    //@}
};

}
}

#endif // __Where_h_
