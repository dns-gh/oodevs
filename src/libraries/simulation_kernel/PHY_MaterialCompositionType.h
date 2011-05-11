// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_MaterialCompositionType_h_
#define __PHY_MaterialCompositionType_h_

#include "MT_Tools/MT_String.h"

// =============================================================================
/** @class  PHY_MaterialCompositionType
    @brief  PHY_MaterialCompositionType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_MaterialCompositionType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_MaterialCompositionType* > T_MaterialCompositionMap;
    typedef T_MaterialCompositionMap::const_iterator CIT_MaterialCompositionMap;

    struct AttritionData
    {
        //! @name Constructors/Destructor
        //@{
        explicit AttritionData( const std::string& protection, xml::xistream& xis );
        //@}

        //! @name Member data
        //@{
        std::string protection_;
        float destruction_;
        float repairableWithEvac_;
        float repairableNoEvac_;
        //@}
    };
    typedef std::map< std::string, AttritionData* > T_AttritionInfos;
    typedef T_AttritionInfos::const_iterator      CIT_AttritionInfos;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static const PHY_MaterialCompositionType* Find( const std::string& strName );
    static unsigned int Count();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    PHY_MaterialCompositionType::AttritionData* FindAttrition( const std::string& protection ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PHY_MaterialCompositionType( const std::string& name, xml::xistream& xis );
    virtual ~PHY_MaterialCompositionType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadMaterialComposition( xml::xistream& xis );
    void ReadAttrition( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    static T_MaterialCompositionMap materialCompositions_;
    static unsigned int nNextId_;
    std::string name_;
    unsigned int nId_;
    T_AttritionInfos attritions_;
    //@}
};

#endif // __PHY_MaterialCompositionType_h_
