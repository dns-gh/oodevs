// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FireClass_h_
#define __FireClass_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  FireClass
    @brief  FireClass
*/
// Created: RFT 2006-04-04
// =============================================================================
class FireClass
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FireClass( xml::xistream& input );
    virtual ~FireClass();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    //@}
};

}

#endif // __FireClass_h_
