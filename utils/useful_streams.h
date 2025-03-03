#pragma once
#include <string>

#include "../core/tokenizer/tokens/abstract_token.h"
#include "container_stream.hpp"

using CodeStream = ContainerStream<std::string>;
using TokensStream = ContainerStream<Token>;