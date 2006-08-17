// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Data_ABC.h $
// $Author: Ape $
// $Modtime: 18/03/05 18:08 $
// $Revision: 5 $
// $Workfile: ADN_Data_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_Data_ABC_h_
#define __ADN_Data_ABC_h_

#include <qobject.h>

// This includes the typedef below, but also includes (via nested includes ADN_Workspace) which
// causes problems.
//#include "ADN_Types.h"

typedef std::list<std::string>                  T_StringList;
typedef T_StringList::iterator                  IT_StringList;
typedef T_StringList::const_iterator            CIT_StringList;

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Data_ABC
    @brief  ADN_Data_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Data_ABC
: public QObject
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_Data_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Data_ABC();
    virtual ~ADN_Data_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const = 0;
    virtual void Reset() = 0;
    virtual void Load();
    virtual void Save();
    //@}

protected:
    virtual void ReadArchive( ADN_XmlInput_Helper& input );
    virtual void WriteArchive( MT_OutputArchive_ABC& output );

};


#endif // __ADN_Data_ABC_h_
