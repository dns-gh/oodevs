// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_DisplayGroup_h_
#define __MOS_DisplayGroup_h_

#include <qgroupbox.h>
#include <map>

class MOS_DisplayItem;

// =============================================================================
/** @class  MOS_DisplayGroup
    @brief  MOS_DisplayGroup
*/
// Created: AGE 2006-02-09
// =============================================================================
class MOS_DisplayGroup : public QGroupBox
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DisplayGroup( QWidget* parent, const char* name  );
    virtual ~MOS_DisplayGroup();
    //@}

    //! @name Operations
    //@{
    MOS_DisplayGroup& AddItem( const char* name, bool bold = false );

    template< typename T >
    MOS_DisplayGroup& Display( const char* name, const T& value )
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
    MOS_DisplayGroup( const MOS_DisplayGroup& );            //!< Copy constructor
    MOS_DisplayGroup& operator=( const MOS_DisplayGroup& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, MOS_DisplayItem* > T_Items;
    typedef T_Items::iterator                        IT_Items;
    //@}

    //! @name Helpers
    //@{
    MOS_DisplayItem& GetItem( const char* name );
    //@}

private:
    //! @name Member data
    //@{
    T_Items items_;
    //@}
};

#endif // __MOS_DisplayGroup_h_
