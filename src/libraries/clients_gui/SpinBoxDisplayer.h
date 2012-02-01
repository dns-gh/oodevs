// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SpinBoxDisplayer_h_
#define __SpinBoxDisplayer_h_

#include "clients_kernel/Displayer_ABC.h"

namespace kernel
{
    class Unit;
}

namespace gui
{

// =============================================================================
/** @class  SpinBoxDisplayer
    @brief  SpinBoxDisplayer
*/
// Created: AGE 2006-02-23
// =============================================================================
class SpinBoxDisplayer : public kernel::Displayer_ABC
                       , public tools::Caller< double >
                       , public tools::Caller< float >
                       , public tools::Caller< int >
                       , public tools::Caller< kernel::Unit >
                       , public tools::Caller< kernel::ValueNotSet >
{
public:
    //! @name Constructors/Destructor
    //@{
             SpinBoxDisplayer( QBoxLayout* parent, const QString& name, int min, int max, int step );
    virtual ~SpinBoxDisplayer();
    //@}

    //! @name Operations
    //@{
    int GetValue() const;
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Hide();
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    virtual void Call( const double& value );
    virtual void Call( const float& value );
    virtual void Call( const int& value );
    virtual void Call( const kernel::Unit& value );
    virtual void Call( const kernel::ValueNotSet& value );
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* edit_;
    //@}
};

}

#endif // __SpinBoxDisplayer_h_
