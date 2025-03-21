#include "format_visitor.h"

#include "../../../../utils/utils.h"
#include "../../factories/expressions_info_factory.h"
#include "../expressions/number_expression.h"
#include "../expressions/variable_expression.h"

ast::FormatVisitor::FormatVisitor() : expressions_info_(fe::ExpressionsInfoFactory().Create()) {
}

void ast::FormatVisitor::Visit(const ast::NumberExpression& number) {
    result_ = number.GetValue();
}

void ast::FormatVisitor::Visit(const ast::VariableExpression& variable) {
    result_ = variable.GetName();
}

void ast::FormatVisitor::Visit(const ast::CallExpression& call_expression) {
    auto callee = call_expression.GetCallee();
    callee->Accept(*this);

    FormatStream new_result;

    if (!Is<VariableExpression>(callee)) {
        result_ = "(" + result_ + ")";
    }
    new_result << result_;

    std::vector<std::string> args;
    for (auto arg : call_expression.GetArgs()) {
        arg->Accept(*this);
        args.push_back(result_);
    }

    new_result << "(" << utils::Join(args, ", ") << ")";
    result_ = new_result;
}

void ast::FormatVisitor::ProcessPartOfBinaryExpression(
    FormatStream& new_result, std::shared_ptr<Expression> expression,
    const ast::ExpressionsInfo::Info& info, ExpressionsInfo::Associativity place
) {
    bool without_braces = Is<AtomExpression>(expression);

    if (Is<BinaryExpression>(expression)) {
        auto binary_expr = Cast<BinaryExpression>(expression);
        auto another_info = expressions_info_.GetInfo(binary_expr->GetCode());

        without_braces = another_info.priority > info.priority ||
                         (another_info.priority == info.priority &&
                          ast::ExpressionsInfo::IsCompatible(info.associativity, place));
    }

    expression->Accept(*this);
    if (without_braces) {
        new_result << result_;
    } else {
        new_result << "(" << result_ << ")";
    }
}

void ast::FormatVisitor::Visit(const ast::BinaryExpression& binary_expression) {
    auto code = binary_expression.GetCode();
    auto first = binary_expression.GetFirst();
    auto second = binary_expression.GetSecond();
    auto info = expressions_info_.GetInfo(code);

    FormatStream new_result;

    ProcessPartOfBinaryExpression(new_result, first, info, ExpressionsInfo::Associativity::LEFT);
    new_result << " " << binary_expression.GetCode() << " ";
    ProcessPartOfBinaryExpression(new_result, second, info, ExpressionsInfo::Associativity::RIGHT);

    result_ = new_result;
}
