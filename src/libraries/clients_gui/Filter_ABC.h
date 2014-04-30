// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_Filter_ABC_h
#define __gui_Filter_ABC_h

#include <boost/noncopyable.hpp>

namespace gui
{
    class StandardModel;

// =============================================================================
/** @class  Filter_ABC
    @brief  Filter definition
*/
// Created: ABR 2014-04-25
// =============================================================================
class Filter_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Filter_ABC() {}
    virtual ~Filter_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual bool Apply( QStandardItem& item ) const = 0;
    virtual void Clear() = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Refresh( const std::function< bool( const QString& ) >& /* filter */ )
    {
        // NOTHING
    }
    //@}
};
 
} //! namespace gui

#endif // __gui_Filter_ABC_h
