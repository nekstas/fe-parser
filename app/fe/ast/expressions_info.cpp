#include "expressions_info.h"

void ast::ExpressionsInfo::AddInfo(const std::string& code, const Info& new_info) {
    if (info_.contains(code)) {
        throw AddExtraExpressionInfoError(code);
    }
    info_[code] = new_info;
}

ast::ExpressionsInfo::Info ast::ExpressionsInfo::GetInfo(const std::string& code) const {
    if (!info_.contains(code)) {
        throw ExpressionInfoDoesNotExistError(code);
    }
    return info_.at(code);
}
