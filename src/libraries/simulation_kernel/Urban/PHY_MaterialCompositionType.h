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
    struct AttritionData
    {
        //! @name Member data
        //@{
        std::string protection_;
        float destruction_;
        float repairableWithEvac_;
        float repairableNoEvac_;
        //@}
    };
    typedef std::map< std::string, AttritionData > T_AttritionInfos;
    //@}

public:
    //! @name Destructor
    //@{
    ~PHY_MaterialCompositionType();
    //@}

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
    const PHY_MaterialCompositionType::AttritionData* FindAttrition( const std::string& protection ) const;
    //@}

private:
    //! @name Constructors
    //@{
    PHY_MaterialCompositionType( const std::string& name, xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    static void ReadMaterialComposition( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const unsigned int nId_;
    const T_AttritionInfos attritions_;
    //@}
};

#endif // __PHY_MaterialCompositionType_h_
