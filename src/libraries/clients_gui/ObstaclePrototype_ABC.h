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
    virtual bool CheckValidity() const;
    //@}
    
signals:
    //! @name signals
    //@{
    void ToggleActivable( bool );
    //@}

private slots:
    //! @name slots
    //@{
    void OnObstacleTypeChanged();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObstaclePrototype_ABC( const ObstaclePrototype_ABC& );            //!< Copy constructor
    ObstaclePrototype_ABC& operator=( const ObstaclePrototype_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );    
    //@}

protected:
    //! @name Member data
    //@{
    ValuedComboBox< E_DemolitionTargetType >* types_;
    QCheckBox*                                bActivated_;
    //@}
};

}

#endif // __ObstaclePrototype_ABC_h_
