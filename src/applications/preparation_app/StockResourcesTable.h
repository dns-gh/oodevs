// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _StockResourcesTable_h
#define _StockResourcesTable_h

#include "ResourcesEditorTable_ABC.h"

// =============================================================================
/** @class  StockResourcesTable
    @brief  StockResourcesTable
*/
// Created: MMC 2013-10-24
// =============================================================================
class StockResourcesTable : public ResourcesEditorTable_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             StockResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType );
    virtual ~StockResourcesTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddResource( const kernel::DotationType& resource, int value = 0 );
    virtual void UpdateLine( int row, int value );
    virtual void CustomizeMenuAction( QAction* action, const kernel::DotationType& actionDotation ) const;
    void SetAllowedNatures( const std::set< std::string >& allowedNatures );
    //@}

private:
    //! @name Member data
    //@{
    std::set< std::string > allowedNatures_;
    //@}
};

#endif // _StockResourcesTable_h
