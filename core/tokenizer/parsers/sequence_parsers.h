#pragma once
#include "../tokens/indent_token.h"
#include "../tokens/operator_token.h"
#include "sequence_parser.hpp"

template <OperatorType type>
using OperatorParser = SequenceParser<OperatorToken, type>;

template <IndentType type>
using IndentParser = SequenceParser<IndentToken, type>;
