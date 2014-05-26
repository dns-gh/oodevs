// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ObjectPrototypeLoader_ABC_h_
#define __ObjectPrototypeLoader_ABC_h_

namespace kernel
{
    class Location_ABC;
}

namespace gui
{
    class RichLabel;
// =============================================================================
/** @class  ObjectPrototypeLoader_ABC
    @brief  ObjectPrototypeLoader_ABC
*/
// Created: BCI 2011-05-09
// =============================================================================
class ObjectPrototypeLoader_ABC
{
public:
    struct LoadCancelledException {};

public:

    //! @name Constructors/Destructor
    //@{
             ObjectPrototypeLoader_ABC();
    virtual ~ObjectPrototypeLoader_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QStringList GetFields() const = 0;
    virtual void StartLoad() = 0;
    virtual bool LoadNext() = 0;
    virtual const kernel::Location_ABC& GetCurrentLocation() const = 0;
    virtual QString GetCurrentFieldValueAsString( const QString& ) const = 0;
    virtual int GetCurrentFieldValueAsInt( const QString& ) const = 0;
    virtual bool GetCurrentFieldValueAsBool( const QString& ) const = 0;
    virtual bool CheckValidity( RichLabel& result ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPrototypeLoader_ABC( const ObjectPrototypeLoader_ABC& );            //!< Copy constructor
    ObjectPrototypeLoader_ABC& operator=( const ObjectPrototypeLoader_ABC& ); //!< Assignment operator
    //@}
};

} // end namespace gui

#endif // __ObjectPrototypeLoader_ABC_h_
