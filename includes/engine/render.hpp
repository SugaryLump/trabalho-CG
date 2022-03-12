#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "engine/config.hpp"

namespace Render {

extern GLuint buffer;
extern size_t _n_indices;
extern std::vector<unsigned int> indices;

int render(int argc, char **argv, Config config);

}  // namespace Render

#endif  // RENDER_HPP_
