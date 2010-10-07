// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __EntryLabelType_h_
#define __EntryLabelType_h_

#include "OptionalValue.h"

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  EntryLabelType
    @brief  EntryLabelType
*/
// Created: JSR 2010-10-01
// =============================================================================
class EntryLabelType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EntryLabelType( xml::xistream& xis );
    virtual ~EntryLabelType();
    //@}

    //! @name Operations
    //@{
    bool Matches( const std::string& language, const std::string& kind ) const;
    const std::string& GetText() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntryLabelType( const EntryLabelType& );            //!< Copy constructor
    EntryLabelType& operator=( const EntryLabelType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string text_;
    OptionalValue< std::string > language_;
    OptionalValue< std::string > kind_;
    //@}
};

}

#endif // __EntryLabelType_h_
