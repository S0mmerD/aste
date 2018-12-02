#pragma once
#include <memory>
#include <vector>
#include <metis.h>

#include "mesh.hpp"

std::vector<idx_t> partition(std::shared_ptr<Mesh> mesh, idx_t numberParts);
