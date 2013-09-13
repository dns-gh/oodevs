// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KernelMaterialCompositionType_h_
#define __KernelMaterialCompositionType_h_

#include <string>
#include <map>

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  MaterialCompositionType
@brief  MaterialCompositionType
*/
// Created: SLG 2010-11-17
// =============================================================================
class MaterialCompositionType
{
public:
    //! @name Types
    //@{
    struct AttritionData
    {
        //! @name Constructors/Destructor
        //@{
        explicit AttritionData( xml::xistream& xis );
        //@}

        //! @name Member data
        //@{
        std::string protection_;
        float       destruction_;
        float       repairableWithEvac_;
        float       repairableNoEvac_;
        //@}
    };

    typedef std::map< std::string, AttritionData > T_AttritionInfos;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit MaterialCompositionType( xml::xistream& xis );
    virtual ~MaterialCompositionType();
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    const MaterialCompositionType::AttritionData* FindAttrition( const std::string& protection ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    MaterialCompositionType& operator=( const MaterialCompositionType& ); //!< Assignment operator
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MaterialCompositionType( const MaterialCompositionType& );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    void ReadAttrition( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    T_AttritionInfos attritions_;
    static unsigned int nNextID_;
    //@}
};

}

#endif // __MaterialCompositionType_h_
