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

signals:
    //! @name Signals
    //@{
    void DotationsQuotasComputed( const std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > >& );
    //@}

private:
    //! @name Operations
    //@{
    virtual void SupplyHierarchy( const gui::LogisticHierarchiesBase& logHierarchy );
    //@}
};

#endif // _LogisticQuotaEditor_h
