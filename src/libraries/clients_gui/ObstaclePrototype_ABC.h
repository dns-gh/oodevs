// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstaclePrototype_ABC_h_
#define __ObstaclePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/tools.h"
#include "ValuedComboBox.h"

namespace gui
{
    class LoadableCheckBox;
    class LoadableDateTimeEdit;
    class LoadableTimeEdit;

// =============================================================================
/** @class  ObstaclePrototype_ABC
    @brief  ObstaclePrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class ObstaclePrototype_ABC : public ObjectAttributePrototype_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObstaclePrototype_ABC( QWidget* parent );
    virtual ~ObstaclePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity( const kernel::Team_ABC& ) const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

private slots:
    //! @name slots
    //@{
    void OnActivationTypeChanged( int index );
    void OnDeactivationTypeChanged( int index );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QDateTime GetCreationDate() const = 0;
    int GetActivationTime() const;
    int GetActivityTime() const;
    bool IsActivated() const;
    //@}

protected:
    //! @name Types
    //@{
    enum EActivationType
    {
        eActivationImmediately,
        eActivationDelay,
        eActivationDate,
        eActivationInactive
    };

    enum EDeactivationType
    {
        eDeactivationNever,
        eDeactivationDelay,
        eDeactivationDate
    };
    //@}

protected:
    //! @name Member data
    //@{
    QComboBox* activationCombo_;
    RichGroupBox* deactivationGroup_;
    QComboBox* deactivationCombo_;
    LoadableTimeEdit* activationDelay_;
    LoadableDateTimeEdit* activationDate_;
    LoadableTimeEdit* deactivationDelay_;
    LoadableDateTimeEdit* deactivationDate_;
    //@}
};

}

#endif // __ObstaclePrototype_ABC_h_
