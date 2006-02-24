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

#include "Displayer_ABC.h"

class Unit;

// =============================================================================
/** @class  SpinBoxDisplayer
    @brief  SpinBoxDisplayer
*/
// Created: AGE 2006-02-23
// =============================================================================
class SpinBoxDisplayer : public Displayer_ABC
                       , public Caller< double >
                       , public Caller< float >
                       , public Caller< int >
                       , public Caller< Unit >
{

public:
    //! @name Constructors/Destructor
    //@{
             SpinBoxDisplayer( QWidget* parent, const char* name, int min, int max, int step );
    virtual ~SpinBoxDisplayer();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SpinBoxDisplayer( const SpinBoxDisplayer& );            //!< Copy constructor
    SpinBoxDisplayer& operator=( const SpinBoxDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Hide();
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    virtual void Call( const double& value );
    virtual void Call( const float& value );
    virtual void Call( const int& value );
    virtual void Call( const Unit& value );
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* edit_;
    //@}
};

#endif // __SpinBoxDisplayer_h_
