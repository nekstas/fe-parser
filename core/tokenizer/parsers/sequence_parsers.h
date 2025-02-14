#pragma once
#include "../tokens/space_token.h"
#include "../tokens/operator_token.h"
#include "sequence_parser.hpp"

template <OperatorType type>
using OperatorParser = SequenceParser<OperatorToken, type>;

template <IndentType type>
using SpaceParser = SequenceParser<SpaceToken, type>;
