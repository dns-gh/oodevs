// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorElement_ABC_h_
#define __IndicatorElement_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
}

class IndicatorPrimitiveParameter;
class IndicatorType;

// =============================================================================
/** @class  IndicatorElement_ABC
    @brief  IndicatorElement_ABC
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorElement_ABC( const std::string& id ) : id_( id ) {}
    virtual ~IndicatorElement_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetInput() const { return id_; }
    virtual std::string GetValue() const = 0;
    virtual const IndicatorType& GetType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< IndicatorElement_ABC > element ) = 0;
    virtual void Serialize( xml::xostream& xos ) const = 0;
    virtual void SerializeDeclaration( xml::xostream& ) const {}
    virtual IndicatorElement_ABC& Clone() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorElement_ABC( const IndicatorElement_ABC& );            //!< Copy constructor
    IndicatorElement_ABC& operator=( const IndicatorElement_ABC& ); //!< Assignment operator
    //@}

    //! @name Member data
    //@{
    const std::string id_;
    //@}
};

#endif // __IndicatorElement_ABC_h_
