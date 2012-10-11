// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeType_h_
#define __GaugeType_h_

#include <boost/noncopyable.hpp>

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
    class Displayer_ABC;
}

namespace xml
{
    class xistream;
}

namespace indicators
{
// =============================================================================
/** @class  GaugeType
    @brief  GaugeType
*/
// Created: SBO 2009-05-05
// =============================================================================
class GaugeType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GaugeType( xml::xistream& xis );
    virtual ~GaugeType();
    //@}

    //! @name Accessors
    //@{
    const QString& GetName() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer, double value ) const;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    //@}
};

}

#endif // __GaugeType_h_
