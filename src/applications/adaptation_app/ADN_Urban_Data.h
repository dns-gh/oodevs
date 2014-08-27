// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Urban_Data_h_
#define __ADN_Urban_Data_h_

#include "ADN_AttritionInfos.h"
#include "ADN_CapacityInfos.h"
#include "ADN_Data_ABC.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Symbols_Data.h"
#include "ADN_RefWithName.h"

class ADN_Urban_Data : public ADN_Data_ABC
{
//*****************************************************************************
public:
    class AccommodationInfos : public ADN_RefWithLocalizedName
    {
    public:
                 AccommodationInfos();
        explicit AccommodationInfos( xml::xistream& input );
        virtual ~AccommodationInfos();

        bool operator==( const std::string& str );
        void WriteAccommodation( xml::xostream& output ) const;
        AccommodationInfos* CreateCopy();

    public:
        //! @name Member Data
        //@{
        ADN_Type_Double nominalCapacity_;
        ADN_Type_Double maxCapacity_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< AccommodationInfos > T_AccommodationInfos_Vector;

//*****************************************************************************
public:
    class InfrastructureInfos : public ADN_RefWithLocalizedName
    {
    public:
                 InfrastructureInfos();
        explicit InfrastructureInfos( xml::xistream& input );
        virtual ~InfrastructureInfos();

        bool operator==( const std::string& str );
        void WriteInfrastructure( xml::xostream& output ) const;
        InfrastructureInfos* CreateCopy();

        typedef std::map< std::string, boost::shared_ptr< helpers::ADN_TypeCapacity_Infos > > T_CapacityMap;

    private:
        void ReadCapacityArchive( const std::string& type, xml::xistream& xis );

    public:
        //! @name Member Data
        //@{
        ADN_TypePtr_InVector_ABC< ADN_Symbols_Data::SymbolsInfra > pSymbol_;
        ADN_Type_Bool bMedical_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< InfrastructureInfos > T_InfrastructureInfos_Vector;

//*****************************************************************************
public:
    class UrbanMaterialInfos : public ADN_RefWithLocalizedName
    {
    public:
                 UrbanMaterialInfos();
        explicit UrbanMaterialInfos( xml::xistream& input );
        virtual ~UrbanMaterialInfos();

        bool operator==( const std::string& str );
        void WriteMaterial( xml::xostream& output ) const;
        UrbanMaterialInfos* CreateCopy();

    private:
        //! @name Helpers
        //@{
        void ReadAttrition( xml::xistream& input );
        void CreateDefaultAttritionInfos();
        //@}

    public:
        //! @name Member Data
        //@{
        helpers::T_AttritionInfos_Vector vAttritionInfos_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< UrbanMaterialInfos > T_UrbanMaterialInfos_Vector;

//*****************************************************************************

public:
    class RoofShapeInfos : public ADN_RefWithLocalizedName
    {
    public:
                 RoofShapeInfos();
        explicit RoofShapeInfos( xml::xistream& input );
        virtual ~RoofShapeInfos();

        bool operator==( const std::string& str );
        void WriteRoofShape( xml::xostream& output ) const;
        RoofShapeInfos* CreateCopy() { return new RoofShapeInfos(); }
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    };

    typedef ADN_Type_Vector_ABC< RoofShapeInfos > T_RoofShapeInfos_Vector;

//*****************************************************************************

public:
    class UrbanInfos : public ADN_RefWithName
    {
    public:
        UrbanInfos() {}
        UrbanInfos* CreateCopy() { return new UrbanInfos(); }
        std::string GetItemName() { return strName_.GetData(); }
    };
    typedef ADN_Type_Vector_ABC< UrbanInfos > T_UrbanInfos_Vector;

//*****************************************************************************

    class UsageTemplateInfos : public ADN_CrossedRef< AccommodationInfos >
    {
    public:
                 UsageTemplateInfos();
        explicit UsageTemplateInfos( xml::xistream& input );
        explicit UsageTemplateInfos( ADN_Urban_Data::AccommodationInfos& accomodation, ADN_Type_Int proportion );

        virtual ~UsageTemplateInfos();

        void Write( xml::xostream& output ) const;
        UsageTemplateInfos* CreateCopy();

    public:
        ADN_Type_Int proportion_;
    };

//*****************************************************************************

public:
    class T_UsageTemplateInfosVector : public ADN_Type_Vector_ABC< UsageTemplateInfos >
    {
    public:
        explicit T_UsageTemplateInfosVector( ADN_Ref_ABC* parent, bool bAutoRef = true );
        virtual ~T_UsageTemplateInfosVector();

    protected:
        virtual bool AddItemPrivate( void* pObj );

    private:
        ADN_Ref_ABC* parent_;
    };

//*****************************************************************************

public:
    class UrbanTemplateInfos : public ADN_RefWithLocalizedName
    {
    public:
                 UrbanTemplateInfos();
        explicit UrbanTemplateInfos( xml::xistream& input );
        virtual ~UrbanTemplateInfos();

        void Write( xml::xostream& output ) const;
        UrbanTemplateInfos* CreateCopy();

    private:
        void ReadUsage( xml::xistream& xis );

