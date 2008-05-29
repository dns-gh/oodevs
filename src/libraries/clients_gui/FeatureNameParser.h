// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FeatureNameParser_h_
#define __FeatureNameParser_h_

#include "LocationParser_ABC.h"

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
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FeatureNameParser( kernel::Controllers& controllers );
    virtual ~FeatureNameParser();
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( const QString& content, geometry::Point2f& result, QString& hint ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FeatureNameParser( const FeatureNameParser& );            //!< Copy constructor
    FeatureNameParser& operator=( const FeatureNameParser& ); //!< Assignment operator
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
