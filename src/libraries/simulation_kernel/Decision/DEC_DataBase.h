// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_DataBase_h_
#define __DEC_DataBase_h_

#include <directia5/Library.h>

namespace directia
{
    class Brain;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DEC_DataBase
    @brief  DEC_DataBase
*/
// Created: MGD 2009-08-06
// =============================================================================
class DEC_DataBase : public directia5::Library
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_DataBase( xml::xistream& xis, const std::string strPath );
    virtual ~DEC_DataBase();
    //@}

    //! @name Operations
    //@{
    void InitKnowledges( directia::Brain& brain );
    //@}

private:
    std::vector< const std::string > knowledges_;
};

#endif // __DEC_DataBase_h_
