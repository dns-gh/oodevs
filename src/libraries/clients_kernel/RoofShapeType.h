// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KernelRoofShapeType_h_
#define __KernelRoofShapeType_h_

#include <string>

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  RoofShapeType
    @brief  RoofShapeType
*/
// Created: SLG 2010-11-17
// =============================================================================
class RoofShapeType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RoofShapeType( xml::xistream& xis );
    virtual ~RoofShapeType();
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RoofShapeType( const RoofShapeType& );            //!< Copy constructor
    RoofShapeType& operator=( const RoofShapeType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    static unsigned int nNextID_;
    //@}
};

}

#endif // __RoofShapeType_h_
