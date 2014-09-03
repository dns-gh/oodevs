// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MissionGenObjectTypes_Infos_h_
#define __ADN_MissionGenObjectTypes_Infos_h_

#include "ADN_CrossedRef.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Type_VectorFixed_ABC.h"

namespace helpers
{
    // $$$$ ABR 2013-01-08: Rename this class, not only about gen objects, that can be about object knowledge too

// =============================================================================
/** @class  ADN_MissionGenObjectTypes_Infos
    @brief  ADN_MissionGenObjectTypes_Infos
*/
// Created: LGY 2012-04-18
// =============================================================================
class ADN_MissionGenObjectTypes_Infos : public ADN_CrossedRef< ADN_Objects_Data_ObjectInfos >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_MissionGenObjectTypes_Infos( ADN_Objects_Data_ObjectInfos* ptr );
             ADN_MissionGenObjectTypes_Infos();
    virtual ~ADN_MissionGenObjectTypes_Infos();
    //@}

    //! @name Operations
    //@{
    virtual void WriteArchive( xml::xostream& ) const;
    ADN_MissionGenObjectTypes_Infos* CreateCopy();
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_Bool isAllowed_;
    //@}

public:

    typedef ADN_Objects_Data_ObjectInfos T_Item;

    class CmpRef : public std::unary_function< ADN_MissionGenObjectTypes_Infos*, bool >
    {
    public:
        CmpRef( ADN_Objects_Data_ObjectInfos* val ) : val_( val ) {}

        bool operator()( ADN_MissionGenObjectTypes_Infos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() == val_;
        }
    private:
        ADN_Objects_Data_ObjectInfos* val_;
    };
};

class T_MissionGenObjectTypes_Infos_Vector : public ADN_Type_VectorFixed_ABC< ADN_MissionGenObjectTypes_Infos >
{
public:
             T_MissionGenObjectTypes_Infos_Vector();
    virtual ~T_MissionGenObjectTypes_Infos_Vector();

    template< class U >
    explicit T_MissionGenObjectTypes_Infos_Vector( const ADN_Type_Vector_ABC< U >& v )
        : ADN_Type_VectorFixed_ABC< ADN_MissionGenObjectTypes_Infos >( v )
    {
        // NOTHING
    }

    bool IsAllChecked() const;

protected:
    virtual void AutoCreatePrivate( void* ptr );

};

} // end namespace helpers

#endif // __ADN_MissionGenObjectTypes_Infos_h_
