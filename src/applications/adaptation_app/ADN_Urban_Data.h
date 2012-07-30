//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Categories_Data.h $
// $Author: Ape $
// $Modtime: 13/04/05 15:10 $
// $Revision: 8 $
// $Workfile: ADN_Categories_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Urban_Data_h_
#define __ADN_Urban_Data_h_

#include "ADN_AttritionInfos.h"
#include "ADN_CapacityInfos.h"
#include "ADN_Categories_Data.h"
#include "ADN_Data_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Tools.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Types.h"
#include "ADN_Symbols_Data.h"
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class ADN_Objects_Data;
class ADN_TypeCapacity_Infos;

//*****************************************************************************
// Created: JDY 03-08-27
//*****************************************************************************
class ADN_Urban_Data : public ADN_Data_ABC
{

//*****************************************************************************
public:
    class AccommodationInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {
    public:
                 AccommodationInfos();
        explicit AccommodationInfos( xml::xistream& input, const std::string& role );
        virtual ~AccommodationInfos();

        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        bool operator==( const std::string& str );
        void WriteAccommodation( xml::xostream& output );
        AccommodationInfos* CreateCopy();

    public:
        //! @name Member Data
        //@{
        ADN_Type_String strName_;
        ADN_Type_Double nominalCapacity_;
        ADN_Type_Double maxCapacity_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< AccommodationInfos >     T_AccommodationInfos_Vector;
    typedef T_AccommodationInfos_Vector::iterator        IT_AccommodationInfos_Vector;
    typedef T_AccommodationInfos_Vector::const_iterator CIT_AccommodationInfos_Vector;

//*****************************************************************************
public:
    class InfrastructureInfos : public ADN_Ref_ABC
                              , public ADN_DataTreeNode_ABC
    {
    public:
                 InfrastructureInfos();
        explicit InfrastructureInfos( xml::xistream& input );
        virtual ~InfrastructureInfos();

        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        bool operator==( const std::string& str );
        void WriteInfrastructure( xml::xostream& output );
        InfrastructureInfos* CreateCopy();

        typedef std::map< std::string, boost::shared_ptr< helpers::ADN_TypeCapacity_Infos > > T_CapacityMap;
        typedef T_CapacityMap::iterator IT_CapacityMap;
        typedef T_CapacityMap::const_iterator CIT_CapacityMap;

    private:
        void ReadCapacityArchive( const std::string& type, xml::xistream& xis );

    public:
        //! @name Member Data
        //@{
        ADN_Type_String strName_;
        ADN_TypePtr_InVector_ABC< ADN_Symbols_Data::SymbolsInfra > pSymbol_;
        T_CapacityMap capacities_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< InfrastructureInfos >     T_InfrastructureInfos_Vector;
    typedef T_InfrastructureInfos_Vector::iterator        IT_InfrastructureInfos_Vector;
    typedef T_InfrastructureInfos_Vector::const_iterator CIT_InfrastructureInfos_Vector;

//*****************************************************************************
public:
    class UrbanMaterialInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {
    public:
                 UrbanMaterialInfos();
        explicit UrbanMaterialInfos( xml::xistream& input );
        virtual ~UrbanMaterialInfos();

        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        bool operator==( const std::string& str );
        void WriteMaterial( xml::xostream& output );
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
        ADN_Type_String strName_;
        helpers::T_AttritionInfos_Vector vAttritionInfos_;
        //@}
    };

    typedef ADN_Type_Vector_ABC< UrbanMaterialInfos >     T_UrbanMaterialInfos_Vector;
    typedef T_UrbanMaterialInfos_Vector::iterator        IT_UrbanMaterialInfos_Vector;
    typedef T_UrbanMaterialInfos_Vector::const_iterator CIT_UrbanMaterialInfos_Vector;

//*****************************************************************************

public:
    class RoofShapeInfos : public ADN_Ref_ABC
                        , public ADN_DataTreeNode_ABC
    {
    public:
                 RoofShapeInfos();
        explicit RoofShapeInfos( xml::xistream& input );
        virtual ~RoofShapeInfos();

        bool operator==( const std::string& str );
        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        void WriteRoofShape( xml::xostream& output );
        RoofShapeInfos* CreateCopy() { return new RoofShapeInfos(); }

    public:
        ADN_Type_String strName_;
    };

    typedef ADN_Type_Vector_ABC< RoofShapeInfos >     T_RoofShapeInfos_Vector;
    typedef T_RoofShapeInfos_Vector::iterator        IT_RoofShapeInfos_Vector;
    typedef T_RoofShapeInfos_Vector::const_iterator CIT_RoofShapeInfos_Vector;

//*****************************************************************************

public:
    class UrbanInfos : public ADN_Ref_ABC
                     , public ADN_DataTreeNode_ABC
    {
    public:
        UrbanInfos() {}
        UrbanInfos* CreateCopy() { return new UrbanInfos(); }
        std::string GetItemName() { return strName_.GetData(); }

    public:
        ADN_Type_String strName_;
    };
    typedef ADN_Type_Vector_ABC< UrbanInfos > T_UrbanInfos_Vector;
    typedef T_UrbanInfos_Vector::iterator    IT_UrbanInfos_Vector;

//*****************************************************************************

    class UsageTemplateInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {
    public:
                 UsageTemplateInfos();
        explicit UsageTemplateInfos( xml::xistream& input );
        explicit UsageTemplateInfos( ADN_Urban_Data::AccommodationInfos& accomodation, ADN_Type_Int proportion );

        virtual ~UsageTemplateInfos();

        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        void Write( xml::xostream& output );
        UsageTemplateInfos* CreateCopy();

    public:
        ADN_TypePtr_InVector_ABC< AccommodationInfos > accommodation_;
        ADN_Type_Int proportion_;
    };

//*****************************************************************************

public:
    class T_UsageTemplateInfosVector : public ADN_Type_Vector_ABC< UsageTemplateInfos >
    {
    public:
        explicit T_UsageTemplateInfosVector( ADN_Ref_ABC* parent, bool bAutoRef = true, const char* szName = 0 );
        virtual ~T_UsageTemplateInfosVector();

    protected:
        virtual bool AddItemPrivate( void* pObj, bool bCreateCommand = false );

    private:
        ADN_Ref_ABC* parent_;
    };

    typedef T_UsageTemplateInfosVector::iterator        IT_UsageTemplateInfosVector;
    typedef T_UsageTemplateInfosVector::const_iterator CIT_UsageTemplateInfosVector;
//*****************************************************************************

public:
    class UrbanTemplateInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {
    public:
                 UrbanTemplateInfos();
        explicit UrbanTemplateInfos( xml::xistream& input );
        virtual ~UrbanTemplateInfos();

        virtual std::string GetNodeName();
        virtual std::string GetItemName();
        void Write( xml::xostream& output );
        UrbanTemplateInfos* CreateCopy();

    private:
        void ReadUsage( xml::xistream& xis );

    public:
        //! @name Member Data
        //@{
        ADN_Type_String strName_;
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

    typedef ADN_Type_Vector_ABC< UrbanTemplateInfos >     T_UrbanTemplateInfos_Vector;
    typedef T_UrbanTemplateInfos_Vector::iterator        IT_UrbanTemplateInfos_Vector;
    typedef T_UrbanTemplateInfos_Vector::const_iterator CIT_UrbanTemplateInfos_Vector;

//*****************************************************************************

public:
             ADN_Urban_Data();
    virtual ~ADN_Urban_Data();

    virtual void FilesNeeded( T_StringList& l ) const;
    virtual void Reset();
    virtual void Save();
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );
    void ReadTemplates( xml::xistream& input );
    void WriteTemplates( xml::xostream& output );

    T_UrbanMaterialInfos_Vector& GetMaterialsInfos();
    UrbanMaterialInfos* FindMaterial( const std::string& strName );
    T_UrbanInfos_Vector& GetFacadesInfos();
    UrbanInfos* FindFacade( const std::string& strName );
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

private:
    //! @name Helpers
    //@{
    void ReadMaterial  ( xml::xistream& input );
    void ReadMaterials ( xml::xistream& input );
    void WriteMaterials( xml::xostream& output ) const;

    void ReadFacade  ( xml::xistream& input );
    void ReadFacades ( xml::xistream& input );
    void WriteFacades( xml::xostream& output ) const;

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
    T_UrbanInfos_Vector vFacades_;
    T_AccommodationInfos_Vector vAccommodations_;
    T_InfrastructureInfos_Vector vInfrastructures_;
    T_UrbanTemplateInfos_Vector vTemplates_;
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
    return ADN_Tools::CaselessCompare( strName_.GetData(), str );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindMaterial
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanMaterialInfos* ADN_Urban_Data::FindMaterial( const std::string& strName )
{
    for( CIT_UrbanMaterialInfos_Vector it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
        if( **it == strName )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetFacadesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanInfos_Vector& ADN_Urban_Data::GetFacadesInfos()
{
    return vFacades_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindFacade
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanInfos* ADN_Urban_Data::FindFacade( const std::string& strName )
{
    for( IT_UrbanInfos_Vector it = vFacades_.begin(); it != vFacades_.end(); ++it )
        if( ADN_Tools::CaselessCompare( ( *it )->strName_.GetData(), strName ) )
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
    for( CIT_RoofShapeInfos_Vector it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
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
    return ADN_Tools::CaselessCompare( strName_.GetData(), str );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindAccommodation
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::AccommodationInfos* ADN_Urban_Data::FindAccommodation( const std::string& strName )
{
    for( IT_AccommodationInfos_Vector it = vAccommodations_.begin(); it != vAccommodations_.end(); ++it )
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
    return ADN_Tools::CaselessCompare( strName_.GetData(), str );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::operator==
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
inline
bool ADN_Urban_Data::RoofShapeInfos::operator==( const std::string& str )
{
    return ADN_Tools::CaselessCompare( strName_.GetData(), str );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindInfrastructure
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::InfrastructureInfos* ADN_Urban_Data::FindInfrastructure( const std::string& strName )
{
    for( IT_InfrastructureInfos_Vector it = vInfrastructures_.begin(); it != vInfrastructures_.end(); ++it )
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
