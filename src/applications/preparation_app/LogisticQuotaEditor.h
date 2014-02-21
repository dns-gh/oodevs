// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _LogisticQuotaEditor_h
#define _LogisticQuotaEditor_h

#include "LogisticEditor.h"

// =============================================================================
/** @class  LogisticQuotaEditor
    @brief  Logistic quota editor
*/
// Created: SLI 2014-02-19
// =============================================================================
class LogisticQuotaEditor : public LogisticEditor
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticQuotaEditor( QWidget* parent, kernel::Controllers& controllers, const ::StaticModel& staticModel );
    virtual ~LogisticQuotaEditor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void SupplyHierarchy( const kernel::Entity_ABC& entity, const gui::LogisticHierarchiesBase& logHierarchy );
    //@}

private:
    //! @name Helpers
    //@{
    void SetQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const T_Requirements& requirements );
    //@}
};

#endif // _LogisticQuotaEditor_h
