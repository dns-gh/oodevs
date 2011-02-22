// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_Repartition_h_
#define __ADN_Type_Repartition_h_

#include "ADN_Types.h"

namespace xml
{
    class xistream;
    class ostream;
}

// =============================================================================
/** @class  ADN_Type_Repartition
*/
// Created: MGD 2011-02-22
// =============================================================================
class ADN_Type_Repartition
{
public:
    explicit ADN_Type_Repartition();
    virtual ~ADN_Type_Repartition();

    //! @name Operations
    //@{
    ADN_Type_Repartition& operator =( ADN_Type_Repartition& copy );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    bool CheckNoError();
    void ConnectItem( T_ConnectorVector& vItemConnectors_, bool bConnect );
    //@}

    //! @name Members
    //@{
    ADN_Type_Int male_;
    ADN_Type_Int female_;
    ADN_Type_Int children_;
    //@}
};

#endif // __ADN_Type_Repartition_h_
