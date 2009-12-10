// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanModel_h_
#define __UrbanModel_h_


namespace xml
{
    class xostream;
    class xistream;
}


class MIL_Config;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
// @class  UrbanModel
// Created: SLG 2009-10-23
// =============================================================================

namespace urban
{
    class Model;
    class StaticModel;
    class TerrainObject_ABC;
    class Block;

class UrbanModel
{

public:

    //! @name Constructors/Destructor
    //@{
    UrbanModel();
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void            ReadUrbanModel          ( const MIL_Config& config );
    void            SendStateToNewClient    () const;
    static void     SendCreation            ( urban::Block& UrbanBlock );
    urban::Block*   FindUrbanBlock          ( unsigned id ) const;
    MT_Float        GetUrbanBlockCost       ( MT_Float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    urban::Model&   GetModel() const;
    //@}

    //! @name Network
    //@{
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteUrbanModel( xml::xostream& xos ) const;
    //@}

private:
    std::auto_ptr< urban::Model       > model_;
    std::auto_ptr< urban::StaticModel > staticModel_;
};

}

#endif // __MIL_EntityManager_h_

