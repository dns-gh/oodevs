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

#include <boost/serialization/export.hpp>

namespace xml
{
    class xostream;
    class xistream;
}

namespace urban
{
    class TerrainObject_ABC;
    class Model;
}

class MIL_Config;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class UrbanObjectWrapper;

// =============================================================================
// @class  UrbanModel
// Created: SLG 2009-10-23
// =============================================================================
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
    void                        ReadUrbanModel          ( const MIL_Config& config );
    void                        SendStateToNewClient    () const;
    static void                 SendCreation            ( urban::TerrainObject_ABC& UrbanBlock );
    urban::TerrainObject_ABC*   FindUrbanBlock          ( unsigned id ) const;
    MT_Float                    GetUrbanBlockCost       ( MT_Float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    void                        CreateObjectWrapper     ( urban::TerrainObject_ABC& object );

    //@}

    //! @name Operations
    //@{
    urban::Model& GetModel();
    static UrbanModel& GetSingleton();
    UrbanObjectWrapper& FindWrapper( const urban::TerrainObject_ABC& terrain );
    UrbanObjectWrapper& FindWrapper( unsigned int id );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    void WriteUrbanModel( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void LoadCapacities( const MIL_Config& config );
    void ReadBlock( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, UrbanObjectWrapper& wrapper );
    //@}

private:
    std::auto_ptr< urban::Model                 > model_;
    std::vector< UrbanObjectWrapper* > urbanWrappers_;

};

BOOST_CLASS_EXPORT_KEY( UrbanModel )

#endif // __UrbanModel_h_

