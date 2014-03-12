// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainFeatureSearcher_h_
#define __TerrainFeatureSearcher_h_

#include "clients_kernel/ModelLoaded.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    template< typename T > class ApproximativeMap;
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  TerrainFeatureSearcher
    @brief  TerrainFeatureSearcher
*/
// Created: AGE 2008-05-29
// =============================================================================
class TerrainFeatureSearcher : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainFeatureSearcher();
    virtual ~TerrainFeatureSearcher();
    //@}

    //! @name Operations
    //@{
    bool Search( const QString& name, geometry::Point2f& point, QString& hint );
    bool FindNext( geometry::Point2f& point, QString& hint );
    void Load( const tools::Path& graphicsDir );
    //@}

private:
    //! @name Helpers
    //@{
    void LoadFeatures();
    //@}

    //! @name Types
    //@{
    typedef std::pair< QString, T_PointVector >   T_Feature;
    typedef kernel::ApproximativeMap< T_Feature > T_NameLocations;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< T_NameLocations > nameLocations_;
    const T_Feature* current_;
    unsigned index_;
    tools::Path pendingSourceFile_;
    //@}
};

}

#endif // __TerrainFeatureSearcher_h_
