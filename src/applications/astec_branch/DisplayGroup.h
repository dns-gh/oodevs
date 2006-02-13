// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayGroup_h_
#define __DisplayGroup_h_

#include <qgroupbox.h>
#include <map>

class DisplayItem;

// =============================================================================
/** @class  DisplayGroup
    @brief  DisplayGroup
*/
// Created: AGE 2006-02-09
// =============================================================================
class DisplayGroup : public QGroupBox
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayGroup( QWidget* parent, const char* name  );
    virtual ~DisplayGroup();
    //@}

    //! @name Operations
    //@{
    DisplayGroup& AddItem( const char* name, bool bold = false );

    template< typename T >
    DisplayGroup& Display( const char* name, const T& value )
    {
        GetItem( name ).Display( value );
        show();
        return *this;
    }

    void Clear();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DisplayGroup( const DisplayGroup& );            //!< Copy constructor
    DisplayGroup& operator=( const DisplayGroup& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, DisplayItem* > T_Items;
    typedef T_Items::iterator                        IT_Items;
    //@}

    //! @name Helpers
    //@{
    DisplayItem& GetItem( const char* name );
    //@}

private:
    //! @name Member data
    //@{
    T_Items items_;
    //@}
};

#endif // __DisplayGroup_h_
