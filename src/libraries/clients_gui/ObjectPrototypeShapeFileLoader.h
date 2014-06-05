// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ObjectPrototypeShapeFileLoader_h_
#define __ObjectPrototypeShapeFileLoader_h_

#include "ObjectPrototypeLoader_ABC.h"
#include <boost/shared_ptr.hpp>

class OGRDataSource;
class OGRLayer;
class OGRFeature;

namespace kernel
{
    class ObjectType;
    class StaticModel;
    class CoordinateConverter_ABC;
}

namespace tools
{
    class Path;
}

namespace gui
{
// =============================================================================
/** @class  ObjectPrototypeShapeFileLoader
    @brief  ObjectPrototypeShapeFileLoader
*/
// Created: BCI 2011-05-09
// =============================================================================
class ObjectPrototypeShapeFileLoader : public ObjectPrototypeLoader_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    ObjectPrototypeShapeFileLoader( const kernel::CoordinateConverter_ABC& coordinateConverter, QWidget* parent, const tools::Path& filename, const kernel::ObjectType& objectType );
    virtual ~ObjectPrototypeShapeFileLoader();
    //@}

    //! @name Operations
    //@{
    virtual QStringList GetFields() const;
    virtual void StartLoad();
    virtual bool LoadNext();
    virtual const kernel::Location_ABC& GetCurrentLocation() const;
    virtual QString GetCurrentFieldValueAsString( const QString& ) const;
    virtual int GetCurrentFieldValueAsInt( const QString& ) const;
    virtual bool GetCurrentFieldValueAsBool( const QString& ) const;
    virtual bool CheckValidity( RichLabel& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPrototypeShapeFileLoader( const ObjectPrototypeShapeFileLoader& );            //!< Copy constructor
    ObjectPrototypeShapeFileLoader& operator=( const ObjectPrototypeShapeFileLoader& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static bool CanLoadLayer( OGRLayer& layer, const kernel::ObjectType& objectType, QString& unloadableExplanation );
    //@}

private:
    //! @name Member data
    //@{
    QStringList fields_;
    const kernel::ObjectType& objectType_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    boost::shared_ptr< OGRDataSource > dataSource_;
    OGRLayer* currentLayer_;
    OGRFeature* currentFeature_;
    std::unique_ptr< kernel::Location_ABC > currentLocation_;
    unsigned int unsupportedShapes_;
    unsigned int unsupportedPoints_;
    //@}
};

} // end namespace gui

#endif // __ObjectPrototypeShapeFileLoader_h_
