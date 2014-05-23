// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationParser_ABC_h_
#define __LocationParser_ABC_h_

#include <geometry/Types.h>
#include <boost/noncopyable.hpp>
#include <QtCore/QStringlist>
#include <QtCore/QList>

class QStringList;

namespace gui
{
    struct LocationParserDescriptor
    {
        QStringList  labels;
        QList< int > sizes;
    };

// =============================================================================
/** @class  LocationParser_ABC
    @brief  Location parser interface
*/
// Created: AGE 2008-05-29
// =============================================================================
class LocationParser_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LocationParser_ABC() {}
    virtual ~LocationParser_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const = 0;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hints, bool small ) const = 0;
    virtual QStringList Split( const QString& input ) const = 0;
    virtual std::string GetStringPosition( const geometry::Point2f& position ) const = 0;
    //@}
};

}

#endif // __LocationParser_ABC_h_
