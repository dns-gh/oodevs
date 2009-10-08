// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainFeatureSearcher_h_
#define __TerrainFeatureSearcher_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"

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
class TerrainFeatureSearcher : public tools::Observer_ABC
                             , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainFeatureSearcher( kernel::Controllers& controllers );
    virtual ~TerrainFeatureSearcher();
    //@}

    //! @name Operations
    //@{
    bool Search  ( const QString& name, geometry::Point2f& point, QString& hint );
    bool FindNext( geometry::Point2f& point, QString& hint );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainFeatureSearcher( const TerrainFeatureSearcher& );            //!< Copy constructor
    TerrainFeatureSearcher& operator=( const TerrainFeatureSearcher& ); //!< Assignment operator
    //@}

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
    kernel::Controllers& controllers_;
    std::auto_ptr< T_NameLocations > nameLocations_;
    const T_Feature* current_;
    unsigned index_;
    std::string pendingSourceFile_;
    //@}
};

}

#endif // __TerrainFeatureSearcher_h_
