// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_OGR_FeatureClass_h_
#define __crossbow_OGR_FeatureClass_h_

#include "Table_ABC.h"

class OGRSpatialReference;
class OGRLayer;

namespace plugins
{
namespace crossbow
{
    class OGR_FeatureRow;
    class DatabaseEditor_ABC;

// =============================================================================
/** @class  OGR_FeatureClass
    @brief  OGR_FeatureClass
*/
// Created: JCR 2010-02-24
// =============================================================================
class OGR_FeatureClass : public Table_ABC
{ 

public:
    //! @name Constructors/Destructor
    //@{
    explicit OGR_FeatureClass( OGRLayer& layer );
    virtual ~OGR_FeatureClass();
    //@}

    //! @name Operations
    //@{
    Row_ABC& CreateRow();
    void DeleteRows( const std::string& query );
    void UpdateRow( const Row_ABC& row );
    void InsertRow( const Row_ABC& row );
    void Clear();

    Row_ABC* Find( const std::string& query, bool forceUpdate = false );
    Row_ABC* GetNextRow();

    void BeginTransaction();
    void EndTransaction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OGR_FeatureClass( const OGR_FeatureClass& );            //!< Copy constructor
    OGR_FeatureClass& operator=( const OGR_FeatureClass& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    OGRLayer& layer_;
    std::auto_ptr< crossbow::DatabaseEditor_ABC > editor_;
    std::auto_ptr< crossbow::OGR_FeatureRow > feature_;
    //@}
};

} // end namespace crossbow
} // end namespace pluginss

#endif // __OGR_FeatureClass_h_
