// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateTable_ABC_h_
#define __InitialStateTable_ABC_h_

#include <boost/noncopyable.hpp>

class InitialState;

// =============================================================================
/** @class  InitialStateTable_ABC
    @brief  InitialStateTable_ABC
*/
// Created: ABR 2011-02-24
// =============================================================================
class InitialStateTable_ABC : public QTable
                            , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InitialStateTable_ABC( int numRows, int numCols, QWidget* parent, const char* name = 0 );
    virtual ~InitialStateTable_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void ReadExtension( const InitialState& extension ) = 0;
    virtual void WriteExtension( InitialState& extension ) const = 0;
    //@}
};

#endif // __InitialStateTable_ABC_h_
