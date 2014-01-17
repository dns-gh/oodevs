// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FeatureNameParser_h_
#define __FeatureNameParser_h_

#include "LocationParser_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class TerrainFeatureSearcher;

// =============================================================================
/** @class  FeatureNameParser
    @brief  FeatureNameParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class FeatureNameParser : public LocationParser_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FeatureNameParser( kernel::Controllers& controllers );
    virtual ~FeatureNameParser();
    //@}

    //! @name LocationParser_ABC methods
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const;
    virtual QStringList Split( const QString& input ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< TerrainFeatureSearcher > searcher_;
    mutable QString lastRequest_;
    //@}
};

}

#endif // __FeatureNameParser_h_
