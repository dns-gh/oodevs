// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceTools_ABC_h_
#define __ResourceTools_ABC_h_

namespace resource
{

// =============================================================================
/** @class  ResourceTools_ABC
    @brief  ResourceTools_ABC
*/
// Created: JSR 2010-09-03
// =============================================================================
class ResourceTools_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceTools_ABC() {}
    virtual ~ResourceTools_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetResourceName( unsigned long id ) const = 0;
    virtual unsigned long GetResourceId( const std::string& name ) const = 0;
    virtual unsigned int GetIdFromSimulation( unsigned int urbanId ) const = 0;
    virtual bool ConvertIdToUrbanId( unsigned int& urbanId ) const = 0;
    //@}
};

}

#endif // __ResourceTools_ABC_h_
