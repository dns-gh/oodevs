// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayItem_h_
#define __DisplayItem_h_

#include "Displayer_ABC.h"
class QLabel;
class Agent;

// =============================================================================
/** @class  DisplayItem
    @brief  DisplayItem
*/
// Created: AGE 2006-02-09
// =============================================================================
class DisplayItem : public Displayer_ABC
                  , public Caller< bool >
                  , public Caller< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayItem( QWidget* parent, const char* name, bool bold );
    virtual ~DisplayItem();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DisplayItem( const DisplayItem& );            //!< Copy constructor
    DisplayItem& operator=( const DisplayItem& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void Call( const bool& value );
    virtual void Call( const Agent& value );
    //@}

private:
    //! @name Member data
    //@{
    QLabel* valueLabel_;
    QString message_;
    //@}
};

#endif // __DisplayItem_h_
