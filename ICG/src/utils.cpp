#include "utils.hpp"

namespace ICGUtils {


    // Shamefully stolen from https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
    // Do it in place
    void ReplaceAll_inplace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
    }

    /**
     * @brief Take a given typename and modify it so that it can be used in a variable name.
     * @note Types are often used as variable names in the code generated by ICG, so we need to
     *          create a version that follow variable name character rules and also won't collide with anything.
     *          Since these are in generated code, they don't have to be pretty, but they should be readable
     *          As a kindness to myself and future Trick developers
     * 
     * @param typename_to_modify 
     * @return std::string version of the type that can be used in a varname
     */
    std::string makeVarname(const std::string& typename_to_modify) {
        std::string result = typename_to_modify;

        ReplaceAll_inplace(result, "::", "_COLONS_");
        ReplaceAll_inplace(result, "<", "_LEFTANGLE_");
        ReplaceAll_inplace(result, ">", "_RIGHTANGLE_");

        return result;
    }



    std::string toStdString(const CXString& str) {
        const char * c_str =  clang_getCString(str);
        if (c_str != NULL) {
            std::string my_std_string = clang_getCString(str);
            clang_disposeString(str);
            return my_std_string;
        }

        return std::string("");
    }


    std::ostream& operator<<(std::ostream& stream, const CXString& str)
    {
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
    }

    std::string getCursorSpelling (CXCursor c) {
        return toStdString(clang_getCursorSpelling(c));
    }

    std::string getTypeSpelling (CXCursor c) {
        return toStdString(clang_getTypeSpelling(clang_getCursorType(c)));
    }

    std::string getKindSpelling (CXCursor c) {
        return toStdString(clang_getCursorKindSpelling(clang_getCursorKind(c)));
    }

    enum AccessLevel getAccessLevel (CXCursor c) {
        auto clang_access = clang_getCXXAccessSpecifier(c);

        enum AccessLevel level;
        switch (clang_access) {
            case CX_CXXPublic:
                level = AccessLevel::PUBLIC;
                break;
            case CX_CXXProtected:
                level = AccessLevel::PROTECTED;
                break;
            case CX_CXXPrivate:
                level = AccessLevel::PRIVATE;
                break;
            case CX_CXXInvalidAccessSpecifier:
                level = AccessLevel::INVALID;
                break;
            default:
                level = AccessLevel::NONE;
                break;
        }

        return level;
    }

    std::string makeIOHeaderName (std::string header_name) {
        return "io_" + header_name;
    }

}