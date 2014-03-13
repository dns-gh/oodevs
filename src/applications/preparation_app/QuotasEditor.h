// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef _QuotasEditor_h
#define _QuotasEditor_h

#include "tools/Resolver.h"

namespace gui
{
    class LogisticHierarchiesBase;
}

namespace kernel
{
    class DotationType;
    class Entity_ABC;
    class LogisticSupplyClass;
}

class QuotasResourcesTable;
class StaticModel;

// =============================================================================
/** @class  QuotasEditor
    @brief  QuotasEditor
*/
// Created: JSR 2014-03-05
// =============================================================================
class QuotasEditor : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             QuotasEditor( QWidget* parent, const StaticModel& staticModel );
    virtual ~QuotasEditor();
    //@}

public:
    //! @name Operations
    //@{
    void Initialize( const kernel::Entity_ABC& entity );
    void ApplyQuotas() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ApplyQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const std::map< const kernel::DotationType*, unsigned int >& generatedQuotas ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void NotifyQuotasUserChange();
    void NotifyAutomaticQuotas( const std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > >& quotas );
    void OnSubordinateChanged( int i );
    //@}

private:
    //! @name Member data
    //@{
    const tools::StringResolver< kernel::LogisticSupplyClass >& supplyClasses_;
    QComboBox* subordinateCombo_;
    QuotasResourcesTable* quotasTableView_;
    std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > > quotasByEntity_;
    //@}
};

#endif // _QuotasEditor_h
