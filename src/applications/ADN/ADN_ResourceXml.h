//*****************************************************************************
//
// $Created: JDY 03-06-20 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ResourceXml.h $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_ResourceXml.h $
//
//*****************************************************************************

#ifndef __ADN_ResourceXml_h_
#define __ADN_ResourceXml_h_

#include "MT/MT_Tools/MT_Vector.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

typedef MT_XXmlInputArchive ADN_ResourceXml_ABC;

//*****************************************************************************
// Created: JDY 03-06-20
//*****************************************************************************
class ADN_ResourceXml
: public ADN_ResourceXml_ABC
{    
    MT_COPYNOTALLOWED( ADN_ResourceXml )

public:
    explicit ADN_ResourceXml(const std::string& data);
    explicit ADN_ResourceXml( int idres);
    virtual ~ADN_ResourceXml();
    
    void Initialize( int idres);

    template< class T >
    bool ReadField( const std::string& name, T& value )
    { 
        if (name.empty())
            return false;

        T_StringVector lfields=getStringVector(name);
        CIT_StringVector    it=lfields.begin();
        int i,j;
        for (i=0;i<(int)lfields.size()-1;++i,++it)
        {
            if (!Section(*it))
            {
                for (int j=0;j<i;++j)
                    EndSection();
                return false;
            }
                
        }

        ADN_ResourceXml_ABC::ReadField(*it,value);
        
        for (j=0;j<(int)lfields.size()-1;++j)
            EndSection();
        return true;
    }

private:
    T_StringVector getStringVector(const std::string& name);
};

#endif // __ADN_ResourceXml_h_