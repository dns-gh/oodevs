// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _QuotasResourcesTable_h
#define _QuotasResourcesTable_h

#include "ResourcesEditorTable_ABC.h"

// =============================================================================
/** @class  QuotasResourcesTable
    @brief  QuotasResourcesTable
*/
// Created: MMC 2013-10-24
// =============================================================================
class QuotasResourcesTable : public ResourcesEditorTable_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             QuotasResourcesTable( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType );
    virtual ~QuotasResourcesTable();
    //@}
};

#endif // _QuotasResourcesTable_h
