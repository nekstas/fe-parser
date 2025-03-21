#include "expressions_info_factory.h"

ast::ExpressionsInfo fe::ExpressionsInfoFactory::Create() const {
    ast::ExpressionsInfo info;

    info.AddInfo("+", {ast::ExpressionsInfo::Associativity::BOTH, 1});
    info.AddInfo("-", {ast::ExpressionsInfo::Associativity::LEFT, 1});

    info.AddInfo("*", {ast::ExpressionsInfo::Associativity::BOTH, 2});
    info.AddInfo("/", {ast::ExpressionsInfo::Associativity::LEFT, 2});

    info.AddInfo("^", {ast::ExpressionsInfo::Associativity::RIGHT, 3});

    return info;
}
