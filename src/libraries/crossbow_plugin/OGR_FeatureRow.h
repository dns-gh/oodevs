// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_OGR_FeatureRow_h_
#define __crossbow_OGR_FeatureRow_h_

#include "FieldVariant.h"
#include "Row_ABC.h"
#include "Shape_ABC.h"

class OGRFeature;
class OGRLayer;

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  OGR_FeatureRow
    @brief  OGR_FeatureRow
*/
// Created: JCR 2010-02-24
// =============================================================================
class OGR_FeatureRow : public Row_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OGR_FeatureRow( OGRSpatialReference* spatialReference );
    virtual ~OGR_FeatureRow();
    //@}

    //! @name Operations
    //@{
    void SetField( const std::string& field, const FieldVariant& value );
    void SetGeometry( const Shape_ABC& value );
    //@}

    //! @name Accessors
    //@{
    FieldVariant GetField( const std::string& name ) const;
    Shape_ABC& GetGeometry() const;
    long GetID() const;
    //@}

    //! @name Tools
    //@{
    void Insert( OGRLayer& layer ) const;
    void BindFeature( OGRFeature& feature, long id );
    //@}
    

private:
    //! @name Copy/Assignment
    //@{
    OGR_FeatureRow( const OGR_FeatureRow& );            //!< Copy constructor
    OGR_FeatureRow& operator=( const OGR_FeatureRow& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    void Release();
    //@}

private:
    //! @name Member data
    //@{
    OGRFeature* feature_;
    OGRSpatialReference* spatialReference_;
    mutable std::auto_ptr< Shape_ABC > shape_;
    //@}
};

} // end namespace crossbow
} // end namespace plugins

#endif // __OGR_FeatureRow_h_
