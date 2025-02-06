

#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "../Matrix/Structure.h"


class Parser {
public:
    static Graph loadGraphFromXML(const std::string& file_path);
};




#endif //PARSER_H
