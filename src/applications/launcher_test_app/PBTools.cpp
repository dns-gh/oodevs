// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_test_app_pch.h"
#include "PBTools.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <google/protobuf/message.h>
#pragma warning( push )
#pragma warning( disable: 4244 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

#include "protocol/LauncherSenders.h"

namespace
{
    struct XmlFieldOutput
    {
        virtual void operator()(std::ostream&, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor&) = 0;
        virtual void operator()(std::ostream&, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor&, int) = 0;
    };

    template <typename T>
    struct SimpleFieldOutput : XmlFieldOutput
    {
        typedef T FieldCppType;
        typedef FieldCppType (google::protobuf::Reflection::*SimpleGetterType)(const google::protobuf::Message&, const google::protobuf::FieldDescriptor*) const;
        typedef FieldCppType (google::protobuf::Reflection::*RepeatedGetterType)(const google::protobuf::Message&, const google::protobuf::FieldDescriptor*, int) const;

        SimpleFieldOutput(SimpleGetterType simple_getter, RepeatedGetterType repeated_getter)
            : simple_getter_ ( simple_getter )
            , repeated_getter_( repeated_getter )
        {
            // NOTHING
        }
        void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "' value='" << (pReflection->*simple_getter_)(msg, &field) << "'/>" << std::endl;
        }
        void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "' value='" << (pReflection->*repeated_getter_)(msg, &field, index) << "'/>" << std::endl;
        }
        SimpleGetterType simple_getter_;
        RepeatedGetterType repeated_getter_;
    };

    struct EnumFieldOutput : XmlFieldOutput
    {
        void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "' value='" << pReflection->GetEnum(msg, &field)->name() << "'/>" << std::endl;
        }
        void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "' value='" << pReflection->GetRepeatedEnum(msg, &field, index)->name() << "'/>" << std::endl;
        }
    };

    struct MessageFieldOutput : XmlFieldOutput
    {
        void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "'>" << std::endl;
            launcher_test_app::PBTools::ToXML(os, pReflection->GetMessage( msg, &field ) );
            os << "</field>" << std::endl;
        }void operator()(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            os << "<field name='" << field.name() << "'>" << std::endl;
            launcher_test_app::PBTools::ToXML(os, pReflection->GetRepeatedMessage( msg, &field, index ) );
            os << "</field>" << std::endl;
        }
    };

    typedef std::auto_ptr< XmlFieldOutput > FieldOutputConverterPtr;

    template <typename T>
    FieldOutputConverterPtr makeFieldOutput(T (google::protobuf::Reflection::*simple_getter)(const google::protobuf::Message&, const google::protobuf::FieldDescriptor*) const,
            T (google::protobuf::Reflection::*repeated_getter)(const google::protobuf::Message&, const google::protobuf::FieldDescriptor*, int) const)
    {
        return FieldOutputConverterPtr( new SimpleFieldOutput<T>( simple_getter, repeated_getter ) );
    }

    FieldOutputConverterPtr outputConverters[google::protobuf::FieldDescriptor::MAX_CPPTYPE+1] =
        {
            FieldOutputConverterPtr( 0 ),
            makeFieldOutput( &google::protobuf::Reflection::GetInt32, &google::protobuf::Reflection::GetRepeatedInt32 ), // CPPTYPE_INT32
            makeFieldOutput( &google::protobuf::Reflection::GetInt64, &google::protobuf::Reflection::GetRepeatedInt64 ), // CPPTYPE_INT64
            makeFieldOutput( &google::protobuf::Reflection::GetUInt32, &google::protobuf::Reflection::GetRepeatedUInt32 ), // CPPTYPE_INT32
            makeFieldOutput( &google::protobuf::Reflection::GetUInt64, &google::protobuf::Reflection::GetRepeatedUInt64 ), // CPPTYPE_UINT64
            makeFieldOutput( &google::protobuf::Reflection::GetDouble, &google::protobuf::Reflection::GetRepeatedDouble ), // CPPTYPE_DOUBLE
            makeFieldOutput( &google::protobuf::Reflection::GetFloat, &google::protobuf::Reflection::GetRepeatedFloat ), // CPPTYPE_FLOAT
            makeFieldOutput( &google::protobuf::Reflection::GetBool, &google::protobuf::Reflection::GetRepeatedBool ), // CPPTYPE_BOOL
            FieldOutputConverterPtr( new EnumFieldOutput() ), // CPPTYPE_ENUM
            makeFieldOutput(&google::protobuf::Reflection::GetString, &google::protobuf::Reflection::GetRepeatedString ), // CPPTYPE_STRING
            FieldOutputConverterPtr( new MessageFieldOutput() ) // CPPTYPE_MESSAGE
        };

    struct XmlFieldSetter
    {
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor&) = 0;
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor&, int) = 0;
    };
    template <typename T>
    struct SimpleFieldSetter : XmlFieldSetter
    {
        typedef T FieldCppType;
        typedef void (google::protobuf::Reflection::*SimpleSetterType)(google::protobuf::Message*, const google::protobuf::FieldDescriptor*, FieldCppType value) const;
        typedef void (google::protobuf::Reflection::*RepeatedSetterType)(google::protobuf::Message*, const google::protobuf::FieldDescriptor*, int, FieldCppType value) const;

        SimpleFieldSetter(SimpleSetterType simple_setter, RepeatedSetterType repeated_setter)
            : simple_setter_( simple_setter )
            , repeated_setter_( repeated_setter )
        {
            // NOTHING
        }
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            FieldCppType value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            (pReflection->*simple_setter_)(&msg, &field, value);
        }
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            FieldCppType value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            (pReflection->*repeated_setter_)(&msg, &field, index, value);
        }
    private:
        SimpleSetterType simple_setter_;
        RepeatedSetterType repeated_setter_;
    };
    struct StringFieldSetter : XmlFieldSetter
    {
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            std::string value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            pReflection->SetString( &msg, &field, value );
        }
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            std::string value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            pReflection->SetRepeatedString( &msg, &field, index, value );
        }
    };
    struct EnumFieldSetter : XmlFieldSetter
    {
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            std::string value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            const google::protobuf::EnumDescriptor* enumDesc = field.enum_type();
            if( !enumDesc )
                return; // throw ?
            const google::protobuf::EnumValueDescriptor* enumValue = enumDesc->FindValueByName( value );
            if( !enumValue )
            {
                std::cerr << "Invalid enum value " << value << " for field " << msg.GetTypeName() << "::" << field.name() << std::endl;
                return;
            }
            pReflection->SetEnum( &msg, &field, enumValue );
        }
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            std::string value;
            xis >> xml::attribute("value", value);
            const google::protobuf::Reflection* pReflection = msg.GetReflection();
            const google::protobuf::EnumDescriptor* enumDesc = field.enum_type();
            if( !enumDesc )
                return; // throw ?
            const google::protobuf::EnumValueDescriptor* enumValue = enumDesc->FindValueByName( value );
            if( enumValue )
                std::cerr << "Invalid enum value " << value << " for field " << msg.GetTypeName() << "::" << field.name() << std::endl;
            pReflection->SetRepeatedEnum( &msg, &field, index, enumValue );
        }
    };

    struct MessageFieldSetter : XmlFieldSetter
    {
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field)
        {
            std::string messageType;
            xis >> xml::start("message")
                    >> xml::attribute("type", messageType );
            if( messageType != field.message_type()->name() )
            {
                std::cerr << "Wrong type for field " << field.name() << std::endl;
            }
            else
            {
                const google::protobuf::Reflection* pReflection = msg.GetReflection();
                launcher_test_app::PBTools::FromXML( xis, *pReflection->MutableMessage( &msg, &field ) );
            }
            xis >> xml::end;
        }
        virtual void operator()(xml::xistream& xis, google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& field, int index)
        {
            std::string messageType;
            xis >> xml::start("message")
                    >> xml::attribute("type", messageType );
            if( messageType != field.message_type()->name() )
            {
                std::cerr << "Wrong type for field " << field.name() << std::endl;
            }
            else
            {
                const google::protobuf::Reflection* pReflection = msg.GetReflection();
                launcher_test_app::PBTools::FromXML( xis, *pReflection->MutableRepeatedMessage( &msg, &field, index ) );
            }
            xis >> xml::end;
        }
    };
    typedef std::auto_ptr<XmlFieldSetter> FieldSetterPtr;
    template <typename T>
    FieldSetterPtr makeFieldSetter(void (google::protobuf::Reflection::*simple_getter)(google::protobuf::Message*, const google::protobuf::FieldDescriptor*, T) const,
           void (google::protobuf::Reflection::*repeated_getter)(google::protobuf::Message*, const google::protobuf::FieldDescriptor*, int, T) const)
    {
        return FieldSetterPtr( new SimpleFieldSetter<T>( simple_getter, repeated_getter ) );
    }

    FieldSetterPtr inputConverters[google::protobuf::FieldDescriptor::MAX_CPPTYPE+1] =
        {
            FieldSetterPtr( 0 ),
            makeFieldSetter( &google::protobuf::Reflection::SetInt32, &google::protobuf::Reflection::SetRepeatedInt32 ), // CPPTYPE_INT32
            makeFieldSetter( &google::protobuf::Reflection::SetInt64, &google::protobuf::Reflection::SetRepeatedInt64 ), // CPPTYPE_INT64
            makeFieldSetter( &google::protobuf::Reflection::SetUInt32, &google::protobuf::Reflection::SetRepeatedUInt32 ), // CPPTYPE_INT32
            makeFieldSetter( &google::protobuf::Reflection::SetUInt64, &google::protobuf::Reflection::SetRepeatedUInt64 ), // CPPTYPE_UINT64
            makeFieldSetter( &google::protobuf::Reflection::SetDouble, &google::protobuf::Reflection::SetRepeatedDouble ), // CPPTYPE_DOUBLE
            makeFieldSetter( &google::protobuf::Reflection::SetFloat, &google::protobuf::Reflection::SetRepeatedFloat ), // CPPTYPE_FLOAT
            makeFieldSetter( &google::protobuf::Reflection::SetBool, &google::protobuf::Reflection::SetRepeatedBool ), // CPPTYPE_BOOL
            FieldSetterPtr( new EnumFieldSetter() ), // CPPTYPE_ENUM
            FieldSetterPtr( new StringFieldSetter() ), // CPPTYPE_STRING
            FieldSetterPtr( new MessageFieldSetter() ) // CPPTYPE_MESSAGE
        };
}
namespace launcher_test_app
{

// -----------------------------------------------------------------------------
// Name: PBTools::ToXML
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void PBTools::ToXML(std::ostream& os, const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor& desc,  const google::protobuf::Reflection& refl)
{
    if( desc.is_repeated() )
    {
        int size = refl.FieldSize(msg, &desc);
        for(int i=0; i<size; ++i)
        {
            (*outputConverters[(int)desc.cpp_type()])(os, msg, desc, i);
        }
    }
    else
        (*outputConverters[(int)desc.cpp_type()])(os, msg, desc);
}
// -----------------------------------------------------------------------------
// Name: PBTools::ToXML
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void PBTools::ToXML(std::ostream& os, const google::protobuf::Message& msg)
{
    const google::protobuf::Descriptor* pDescriptor = msg.GetDescriptor();
    const google::protobuf::Reflection* pReflection = msg.GetReflection();
    os  << "<message type='"<< pDescriptor->name() << "'>" << std::endl;
    std::vector< const google::protobuf::FieldDescriptor* > validFields;
    pReflection->ListFields(msg, &validFields);
    for(std::size_t i=0; i<validFields.size(); ++i)
    {
        ToXML(os, msg, *validFields[i], *pReflection );
    }
    os  << "</message>" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: PBTools::FromXML
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void PBTools::FromXML(xml::xistream& xis, google::protobuf::Message& msg)
{
    std::map<std::string, int> indexMap;
    xis >> xml::list("field", boost::bind(&PBTools::FieldFromXML, _1, boost::ref( msg ), boost::ref( indexMap ) ) );
}

// -----------------------------------------------------------------------------
// Name: PBTools::FieldFromXML
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void PBTools::FieldFromXML(xml::xistream& xis, google::protobuf::Message& msg, std::map<std::string, int>& indexMap)
{
    const google::protobuf::Descriptor* pDescriptor = msg.GetDescriptor();
    const google::protobuf::Reflection* pReflection = msg.GetReflection();
    std::string fieldName;
    xis >> xml::attribute("name", fieldName);
    const google::protobuf::FieldDescriptor* fieldDesc = pDescriptor->FindFieldByLowercaseName(fieldName);
    if( !fieldDesc )
    {
        std::cerr << "Field " << fieldName << " not found for message " << pDescriptor->name() << std::endl;
        return;
    }
    XmlFieldSetter* setter = inputConverters[ (int)fieldDesc->cpp_type() ].get();
    if( !setter )
    {
        std::cerr << "No setter for field " << fieldName << " in message " << pDescriptor->name() << std::endl;
        return;
    }
    if( fieldDesc->is_repeated() )
    {
        std::map<std::string, int>::iterator indexIt = indexMap.find(fieldName);
        if( indexMap.end() == indexIt )
            indexMap[fieldName] = 0;
        int currentIndex = indexMap[fieldName];
        (*setter)(xis, msg, *fieldDesc, currentIndex);
        ++indexMap[fieldName];
    }
    else
    {
        (*setter)(xis, msg, *fieldDesc);
    }
}
}
