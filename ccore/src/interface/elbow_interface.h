/**
*
* Copyright (C) 2014-2018    Andrei Novikov (pyclustering@yandex.ru)
*
* GNU_PUBLIC_LICENSE
*   pyclustering is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   pyclustering is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once


#include "interface/pyclustering_package.hpp"

#include "cluster/elbow.hpp"
#include "cluster/kmeans_plus_plus.hpp"
#include "cluster/random_center_initializer.hpp"

#include "definitions.hpp"


enum elbow_package_indexer {
    ELBOW_PACKAGE_AMOUNT = 0,
    ELBOW_PACKAGE_WCE,
    ELBOW_PACKAGE_SIZE
};


template <class type_initializer>
pyclustering_package * elbow_method(const pyclustering_package * const p_sample,
                                    const std::size_t p_kmin,
                                    const std::size_t p_kmax)
{
    dataset input_dataset;
    p_sample->extract(input_dataset);

    ccore::clst::elbow_data result;
    ccore::clst::elbow<type_initializer> solver(p_kmin, p_kmax);
    solver.process(input_dataset, result);

    pyclustering_package * package = new pyclustering_package(pyclustering_data_t::PYCLUSTERING_TYPE_LIST);
    package->size = ELBOW_PACKAGE_SIZE;
    package->data = new pyclustering_package * [ELBOW_PACKAGE_SIZE];

    std::vector<std::size_t> amount_cluters = { result.get_amount() };
    ((pyclustering_package **) package->data)[ELBOW_PACKAGE_AMOUNT] = create_package(&amount_cluters);
    ((pyclustering_package **) package->data)[ELBOW_PACKAGE_WCE] = create_package(&result.get_wce());

    return package;
}

/**
 *
 * @brief   Performs data analysis using Elbow method with K-Means++ center initialization to found out proper amount of clusters.
 * @details Caller should destroy returned result by 'free_pyclustering_package'.
 *
 * @param[in] p_sample: input data for clustering.
 * @param[in] p_kmin: minimum amount of clusters that should be considered.
 * @param[in] p_kmax: maximum amount of clusters that should be considered.
 *
 * @return  Returns Elbow's analysis results as a pyclustering package [ [ amount of clusters ], [ within cluster errors (wce) ] ].
 *
 */
extern "C" DECLARATION pyclustering_package * elbow_method_ikpp(const pyclustering_package * const p_sample, 
                                                                const std::size_t p_kmin, 
                                                                const std::size_t p_kmax);


/**
 *
 * @brief   Performs data analysis using Elbow method with random center initialization to found out proper amount of clusters.
 * @details Caller should destroy returned result by 'free_pyclustering_package'.
 *
 * @param[in] p_sample: input data for clustering.
 * @param[in] p_kmin: minimum amount of clusters that should be considered.
 * @param[in] p_kmax: maximum amount of clusters that should be considered.
 *
 * @return  Returns Elbow's analysis results as a pyclustering package [ [ amount of clusters ], [ within cluster errors (wce) ] ].
 *
 */
extern "C" DECLARATION pyclustering_package * elbow_method_irnd(const pyclustering_package * const p_sample, 
                                                                const std::size_t p_kmin, 
                                                                const std::size_t p_kmax);
