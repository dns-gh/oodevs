// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Display_h_
#define __Display_h_

#include "astec_kernel/Displayer_ABC.h"
#include "GroupDisplayer.h"

class ItemFactory_ABC;

// =============================================================================
/** @class  DisplayBuilder
    @brief  DisplayBuilder
*/
// Created: AGE 2006-02-09
// =============================================================================
class DisplayBuilder : public Displayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayBuilder( QWidget* parent, ItemFactory_ABC& factory );
    virtual ~DisplayBuilder();
    //@}

    //! @name Operations
    //@{
    GroupDisplayer& AddGroup( const char* name );
    virtual GroupDisplayer& Group( const char* name );
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DisplayBuilder( const DisplayBuilder& );            //!< Copy constructor
    DisplayBuilder& operator=( const DisplayBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, GroupDisplayer* > T_Groups;
    typedef T_Groups::iterator                      IT_Groups;
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    QWidget* parent_;
    T_Groups groups_;
    //@}
};

#endif // __Display_h_
