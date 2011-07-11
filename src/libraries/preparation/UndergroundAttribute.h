// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundAttribute_h_
#define __UndergroundAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UndergroundAttribute
    @brief  UndergroundAttribute
*/
// Created: JSR 2011-07-07
// =============================================================================
class UndergroundAttribute : public kernel::UndergroundAttribute_ABC
                           , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UndergroundAttribute( kernel::PropertiesDictionary& dico, kernel::Controller& controller );
             UndergroundAttribute( xml::xistream& xis, kernel::Controller& controller, kernel::PropertiesDictionary& dico );
    virtual ~UndergroundAttribute();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetNetwork() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void SetNetwork( const std::string& network );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UndergroundAttribute( const UndergroundAttribute& );            //!< Copy constructor
    UndergroundAttribute& operator=( const UndergroundAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    std::string network_;
    kernel::Controller& controller_;
    //@}
};

#endif // __UndergroundAttribute_h_
