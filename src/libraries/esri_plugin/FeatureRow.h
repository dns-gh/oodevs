// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FeatureRow_h_
#define __FeatureRow_h_

#include "Row.h"

namespace plugins
{
namespace esri
{

// =============================================================================
/** @class  FeatureRow
    @brief  FeatureRow
*/
// Created: SBO 2007-08-30
// =============================================================================
class FeatureRow : public esri::Row
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FeatureRow( ISpatialReferencePtr spatialReference );
    virtual ~FeatureRow();
    //@}

    //! @name Operations
    //@{
    virtual void SetShape( const Shape_ABC& value );
    virtual Shape_ABC& GetShape() const;
    void BindFeature( IFeaturePtr feature );
    void Commit( IFeatureCursorPtr cursor = NULL );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FeatureRow( const FeatureRow& );            //!< Copy constructor
    FeatureRow& operator=( const FeatureRow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ISpatialReferencePtr spatialReference_;
    IFeaturePtr feature_;
    mutable std::auto_ptr< Shape_ABC > shape_;
    //@}
};

}
}

#endif // __FeatureRow_h_
