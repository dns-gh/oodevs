// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Objects_Data_ObjectInfos_h_
#define __ADN_Objects_Data_ObjectInfos_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ADN_CapacityInfos.h"
#include "ADN_Drawings_Data.h"

// =============================================================================
/** @class  ADN_Objects_Data_ObjectInfos
    @brief  ADN_Objects_Data_ObjectInfos Here to avoid circular include
*/
// Created: ABR 2011-12-06
// =============================================================================
class ADN_Objects_Data_ObjectInfos : public ADN_Ref_ABC
                                   , public ADN_DataTreeNode_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Objects_Data_ObjectInfos();
    explicit ADN_Objects_Data_ObjectInfos( const std::string& name );
    virtual ~ADN_Objects_Data_ObjectInfos();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    std::string GetItemName();
    ADN_Objects_Data_ObjectInfos* CreateCopy();
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadCapacityArchive( const std::string& type, xml::xistream& input );
    void InitializeCapacities();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< helpers::ADN_TypeCapacity_Infos > > T_CapacityMap;
    typedef T_CapacityMap::iterator                                                      IT_CapacityMap;
    typedef T_CapacityMap::const_iterator                                               CIT_CapacityMap;
    //@}

    //! @name Member data
    //@{
    ADN_Type_String strName_;
    ADN_Type_String strType_;
    ADN_Type_String geometries_;
    ADN_Type_String description_;
    ADN_Type_Double pointSize_;
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
    T_CapacityMap capacities_;
    //@}

    //! @name Static member data
    //@{
    static int VAL;
    //@}
    template< typename T > struct Enumerator
    {
        enum E { value = VAL++ };
    };
};

#endif // __ADN_Objects_Data_ObjectInfos_h_
