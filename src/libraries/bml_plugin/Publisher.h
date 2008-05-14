// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_h_
#define __Publisher_h_

namespace xml
{
    class xistream;
}

namespace bml
{

// =============================================================================
/** @class  Publisher
    @brief  Publisher
*/
// Created: SBO 2008-04-02
// =============================================================================
class Publisher
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Publisher( xml::xistream& xis );
    virtual ~Publisher();
    //@}

    //! @name Operations
    //@{
    void BBSPush( const std::string& operation, const std::string& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Publisher( const Publisher& );            //!< Copy constructor
    Publisher& operator=( const Publisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __Publisher_h_
