// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FeatureClass_h_
#define __FeatureClass_h_

#include "Table.h"
#include "ESRI.h"

namespace plugins
{
namespace crossbow
{
    class FeatureRow;

// =============================================================================
/** @class  FeatureClass
    @brief  FeatureClass
*/
// Created: SBO 2007-08-30
// =============================================================================
class FeatureClass : public Table
{

public:
    //! @name Constructors/Destructor
    //@{
             FeatureClass( IFeatureClassPtr featureClass, const std::string& name );
    virtual ~FeatureClass();
    //@}

    //! @name Operations
    //@{
    virtual Row_ABC& CreateRow();
    virtual void UpdateRow( const Row_ABC& row );
    virtual Row_ABC* Find( const std::string& query );
    virtual Row_ABC* GetNextRow();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FeatureClass( const FeatureClass& );            //!< Copy constructor
    FeatureClass& operator=( const FeatureClass& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeSpatialReference();
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    IFeatureClassPtr featureClass_;
    ISpatialReferencePtr spatialReference_;
    IFeatureCursorPtr cursor_;
    std::auto_ptr< FeatureRow > feature_;
    //@}
};

}
}

#endif // __FeatureClass_h_
