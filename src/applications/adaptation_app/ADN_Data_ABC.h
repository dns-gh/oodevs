// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Data_ABC_h_
#define __ADN_Data_ABC_h_

#include <boost/noncopyable.hpp>
#include <qobject.h>
#include <list>

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class Loader_ABC;
}

typedef std::list<std::string>         T_StringList;
typedef T_StringList::iterator        IT_StringList;
typedef T_StringList::const_iterator CIT_StringList;

// =============================================================================
/** @class  ADN_Data_ABC
    @brief  ADN_Data_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Data_ABC : public QObject
                   , private boost::noncopyable
{
    Q_OBJECT

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
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Save();
    virtual bool IsValidDatabase();
    //@}

protected:
    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );

};


#endif // __ADN_Data_ABC_h_
