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

namespace tools
{
    class Path;
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
    explicit FeatureNameParser();
    virtual ~FeatureNameParser();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Path& graphicsDir );
    //@}

    //! @name LocationParser_ABC methods
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool small ) const;
    virtual QStringList Split( const QString& input ) const;
    virtual std::string GetStringPosition( const geometry::Point2f& position ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< TerrainFeatureSearcher > searcher_;
    mutable QString lastRequest_;
    //@}
};

}

#endif // __FeatureNameParser_h_
