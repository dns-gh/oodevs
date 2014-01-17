// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XyParser_h_
#define __XyParser_h_

#include "LocationParser_ABC.h"
#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  XyParser
    @brief  XyParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class XyParser : public LocationParser_ABC
               , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             XyParser();
    virtual ~XyParser();
    //@}

    //! @name LocationParser_ABC methods
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const;
    virtual QStringList Split( const QString& input ) const;
    //@}

    //! @name Operations
    //@{
    bool Parse( const QString& content, geometry::Point2f& result, QString& hint ) const;
    //@}
};

}

#endif // __XyParser_h_
