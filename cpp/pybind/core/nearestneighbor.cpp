// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/core/nns/NearestNeighbor.h"

#include "open3d/core/Dtype.h"
#include "pybind/core/core.h"
#include "pybind/docstring.h"
#include "pybind/open3d_pybind.h"
#include "pybind/pybind_utils.h"

namespace open3d {

void pybind_core_nn(py::module &m) {
    py::module m_nn = m.def_submodule("nns");

    // open3d.core.nns.NearestNeighbor
    static const std::unordered_map<std::string, std::string>
            map_nearest_neighbor_search_method_docs = {
                    {"query", "The input query tensor."},
                    {"radii", "Search multiple radii"},
                    {"radius", "Search fixed radius."},
                    {"max_nn",
                     "At maximum, ``max_nn`` neighbors will be searched."},
                    {"knn", "``knn`` neighbors will be searched."}};
    py::class_<core::nns::NearestNeighbor,
               std::shared_ptr<core::nns::NearestNeighbor>>
            nearestneighbor(
                    m_nn, "NearestNeighbor",
                    "NearestNeighbor class for nearest neighbor search.");

    nearestneighbor.def(py::init<const core::Tensor &>(), "data"_a)
            .def("knn_index", &core::nns::NearestNeighbor::KnnIndex)
            .def("radius_index", &core::nns::NearestNeighbor::RadiusIndex)
            .def("fixed_radius_index",
                 &core::nns::NearestNeighbor::FixedRadiusIndex)
            .def("hybrid_index", &core::nns::NearestNeighbor::HybridIndex)
            .def("knn_search", &core::nns::NearestNeighbor::KnnSearch,
                 "query"_a, "knn"_a)
            .def(
                    "radius_search",
                    [](core::nns::NearestNeighbor &nn_,
                       const core::Tensor &query, py::array np_array) {
                        py::buffer_info info = np_array.request();
                        core::Dtype dtype =
                                pybind_utils::ArrayFormatToDtype(info.format);
                        if (dtype != core::Dtype::Float64) {
                            throw std::runtime_error(
                                    "radius type must be Float64!");
                        }
                        return nn_.RadiusSearch(
                                query, static_cast<double *>(info.ptr));
                    },
                    "query"_a, "radii"_a)
            .def("fixed_radius_search",
                 &core::nns::NearestNeighbor::FixedRadiusSearch, "query"_a,
                 "radius"_a)
            .def("hybrid_search", &core::nns::NearestNeighbor::HybridSearch,
                 "query"_a, "radius"_a, "max_knn"_a);
    docstring::ClassMethodDocInject(m_nn, "NearestNeighbor", "knn_search",
                                    map_nearest_neighbor_search_method_docs);
    docstring::ClassMethodDocInject(m_nn, "NearestNeighbor", "radius_search",
                                    map_nearest_neighbor_search_method_docs);
    docstring::ClassMethodDocInject(m_nn, "NearestNeighbor",
                                    "fixed_radius_search",
                                    map_nearest_neighbor_search_method_docs);
    docstring::ClassMethodDocInject(m_nn, "NearestNeighbor", "hybrid_search",
                                    map_nearest_neighbor_search_method_docs);
}

}  // namespace open3d