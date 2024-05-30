#include "IntermediateRepresentation/EnumInfo.hpp"
#include "utils.hpp"

/************************************/
/*       EnumInfo Definitions      */
/************************************/

EnumInfo::EnumInfo() {}
EnumInfo::EnumInfo(std::string n) : name(n) {}

ICGTemplateEngine::Dictionary EnumInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;

    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }

    dictionary["EnumName"] = fullname.str();
    dictionary["EnumName_mangled"] = ICGUtils::makeVarname(fullname.str());

    return dictionary;
}

// Push the fields
ICGTemplateEngine::ListTokenItems EnumInfo::nextLevel() const {

    ICGTemplateEngine::ListTokenItems my_info_dictionary;
    std::string field_key = "fields";

    for (const auto& field : fields) {
        my_info_dictionary[field_key].push_back(&field);
    }

    return my_info_dictionary;
}

std::string EnumInfo::toString () const {
    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }


    std::stringstream ret;
    ret << "enum" << fullname.str();
    
    ret << " {\n";
    for (auto field : fields) {
        ret << "\t" << field.toString() << "\n";
    }

    ret << "};\n";
    return ret.str();
}

