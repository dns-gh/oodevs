// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Automata_ListView_h_
#define __ADN_Automata_ListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Automata_ListView
    @brief  ADN_Automata_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Automata_ListView : public ADN_ListView
                            , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Automata_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_Automata_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Automata_ListView_h_
