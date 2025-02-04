#pragma once
#include "../tokens/indent_token.h"
#include "../tokens/operator_token.h"
#include "empty_parser.h"
#include "sequence_parser.hpp"

template <OperatorType type>
using OperatorParser = SequenceParser<NonEmptyParser, OperatorToken, type>;

template <IndentType type>
using IndentParser = SequenceParser<EmptyParser, IndentToken, type>;