    public:
        //! @name Member Data
        //@{
        ADN_Type_String color_;
        ADN_Type_Int alpha_;
        ADN_Type_Int height_;
        ADN_Type_Int floor_;
        ADN_Type_Int parking_;
        ADN_Type_Double occupation_;
        ADN_Type_Int trafficability_;
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::UrbanMaterialInfos > ptrMaterial_;
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::RoofShapeInfos > ptrRoofShape_;
        T_UsageTemplateInfosVector usages_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< UrbanTemplateInfos > T_UrbanTemplateInfos_Vector;

//*****************************************************************************

public:
             ADN_Urban_Data();
    virtual ~ADN_Urban_Data();

    virtual void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    virtual void Save() const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void ReadArchive( xml::xistream& input );
    virtual bool FixConsistency();
    virtual void WriteArchive( xml::xostream& output ) const;
    void ReadTemplates( xml::xistream& input );
    void WriteTemplates( xml::xostream& output ) const;

    T_UrbanMaterialInfos_Vector& GetMaterialsInfos();
    UrbanMaterialInfos* FindMaterial( const std::string& strName );
    T_RoofShapeInfos_Vector& GetRoofShapesInfos();
    RoofShapeInfos* FindRoofShape( const std::string& strName );
    T_AccommodationInfos_Vector& GetAccommodationsInfos();
    AccommodationInfos* FindAccommodation( const std::string& strName );
    T_InfrastructureInfos_Vector& GetInfrastructuresInfos();
    InfrastructureInfos* FindInfrastructure( const std::string& strName );
    T_UrbanTemplateInfos_Vector& GetTemplatesInfos();

    QStringList GetUrbanTemplateThatUse( UrbanMaterialInfos& infos );
    QStringList GetUrbanTemplateThatUse( RoofShapeInfos& infos );
    QStringList GetUrbanTemplateThatUse( AccommodationInfos& infos );

    virtual bool ApplyOnTranslations( const std::function< bool( kernel::LocalizedString& ) >& functor ) const;
    virtual bool ApplyOnContexts( const std::function< bool( kernel::Context& ) >& functor ) const;
    const boost::shared_ptr< kernel::Context >& GetTemplateContext( const std::string& context );

private:
    //! @name Helpers
    //@{
    void ReadMaterial  ( xml::xistream& input );
    void ReadMaterials ( xml::xistream& input );
    void WriteMaterials( xml::xostream& output ) const;

    void ReadRoofShape  ( xml::xistream& input );
    void ReadRoofShapes ( xml::xistream& input );
    void WriteRoofShapes( xml::xostream& output ) const;

    void ReadAccommodation  ( xml::xistream& input );
    void ReadAccommodations ( xml::xistream& input );
    void WriteAccommodations( xml::xostream& output ) const;

    void ReadInfrastructure  ( xml::xistream& input );
    void ReadInfrastructures ( xml::xistream& input );
    void WriteInfrastructures( xml::xostream& output ) const;

    void ReadTemplate( xml::xistream& input );
    //@}

private:
    //! @name Member Data
    //@{
    T_UrbanMaterialInfos_Vector vMaterials_;
    T_RoofShapeInfos_Vector vRoofShapes_;
    T_AccommodationInfos_Vector vAccommodations_;
    T_InfrastructureInfos_Vector vInfrastructures_;
    T_UrbanTemplateInfos_Vector vTemplates_;
    std::unique_ptr< kernel::XmlTranslations > templateTranslations_;
    //@}

public:
    //! @name Member Data
    //@{
    ADN_Type_Double defaultNominalCapacity_;
    ADN_Type_Double defaultMaxCapacity_;
    //@}
};

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetMaterialsInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanMaterialInfos_Vector& ADN_Urban_Data::GetMaterialsInfos()
{
    return vMaterials_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos::operator==
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
inline
bool ADN_Urban_Data::UrbanMaterialInfos::operator==( const std::string& str )
{
    return strName_ == str;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindMaterial
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanMaterialInfos* ADN_Urban_Data::FindMaterial( const std::string& strName )
{
    for( auto it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetRoofShapesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_RoofShapeInfos_Vector& ADN_Urban_Data::GetRoofShapesInfos()
{
    return vRoofShapes_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindRoofShape
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::RoofShapeInfos* ADN_Urban_Data::FindRoofShape( const std::string& strName )
{
    for( auto it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetAccommodationsInfos
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_AccommodationInfos_Vector& ADN_Urban_Data::GetAccommodationsInfos()
{
    return vAccommodations_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos::operator==
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
bool ADN_Urban_Data::AccommodationInfos::operator==( const std::string& str )
{
    return strName_ == str;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindAccommodation
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::AccommodationInfos* ADN_Urban_Data::FindAccommodation( const std::string& strName )
{
    for( auto it = vAccommodations_.begin(); it != vAccommodations_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetInfrastructuresInfos
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_InfrastructureInfos_Vector& ADN_Urban_Data::GetInfrastructuresInfos()
{
    return vInfrastructures_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos::operator==
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
bool ADN_Urban_Data::InfrastructureInfos::operator==( const std::string& str )
{
    return strName_ == str;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::operator==
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
inline
bool ADN_Urban_Data::RoofShapeInfos::operator==( const std::string& str )
{
    return strName_.GetData() == str;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindInfrastructure
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::InfrastructureInfos* ADN_Urban_Data::FindInfrastructure( const std::string& strName )
{
    for( auto it = vInfrastructures_.begin(); it != vInfrastructures_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetTemplatesInfos
// Created: LGY 2011-09-20
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanTemplateInfos_Vector& ADN_Urban_Data::GetTemplatesInfos()
{
    return vTemplates_;
}

#endif // __ADN_Urban_Data_h_
