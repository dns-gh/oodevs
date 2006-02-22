// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LabelDisplayer_h_
#define __LabelDisplayer_h_

#include "Displayer_ABC.h"
class QLabel;
class Agent;
class DotationType;

// =============================================================================
/** @class  LabelDisplayer
    @brief  LabelDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class LabelDisplayer : public Displayer_ABC
                  , public Caller< bool >
                  , public Caller< Agent >
                  , public Caller< DotationType >
{

public:
    //! @name Constructors/Destructor
    //@{
             LabelDisplayer( QWidget* parent, const char* name, bool bold );
    virtual ~LabelDisplayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LabelDisplayer( const LabelDisplayer& );            //!< Copy constructor
    LabelDisplayer& operator=( const LabelDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void Call( const bool& value );
    virtual void Call( const Agent& value );
    virtual void Call( const DotationType& value );
    //@}

private:
    //! @name Member data
    //@{
    QLabel* valueLabel_;
    QString message_;
    //@}
};

#endif // __LabelDisplayer_h_
