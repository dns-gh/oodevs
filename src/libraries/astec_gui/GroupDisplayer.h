// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GroupDisplayer_h_
#define __GroupDisplayer_h_

#include "Displayer_ABC.h"

#include <qlabel.h>
#include <qgroupbox.h>
#include <map>

class LabelDisplayer;
class ItemFactory_ABC;

// =============================================================================
/** @class  GroupDisplayer
    @brief  GroupDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class GroupDisplayer : public Displayer_ABC, public QGroupBox
{

public:
    //! @name Constructors/Destructor
    //@{
             GroupDisplayer( QWidget* parent, const char* name, ItemFactory_ABC& factory );
    virtual ~GroupDisplayer();
    //@}

    //! @name Operations
    //@{
    GroupDisplayer& AddLabel( const char* name, bool bold = false );
    GroupDisplayer& AddSpinBox( const char* name, int min, int max, int step );
    GroupDisplayer& AddCheckBox( const char* name );
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GroupDisplayer( const GroupDisplayer& );            //!< Copy constructor
    GroupDisplayer& operator=( const GroupDisplayer& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, Displayer_ABC* > T_Items;
    typedef T_Items::iterator                      IT_Items;
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    T_Items items_;
    //@}
};

#endif // __GroupDisplayer_h_
