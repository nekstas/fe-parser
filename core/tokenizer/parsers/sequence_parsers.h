#pragma once
#include "../tokens/space_token.h"
#include "../tokens/operator_token.h"
#include "sequence_parser.hpp"

template <IndentType type>
using SpaceParser = SequenceParser<SpaceToken, type>;
