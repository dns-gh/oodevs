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

#include <masalife/brain/library/Library.h>

namespace directia
{
    namespace brain
    {
        class Brain;
    }
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
class DEC_DataBase // : public masalife::brain::library::Library
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_DataBase( xml::xistream& xis, const std::string& strPath );
             DEC_DataBase( masalife::brain::library::Library::T_Paths libPaths, const std::vector< const std::string >& knowledge );
    virtual ~DEC_DataBase();
    //@}

    //! @name Operations
    //@{
    void InitKnowledges( directia::brain::Brain& brain );
    //@}

    //! @name Checkpoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    template< typename Archive > friend void save_construct_data( Archive& archive, const DEC_DataBase* role, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, DEC_DataBase* role, const unsigned int /*version*/ );
    //@}

private:
    std::vector< const std::string > knowledges_;
    masalife::brain::library::Library::T_Paths libPaths_;
};

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_DataBase* database, const unsigned int /*version*/ )
{
    archive << database->libPaths_
            << database->knowledges_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_DataBase* database, const unsigned int /*version*/ )
{
    masalife::brain::library::Library::T_Paths libPaths;
    std::vector< const std::string > knowledges;
    archive >> libPaths
            >> knowledges;
    ::new( database )DEC_DataBase( libPaths, knowledges );
}


#endif // __DEC_DataBase_h_
