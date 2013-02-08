// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef GEOSTORE_GEOMETRY_COLLECTION
#define GEOSTORE_GEOMETRY_COLLECTION

#include <spatialite/spatialite/gaiageo.h>

namespace geostore
{

class GeometryCollection
{
public:
    GeometryCollection( gaiaGeomCollPtr p )
        : p_( p, &gaiaFreeGeomColl )
    {}
    GeometryCollection& operator=( gaiaGeomCollPtr p )
    {
        p_.reset( p, &gaiaFreeGeomColl );
        return *this;
    }
    gaiaGeomCollPtr operator->()
    {
        return p_.get();
    }
    operator gaiaGeomCollPtr()
    {
        return p_.get();
    }
private:
    boost::shared_ptr< gaiaGeomColl > p_;
};

}

#endif // GEOSTORE_GEOMETRY_COLLECTION
