// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __EfficientRangeDialog_h_
#define __EfficientRangeDialog_h_

#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class Options;
    class VolumeType;
}

// =============================================================================
/** @class  EfficientRangeDialog
    @brief  EfficientRangeDialog
*/
// Created: JSR 2010-06-07
// =============================================================================
class EfficientRangeDialog : public QDialog
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EfficientRangeDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::VolumeType >& resolver, kernel::Options& options );
    virtual ~EfficientRangeDialog();
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EfficientRangeDialog( const EfficientRangeDialog& );            //!< Copy constructor
    EfficientRangeDialog& operator=( const EfficientRangeDialog& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::VolumeType >& resolver_;
    kernel::Options& options_;
    QComboBox* volumeCombo_;
    QSpinBox* phSpinbox_;
    //@}
};

#endif // __EfficientRangeDialog_h_
