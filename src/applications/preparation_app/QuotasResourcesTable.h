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

#include "LogisticEditor.h" // todo for typedef
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

public:
    //! @name Operations
    //@{
    void SetQuotas( const LogisticEditor::T_Requirements& stocks );
    //@}
};

#endif // _QuotasResourcesTable_h
