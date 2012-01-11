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
#include <boost/noncopyable.hpp>

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
                           , private boost::noncopyable
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
    static void Purge();
    virtual const std::string& GetNetwork() const;
    virtual bool IsActivated() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void SetNetwork( const std::string& network );
    const QColor* GetOverridenColor() const;
    void OverrideColor( const QColor& color ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    std::string network_;
    kernel::Controller& controller_;
    static std::map< std::string, QColor > undergroundColors_;
    //@}
};

#endif // __UndergroundAttribute_h_
