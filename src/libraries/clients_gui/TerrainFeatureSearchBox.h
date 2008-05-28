// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainFeatureSearchBox_h_
#define __TerrainFeatureSearchBox_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"

namespace kernel
{
    template< typename T > class ApproximativeMap;
    class Controllers;
}

namespace gui
{
    class View_ABC;

// =============================================================================
/** @class  TerrainFeatureSearchBox
    @brief  TerrainFeatureSearchBox
*/
// Created: AGE 2008-05-28
// =============================================================================
class TerrainFeatureSearchBox : public QHBox
                              , public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TerrainFeatureSearchBox( QWidget* parent, kernel::Controllers& controllers, View_ABC& view );
    virtual ~TerrainFeatureSearchBox();
    //@}

private slots:
    //! @name Slots
    //@{
    void Search( const QString& );
    void FindNext();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainFeatureSearchBox( const TerrainFeatureSearchBox& );            //!< Copy constructor
    TerrainFeatureSearchBox& operator=( const TerrainFeatureSearchBox& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef kernel::ApproximativeMap< T_PointVector > T_NameLocations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    View_ABC& view_;
    std::auto_ptr< T_NameLocations > nameLocations_;
    const T_PointVector* current_;
    unsigned index_;
    //@}
};

}

#endif // __TerrainFeatureSearchBox_h_
