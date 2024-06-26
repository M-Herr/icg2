#pragma once

#include <map>
#include <string>

// List format string: list_recursableName_tokenName
std::string io_src = std::string(R"(
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/SpecifiedCompositeType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"
#include "Type/EnumDataType.hpp"
#include "{{filename}}"

{{list_classes_class_type_decl}}

void populate_type_dictionary(DataTypeInator * dataTypeInator) {
    //
    // Register class types
    //
    {{list_classes_call_class_decl}}

    //
    // Register STL types
    //
    {{list_stls_stl_decl}}

    //
    // Register typedef statements
    //
    {{list_typedefs_typedef_decl}}

    //
    //Register Num Types
    //
    {{list_enums_enum_decl}}

    //
    // Validate!
    //
    dataTypeInator->validateDictionary();
}
)");

std::string call_class_decl = std::string(R"(dataTypeInator->addToDictionary("{{ClassName}}", new SpecifiedCompositeType<{{ClassName}}>);
)");

std::string class_type_decl = std::string(R"(
//
// ICG generated definition of the DataType for {{ClassName}}
//
template <>
class SpecifiedCompositeType<{{ClassName}}> : public CompositeDataType {

    public:
    SpecifiedCompositeType() : CompositeDataType("{{ClassName}}", sizeof({{ClassName}}), &construct_composite<{{ClassName}}>, &destruct_composite<{{ClassName}}>) {}

    template<typename Derived>
    static MemberMap applyMembersToDerived () {
        using type_to_add = {{ClassName}};

        MemberMap derived_member_map = {
            {{list_fields_derived_field_decl}}
        };

        {{list_bases_add_base_members_to_derived}}

        return derived_member_map;
    }    

    MemberMap& getMemberMap () {
        using type_to_add = {{ClassName}};

        static bool initialized = false;
        static MemberMap member_map = {
            {{list_fields_field_decl}}
        };

        if (!initialized) {
            {{list_bases_add_base_members_to_self}}

            initialized = true;
        }

        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<{{ClassName}}>*> (this))->getMemberMap();
    }
};
)");

std::string derived_field_decl = std::string (R"({"{{FieldName}}", StructMember("{{FieldName}}", "{{FieldType}}", offsetof(Derived, type_to_add::{{FieldName}}))},
)");

std::string field_decl = std::string(R"({"{{FieldName}}", StructMember("{{FieldName}}", "{{FieldType}}", offsetof(type_to_add, {{FieldName}}))},
)");

std::string stl_decl = std::string(R"(dataTypeInator->addToDictionary("{{STLName}}", new SpecifiedSequenceDataType<{{STLName}}>("{{STLName}}"));
)");

std::string add_enum_types = std::string(R"(
    
    Enumerator * {{EnumName}}_{{FieldName}} = new Enumerator("{{EnumName}}_{{FieldName}}", {{EnumName}}_enum_counter);
    {{EnumName}}_enum_counter++;
    {{EnumName}}_enum_vec.push_back({{EnumName}}_{{FieldName}});
)");

std::string enum_decl = std::string(R"(
    //Assuming that all enums start at 0 and increase by 1 for every field

    size_t {{EnumName}}_enum_counter = 0;
    std::vector<Enumerator*> {{EnumName}}_enum_vec;

    {{list_fields_add_enum_types}}

    EnumDataType * {{EnumName}}_EnumDataType = new EnumDataType(dataTypeInator->getEnumDictionary(), "{{EnumName}}", sizeof({{EnumName}}));

    for(size_t i = 0; i < {{EnumName}}_enum_vec.size(); i++) {
        {{EnumName}}_EnumDataType->addEnumerator({{EnumName}}_enum_vec[i]);
    }

    dataTypeInator->addToDictionary("{{EnumName}}", {{EnumName}}_EnumDataType);
)");

std::string typedef_decl = std::string(R"(dataTypeInator->addTypeDef("{{ExistingName}}", "{{AliasName}}");
)");

std::string add_base_members_to_self = std::string(R"(
auto derived_members = SpecifiedCompositeType<{{BaseClassName}}>::applyMembersToDerived<{{ClassName}}>();
member_map.insert(derived_members.begin(), derived_members.end());
)");

std::string add_base_members_to_derived = std::string(R"(
auto derived_members = SpecifiedCompositeType<{{BaseClassName}}>::applyMembersToDerived<Derived>();
derived_member_map.insert(derived_members.begin(), derived_members.end());
)");

std::map<std::string, std::string> template_dictionary {
    {"top", io_src},
    {"call_class_decl", call_class_decl},
    {"class_type_decl", class_type_decl},
    {"field_decl", field_decl},
    {"derived_field_decl", derived_field_decl},
    {"add_base_members_to_self", add_base_members_to_self},
    {"add_base_members_to_derived", add_base_members_to_derived},
    {"stl_decl", stl_decl},
    {"enum_decl", enum_decl},
    {"add_enum_types", add_enum_types},
    {"typedef_decl", typedef_decl}
};
